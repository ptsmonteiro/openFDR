# openFDR app

Connect your Virtual Airline to your pilots using X-Plane and analyse all the flight data
in real time.

openFDR runs on Windows, MacOS and Linux (binary packages not yet available).

## Usage

```bash
yarn start
```

## Virtual Airline API
openFDR will connect to your Virtual Airline using a configurable base URL.

**It's strongly advised that HTTPS is enforced by the server on every request.**

### Data Model
The data model is very simple. There are *flights* and
*recording samples*. Each flight has a list of recording samples.
Flights might only have partial data at a given point, however, the Virtual Airline
should define it's own rules on approving a flight and should check the flight data in order to do so.

#### <a id="flight"></a>Flight
```json
{
  "id": "LFPO.LPPT.202005301011.A320.7124",
  "number": "ZE999",
  "aircraftType": "A320",
  "aircraftRegistration": "CS-TLM",
  "departure": "LFPO",
  "destination": "LPPT",
  "alternate": "LPPR",
  "route": "ESP UL120 BLM",
  "timeOut": "2012-01-18T11:45:00+01:00",
  "timeOff": "2012-01-18T11:50:00+01:00",
  "timeOn": "2012-01-18T14:30:00+01:00",
  "timeIn": "2012-01-18T14:45:00+01:00",
  "totalBlockTime": 2.5,
  "totalFlightTime": 2.3,
  "dryOperatingWeight": 60000,
  "payloadWeight": 12000,
  "fuelOut": 9000,
  "fuelOff": 8800,
  "fuelOn": 3000,
  "fuelIn": 2800
}
```

#### <a id="recording-sample"></a>Recording Sample
```json
{
  "simulatorVersion": 115033,
  "aircraftType": "BE58",
  "aircraftNumberOfEngines": 2,
  "aircraftRegistration": "N45XS",
  "aircraftEmptyWeightKg": 1807,
  "aircraftMaxWeightKg": 2495,
  "elapsedFlightTime": 859.83,
  "totalFlightTimeSec": 784.71,
  "flightTimeStamp": 41978.6,
  "headingDeg": 157,
  "heightFt": 1042,
  "altitudeFt": 1791,
  "verticalSpeedFPM": 1749,
  "speedIAS": 107,
  "speedMach": 0.167776,
  "pitchDeg": 12.2928,
  "bankDeg": -10.1624,
  "alpha": 3.70164,
  "loadFactorG": 0.980528,
  "trackDeg": 114,
  "speedGS": 96,
  "latitudeDeg": 44.341,
  "longitudeDeg": 1.49842,
  "nearestAirportId": "LFCC",
  "nearestAirportName": "Cahors Lalbenque",
  "windDeg": 131,
  "windKt": 14,
  "oat": 13,
  "engineRunning": [
    1,
    1
  ],
  "engineLever": [
    100,
    100
  ],
  "enginePowerWatt": [
    197856,
    197856
  ],
  "brakeLeft": 0,
  "brakeRight": 0,
  "parkingBrake": 0,
  "gearLeverDown": 0,
  "pitchControl": 0,
  "rollControl": 2,
  "yawControl": 0,
  "flapLever": 0,
  "speedBrakeLever": 0,
  "autopilotOn": 0,
  "weightKg": 2180,
  "fuelQuantityKg": 270,
  "cabinAltFt": 1828,
  "cabinVsFPM": 1687,
  "nav1Id": "",
  "nav1CourseDeg": 0,
  "nav1SlopeDeg": 0,
  "nav1DME": 0,
  "locDevDots": 0,
  "glideDevDots": 0
}
```

### Headers
openFDR will pass these headers in every request:
- X-openFDR-Username
- X-openFDR-Password

### Operations

#### POST: /flight
Creates/updates a new/existing flight.

openFDR will call this regularly from the moment at least one engine is started
to the moment engines are shutdown.

openFDR will call this to submit partial or complete information for a flight.
The server should create or update the flight according to the provided flight ID.

##### request {#flight}
```json
{
  "flight": {...},
  "sample": {...}
}
```
"flight" is a [Flight data](#flight) (complete or partial) object and "sample" is a [Recording sample](#recording-sample) (optional).

##### response status code
- HTTP STATUS 200 (No errors)
- HTTP STATUS 401 (Username/password mismatch)
- HTTP STATUS 500 (Server side error)

#### POST: /recording
Send recording data for a flight.

openFDR might call this operation several times with different sample sets to contain the payload size of a single call.
This allows for post flight analysis on the Virtual Airline (check exceedance of limits, level of realism, etc).

##### request
```json
{
  "flightId": "LFPO.LPPT.202005301011.A320.7124",
  "samples": [...]
}
```
"samples" is a list of [recording samples](#recording-sample).

##### response status code
- HTTP STATUS 200 (No errors)
- HTTP STATUS 401 (Username/password mismatch)
- HTTP STATUS 404 (Flight not found)
- HTTP STATUS 500 (Server side error)
