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
    this.flight = new Flight(this.flightDb, data)
    this.saveFlight()
    this.isRecording = true
  }

  stopRecording(data) {
    this.saveFlight()
    this.flight.timeIn = Date.now()
    this.flight.totalBlockTime = (this.flight.timeIn - this.flight.timeOut) / 1000 / 3600
    this.flight.totalFlightTime = (this.flight.timeOn - this.flight.timeOff) / 1000 / 3600
    this.flight.fuelIn = data.fuelQuantityKg
    this.flight.usedFuel = this.flight.fuelOut - this.flight.fuelIn
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
    // Recording will stop when stopped on ground with engines turned off
    if (this.isRecording &&
      this.aircraftIsOnGround(data) &&
      this.aircraftIsStopped(data) &&
      (!this.aircraftEngineIsRunning(data) || this.aircraftIsCrashed(data))) {
        this.stopRecording(data)
    }
    // Recording will start when the first engine is started on ground
    else if (!this.isRecording &&
      this.aircraftIsOnGround(data) &&
      this.aircraftEngineIsRunning(data) &&
      !this.aircraftIsCrashed(data)) {
        this.startRecording(data)
    }

    if (this.isRecording) {
      this.saveData(data)
      this.updatePhase(data)
    }
  }

  updatePhase(data) {
    //console.log(util.inspect(data))
    console.log('Recorder updating with data at time ' + data.totalFlightTimeSec)

    let oldphase = this.phase

    if (this.phase == Phase.RAMP) {
      if (this.aircraftEngineIsRunning(data) && !this.aircraftIsStopped(data)) {
        this.phase = Phase.TAXI_OUT
        this.flight.timeOut = Date.now()
        this.flight.fuelOut = data.fuelQuantityKg
        this.saveFlight()
      }
    }

    else if (this.phase == Phase.TAXI_OUT) {
      if (data.speedIAS > 35 && data.heightFt < 500) {
        this.phase = Phase.TAKEOFF
        this.flight.timeOff = Date.now()
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
        this.flight.timeOff = Date.now()
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
        this.flight.timeOn = Date.now()
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
          this.flight.timeIn = Date.now()
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
