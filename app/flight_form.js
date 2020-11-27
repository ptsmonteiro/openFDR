'use strict'

const { ipcRenderer, remote } = require('electron')
const Store = require('electron-store')

ipcRenderer.on('flight-data', (event, doc) => {
  const form = document.querySelector('form')
  form.elements.id.value = doc.id
  form.elements.inputFlightNumber.value = doc.number
  form.elements.inputType.value = doc.aircraftType
  form.elements.inputDeparture.value = doc.departure
  form.elements.inputArrival.value = doc.destination
  form.elements.inputRoute.value = doc.route
  form.elements.inputRemarks.value = doc.remarks
})

document.querySelector('form').addEventListener('submit', (ev) => {
  ev.preventDefault()
  const form = ev.target
  const flight = {
    id: form.elements.id.value,
    number: form.elements.inputFlightNumber.value,
    aircraftType: form.elements.inputType.value,
    departure: form.elements.inputDeparture.value,
    destination: form.elements.inputArrival.value,
    alternate: form.elements.inputAlternate.value,
    route: form.elements.inputRoute.value,
    remarks: form.elements.inputRemarks.value,
  }
  ipcRenderer.send('flight-data-merge', flight)
})

document.getElementById('cancelBtn').addEventListener('click', () => {
  remote.getCurrentWindow().close()
})
