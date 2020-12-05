'use strict'

const { ipcRenderer } = require('electron')
const dateFormat = require('dateformat')

document.getElementById('configBtn').addEventListener('click', () => {
  ipcRenderer.send('open-settings')
})

document.getElementById('recorded-flights').addEventListener('click', (ev) => {
  ipcRenderer.send('open-flight', ev.target.parentNode.dataset.flightId)
})

document.getElementById('syncBtn').addEventListener('click', (ev) => {
  ipcRenderer.send('sync-flights')
})

document.getElementById('clearBtn').addEventListener('click', (ev) => {
  ipcRenderer.send('clear-flights')
})

function updateStateParam(id, value) {
  document.getElementById(id).innerHTML = value
}

ipcRenderer.on('state-update', (event, data) => {
  updateStateParam('span-aircraft', data.aircraftType)
  updateStateParam('span-onground', data.heightFt < 1 ? 'yes' : 'no')
  updateStateParam('span-engines-running', data.engineRunning.includes(1) ? 'yes' : 'no')
  //updateStateParam('span-location', data.latitudeDeg + ', ' + data.longitudeDeg)
  updateStateParam('span-location', data.nearestAirportName +
    ' (' + data.nearestAirportId + ')')
  updateStateParam('span-heading', data.headingDeg)
  updateStateParam('span-track', data.trackDeg)
  updateStateParam('span-gs', data.speedGS)
  updateStateParam('span-ias', data.speedIAS)
  updateStateParam('span-altitude', data.altitudeFt)
  updateStateParam('span-vs', data.verticalSpeedFPM)
})

ipcRenderer.on('connection-state-changed', (event, status) => {
  const span = document.getElementById('span-xplane')
  if (status.connected) {
    span.innerHTML = 'X-Plane connected'
    span.className = 'label label-success'

  } else {
    span.innerHTML = 'X-Plane not connected'
    span.className = 'label label-error'
  }
})

ipcRenderer.on('recording-state-update', (event, status) => {
  const span = document.getElementById('span-recorder')
  if (status.recording) {
    span.innerHTML = 'Recording flight'
    span.className = 'label label-error'

  } else {
    span.innerHTML = 'Recorder not active'
    span.className = 'label label-warning'
  }
})

ipcRenderer.on('flight-list', (event, flights) => {
  // Clear list
  for (const tr of document.querySelectorAll("#recorded-flights tr")) {
    tr.remove()
  }

  // Populate list
  const t = document.querySelector("template.flight-entry")
  for (const f of flights) {
    const newFlight = t.content.cloneNode(true)
    newFlight.querySelector("tr").dataset.flightId = f.id
    const d = new Date(f.timeOut * 1000)
    newFlight.querySelector("td.flight-date").innerHTML = dateFormat(d, "UTC:yyyy-mm-dd")
    newFlight.querySelector("td.flight-time").innerHTML = dateFormat(d, "UTC:HH:MM")
    newFlight.querySelector("td.flight-number").innerHTML = f.number || 'N/A'
    newFlight.querySelector("td.flight-aircraft").innerHTML = f.aircraftType || 'N/A'
    newFlight.querySelector("td.flight-departure").innerHTML = f.departure || 'N/A'
    newFlight.querySelector("td.flight-destination").innerHTML = f.destination || 'N/A'
    newFlight.querySelector("td.flight-duration").innerHTML = f.totalBlockTime.toFixed(2) || 'N/A'
    newFlight.querySelector("td.flight-fuel").innerHTML = f.usedFuel || 'N/A'

    let status
    if (!f.timeIn || !f.timeOff || !f.timeOn || !f.timeIn) {
      status = 'Interrupted'
    }
    else if (!f.number || !f.departure || !f.destination || !f.alternate || !f.route) {
      status = 'Information Missing'
    }
    else if (!f.sent) {
      status = 'Ready to Send'
    }
    else {
      status = 'Sent'
    }
    newFlight.querySelector("td.flight-status").innerHTML = status

    t.parentNode.appendChild(newFlight)
  }
})
