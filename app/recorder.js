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

  startRecording() {
    this.isRecording = true
  }

  stopRecording() {
    this.isRecording = false
  }

  update(data) {
    // Recording will stop when stopped on ground with engines turned off
    if (this.isRecording &&
      this.aircraftIsOnGround(data) &&
      this.aircraftIsStopped(data) &&
      !this.aircraftEngineIsRunning(data)) {
        this.stopRecording()
    }
    // Recording will start when the first engine is started on ground
    else if (!this.isRecording && this.aircraftIsOnGround(data)) {
        this.startRecording()
    }
  }

  updatePhase(data) {
    console.log(util.inspect(data))
    console.log('Recorder updating with data at time ' + data.totalFlightTimeSec)

    let oldphase = this.phase

    if (this.phase == Phase.RAMP) {
      if (this.aircraftEngineIsRunning(data) && !this.aircraftIsStopped(data)) {
        this.phase = Phase.TAXI_OUT
      }
    }

    else if (this.phase == Phase.TAXI_OUT) {
      if (data.speedIAS > 35 && data.heightFt < 500) {
        this.phase = Phase.TAKEOFF
      }
      else if (this.aircraftIsOnGround() && this.aircraftIsStopped(data) && !this.aircraftEngineIsRunning(data)) {
        this.phase = Phase.RAMP
      }
    }

    else if (this.phase == Phase.TAKEOFF) {
      if (data.verticalSpeedFTM > 150 && data.heightFt >= 500) {
        this.phase = Phase.CLIMB
      }
      if (data.verticalSpeedFTM < -150 && data.heightFt < 500) {
        this.phase = Phase.LANDING
      }
    }

    else if (this.phase == Phase.CLIMB) {
      if (abs(data.verticalSpeedFTM) < 150) {
        this.phase = Phase.CRUISE
      }
      else if (data.verticalSpeedFTM < -150) {
        this.phase = Phase.DESCENT
      }
    }

    else if (this.phase == Phase.CRUISE) {
      if (data.verticalSpeedFTM > 150) {
        this.phase = Phase.CLIMB
      }
      else if (data.verticalSpeedFTM < -150) {
        this.phase = Phase.DESCENT
      }
    }

    else if (this.phase == Phase.DESCENT) {
      if (data.heightFt <= 500) {
        this.phase = Phase.LANDING
      }
    }

    else if (this.phase == Phase.LANDING) {
      if (this.aircraftIsOnGround() && data.speedGS < 35) {
        this.phase = Phase.TAXI_IN
      }
      else if (data.verticalSpeedFTM > 150 && data.heightFt >= 500) {
        this.phase = Phase.CLIMB
      }
    }

    else if (this.phase == Phase.TAXI_IN) {
      if (this.aircraftIsOnGround() && !this.aircraftEngineIsRunning(data) && this.aircraftIsStopped(data)) {
        this.phase = Phase.RAMP
      }
      if (data.speedIAS > 35 && data.heightFt < 500 && data.verticalSpeedFTM > 150) {
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
