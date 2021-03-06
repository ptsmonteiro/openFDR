const {ipcMain} = require('electron')

const util = require('util')
const Flight = require('./flight')

const Phase = {
  RAMP: 0,
  TAXI_OUT: 1,
  TAKEOFF: 2,
  CLIMB: 3,
  CRUISE: 4,
  DESCENT: 5,
  LANDING: 6,
  TAXI_IN: 7
}

class Recorder {
  constructor(flightDb, dataDb) {
    this.flightDb = flightDb
    this.dataDb = dataDb
    this.flight = null
    this.phase = Phase.RAMP
    this.isRecording = false
  }

  aircraftIsOnGround(data) {
    return (data.heightFt < 1)
  }

  aircraftEngineIsRunning(data) {
    return data.engineRunning.includes(1)
  }

  aircraftIsStopped(data) {
    return data.speedGS < 1
  }

  aircraftIsCrashed(data) {
    return (data.isCrashed ? true : false)
  }

  startRecording(data) {
    console.log('starting recording')
    this.flight = new Flight(this.flightDb, data)
    this.saveFlight()
    this.phase = Phase.RAMP
    this.isRecording = true
    ipcMain.emit('recording-started')
  }

  stopRecording(data) {
    console.log('stopping recording')
    this.flight.timeIn = Math.floor(Date.now()/1000)
    this.flight.totalBlockTime = Math.floor((this.flight.timeIn - this.flight.timeOut) / 36) / 100
    this.flight.totalFlightTime = Math.floor((this.flight.timeOn - this.flight.timeOff) / 36) / 100
    this.flight.fuelIn = data.fuelQuantityKg
    this.flight.usedFuel = this.flight.fuelOut - this.flight.fuelIn
    this.saveFlight()
    ipcMain.emit('recording-stopped')
    this.isRecording = false
  }

  saveData(data) {
    console.log('saving data snapshot')
    let d = data
    d.flightId = this.flight.id
    this.dataDb.insert(d, (err, doc) => {
      if (err) { console.log('error saving data snapshot: ' + err) }
    })
  }

  saveFlight() {
    console.log('saving flight')
    this.flightDb.update({'id': this.flight.id}, this.flight, {upsert: true}, (err, doc) => {
      if (err) { console.log('error saving flight: ' + err) }
    })
  }

  update(data) {
    if (this.isRecording) {
      this.saveData(data)
      this.updatePhase(data)

      // Recording will stop when stopped on ground with engines turned off or if crashed
      if (this.aircraftIsOnGround(data) &&
        this.aircraftIsStopped(data) &&
        (!this.aircraftEngineIsRunning(data) || this.aircraftIsCrashed(data))) {
          this.stopRecording(data)
      }

    }
    // Recording will start when the first engine is started on ground
    else if (!this.isRecording &&
      this.aircraftIsOnGround(data) &&
      this.aircraftEngineIsRunning(data) &&
      !this.aircraftIsCrashed(data)) {
        this.startRecording(data)
    }
  }

  updatePhase(data) {
    //console.log(util.inspect(data))
    console.log('Recorder updating with data at time ' + data.totalFlightTimeSec)

    let oldphase = this.phase

    if (this.phase == Phase.RAMP) {
      if (this.aircraftEngineIsRunning(data) && !this.aircraftIsStopped(data)) {
        this.phase = Phase.TAXI_OUT
        this.flight.timeOut = Math.floor(Date.now()/1000)
        this.flight.fuelOut = data.fuelQuantityKg
        this.saveFlight()
      }
    }

    else if (this.phase == Phase.TAXI_OUT) {
      if (data.speedIAS > 35 && data.heightFt < 500) {
        this.phase = Phase.TAKEOFF
        this.flight.timeOff = Math.floor(Date.now()/1000)
        this.flight.fuelOff = data.fuelQuantityKg
        this.saveFlight()
      }
      else if (this.aircraftIsOnGround(data) && this.aircraftIsStopped(data) && !this.aircraftEngineIsRunning(data)) {
        this.phase = Phase.RAMP
        this.saveFlight()
      }
    }

    else if (this.phase == Phase.TAKEOFF) {
      if (data.verticalSpeedFPM > 150 && data.heightFt >= 500) {
        this.phase = Phase.CLIMB
        this.flight.timeOff = Math.floor(Date.now()/1000)
        this.flight.fuelOff = data.fuelQuantityKg
        this.saveFlight()
      }
      if (data.verticalSpeedFPM < -150 && data.heightFt < 500) {
        this.phase = Phase.LANDING
      }
    }

    else if (this.phase == Phase.CLIMB) {
      if (Math.abs(data.verticalSpeedFPM) < 150) {
        this.phase = Phase.CRUISE
      }
      else if (data.verticalSpeedFPM < -150) {
        this.phase = Phase.DESCENT
      }
    }

    else if (this.phase == Phase.CRUISE) {
      if (data.verticalSpeedFPM > 150) {
        this.phase = Phase.CLIMB
      }
      else if (data.verticalSpeedFPM < -150) {
        this.phase = Phase.DESCENT
      }
    }

    else if (this.phase == Phase.DESCENT) {
      if (data.heightFt <= 500) {
        this.phase = Phase.LANDING
      }
    }

    else if (this.phase == Phase.LANDING) {
      if (this.aircraftIsOnGround(data) && data.speedGS < 35) {
        this.phase = Phase.TAXI_IN
        this.flight.timeOn = Math.floor(Date.now()/1000)
        this.flight.fuelOn = data.fuelQuantityKg
        this.flight.destination = data.nearestAirportId
        this.saveFlight()
      }
      else if (data.verticalSpeedFPM > 150 && data.heightFt >= 500) {
        this.phase = Phase.CLIMB
      }
    }

    else if (this.phase == Phase.TAXI_IN) {
      if (this.aircraftIsOnGround(data) &&
        !this.aircraftEngineIsRunning(data) &&
        this.aircraftIsStopped(data)) {
          this.phase = Phase.RAMP
          this.flight.timeIn = Math.floor(Date.now()/1000)
          this.flight.fuelIn = data.fuelQuantityKg
          this.saveFlight()
      }
      if (data.speedIAS > 35 && data.heightFt < 500 && data.verticalSpeedFPM > 150) {
        this.phase = Phase.TAKEOFF
      }
    }

    if (oldphase != this.phase) {
      console.log('Phase change from ' + oldphase + ' to ' + this.phase)
    }

    console.log('Current phase is ' + this.phase)

  }
}

module.exports = Recorder
