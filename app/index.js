'use strict'

const { ipcRenderer } = require('electron')

document.getElementById('configBtn').addEventListener('click', () => {
  ipcRenderer.send('open-settings')
})

document.getElementById('recorded-flights').addEventListener('click', (ev) => {
  ipcRenderer.send('open-flight', {id: ev.target.parentNode.dataset.flightId})
})
