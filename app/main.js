'use strict'

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')
const net = require('net');

const Datastore = require('nedb')
const db = {
  config: new Datastore({ filename: 'config.db', autoload: true }),
  flights: new Datastore({ filename: 'flights.db', autoload: true }),
  data: new Datastore({ filename: 'telemetry.db', autoload: true }),
}

const util = require('util')

function createWindow () {
  const mainWindow = new BrowserWindow({
    width: 700,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true
    }
  })
  mainWindow.loadFile('index.html')
  return mainWindow
}

function setupSettings(parentWindow) {
  let settingsWindow
  ipcMain.on('open-settings', () => {
    if (settingsWindow) return
    settingsWindow = new BrowserWindow({
      webPreferences: { nodeIntegration: true, enableRemoteModule: true},
      width: 500,
      height: 500,
      show: false,
      modal: true,
      parent: parentWindow
    })
    settingsWindow.loadFile('settings.html')
    settingsWindow.once('ready-to-show', () => {
      db.config.findOne({}, (err, doc) => {
        settingsWindow.webContents.send('config-data-read', doc)
      })
      settingsWindow.show()
    })
    settingsWindow.on('closed', () => {
      settingsWindow = null
    })
  })

  ipcMain.on('config-data-write', (event, doc) => {
    db.config.update({}, doc, {upsert: true}, (err, doc) => {
      if (err) { console.log('error ' + err) }
      settingsWindow.close()
    })
  })
}

function setupFlightDetails(parentWindow) {
  let flightWindow
  ipcMain.on('open-flight', () => {
    if (flightWindow) return
    flightWindow = new BrowserWindow({
      webPreferences: { nodeIntegration: true, enableRemoteModule: true},
      width: 400,
      height: 500,
      show: false,
      modal: true,
      parent: parentWindow
    })
    flightWindow.loadFile('flight.html')
    flightWindow.once('ready-to-show', () => {
      flightWindow.show()
    })
    flightWindow.on('closed', () => {
      flightWindow = null
    })
  })
}

function start() {
  const mainWindow = createWindow()

  setupSettings(mainWindow)
  setupFlightDetails(mainWindow)

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })

}

app.whenReady().then(() => {
  start()
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})
