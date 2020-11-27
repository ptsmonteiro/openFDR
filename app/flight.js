class Flight {

  constructor(flightDb, data) {
    // general
    this.id = this.idFromData(data)
    this.number = ''
    this.aircraftType = data.aircraftType
    this.departure = data.nearestAirportId
    this.destination = ''
    this.alternate = ''
    this.route = ''
    this.remarks = ''
    this.onlineNetwork = ''

    // times
    this.timeOut = Date.now()
    this.timeOff = 0
    this.timeOn = 0
    this.timeIn = 0
    this.totalBlockTime = 0
    this.totalFlightTime = 0

    // fuel
    this.fuelOut = 0
    this.fuelOff = 0
    this.fuelOn = 0
    this.fuelIn = 0
    this.usedFuel = 0

    // weights
    this.totalWeight = data.totalWeight
    this.payloadWeight = data.payloadWeight

    // flight report sent
    this.sent = false
  }

  idFromData(data) {
    let id = Date.now() + '.' + data.nearestAirportId + '.' + data.aircraftType
    return id
  }

}

module.exports = Flight
