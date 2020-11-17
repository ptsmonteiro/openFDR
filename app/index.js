'use strict'

const { ipcRenderer } = require('electron')

function updateStateParam(id, value) {
  document.getElementById(id).innerHTML = value
}

document.getElementById('configBtn').addEventListener('click', () => {
  ipcRenderer.send('open-settings')
})

document.getElementById('recorded-flights').addEventListener('click', (ev) => {
  ipcRenderer.send('open-flight', {id: ev.target.parentNode.dataset.flightId})
})

ipcRenderer.on('state-update', (event, data) => {
  updateStateParam('span-aircraft', data.aircraftType)
  updateStateParam('span-onground', data.heightFt < 1 ? 'yes' : 'no')
  updateStateParam('span-engines-running', data.engineRunning.includes(1) ? 'yes' : 'no')
  updateStateParam('span-location', data.latitudeDeg + ', ' + data.longitudeDeg)
  updateStateParam('span-heading', data.headingDeg)
  updateStateParam('span-track', data.trackDeg)
  updateStateParam('span-gs', data.speedGS)
  updateStateParam('span-ias', data.speedIAS)
  updateStateParam('span-altitude', data.altitudeFt)
  updateStateParam('span-vs', data.verticalSpeedFPM)
})
