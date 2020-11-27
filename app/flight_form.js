'use strict'

const { ipcRenderer, remote } = require('electron')
const Store = require('electron-store')

ipcRenderer.on('config-data-read', (event, doc) => {
  document.getElementById('inputVA').value = doc.va
  document.getElementById('inputURL').value = doc.url
  document.getElementById('inputUsername').value = doc.username
  document.getElementById('inputPassword').value = doc.password
  document.getElementById('inputIP').value = doc.ip
})

document.getElementById('submitBtn').addEventListener('click', () => {
  const form = document.getElementById('form-settings')
  const settings = {
    va: form.elements.inputVA.value,
    url: form.elements.inputURL.value,
    username: form.elements.inputUsername.value,
    password: form.elements.inputPassword.value,
    ip: form.elements.inputIP.value,
  }
  ipcRenderer.send('config-data-write', settings)
})

document.getElementById('cancelBtn').addEventListener('click', () => {
  remote.getCurrentWindow().close()
})
