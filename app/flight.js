class Flight {

  constructor(flightDb, data) {
    // general
    this.id = this.idFromData(data)
    this.number = ''
    this.aircraftType = ''
    this.departure = ''
    this.destination = ''
    this.alternate = ''

    // times
    this.timeOut = 0
    this.timeOff = 0
    this.timeOn = 0
    this.timeIn = 0
    this.totalBlockTime = 0
    this.totalFlightTime = 0

    // weights
    this.dryOperatingWeight = 0
    this.payloadWeight = 0
    this.totalBlockOffWeight = 0
    this.totalBlockOnWeight = 0
    this.usedFuelWeight = 0

    // flight report sent
    this.sent = false
  }

  idFromData(data) {
    let id = Date.now() + '.' + data.nearestAirportId + '.' + data.aircraftType
    return id
  }

}

module.exports = Flight
