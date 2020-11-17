'use strict'

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')
const net = require('net')

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

function setupNetwork() {
  const client = net.connect({
    host: '192.168.1.49',
    port: 45558,
    onread: {
      // Reuses a 4KiB Buffer for every read from the socket.
      buffer: Buffer.alloc(4 * 1024),
      callback: function(nread, buf) {
        // Received data is available in `buf` from 0 to `nread`.
      //  console.log(buf.toString('utf8', 0, nread))
        const dataPoint = JSON.parse(buf.toString('utf8', 0, nread))
        console.log(util.inspect(dataPoint))
        ipcMain.emit('datapoint-received', dataPoint)
      }
    }
  })

  client.on('error', (err) => {
    console.log('client error: ' + err)
  })
  client.on('end', () => {
    console.log('client ended connection')
  })
  client.on('close', () => {
    console.log('client closed the connection')
  })

}

app.whenReady().then(() => {
  const mainWindow = createWindow()
  ipcMain.on('datapoint-received', (data) => {
    mainWindow.webContents.send('state-update', data)
  })

  setupSettings(mainWindow)
  setupFlightDetails(mainWindow)
  setupNetwork()

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})
