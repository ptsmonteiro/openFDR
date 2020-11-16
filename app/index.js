'use strict'

const { ipcRenderer } = require('electron')

document.getElementById('configBtn').addEventListener('click', () => {
  ipcRenderer.send('open-settings')
})
