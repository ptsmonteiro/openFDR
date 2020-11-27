'use strict'

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')
const net = require('net')
const util = require('util')
const Recorder = require('./recorder')

const Datastore = require('nedb')
const db = {
  config: new Datastore({ filename: 'config.db', autoload: true }),
  flights: new Datastore({ filename: 'flights.db', autoload: true }),
  data: new Datastore({ filename: 'telemetry.db', autoload: true }),
}

const CONNECTION_RETRY_TIME = 2

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
      height: 548,
      show: false,
      modal: true,
      parent: parentWindow
    })
    settingsWindow.loadFile('settings.html')
    settingsWindow.once('ready-to-show', () => {
      db.config.findOne({}, (err, doc) => {
        settingsWindow.webContents.send('config-data-read', doc)
        settingsWindow.show()
      })
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

function loadFlights(mainWindow) {
  db.flights.find({}, (err, docs) => {
    mainWindow.webContents.send('flight-list', docs)
  })
}

function setupFlightDetails(parentWindow) {
  let flightWindow
  ipcMain.on('open-flight', (event, id) => {
    if (flightWindow) return
    flightWindow = new BrowserWindow({
      webPreferences: { nodeIntegration: true, enableRemoteModule: true},
      width: 400,
      height: 500,
      show: false,
      modal: true,
      parent: parentWindow
    })
    flightWindow.loadFile('flight_form.html')
    flightWindow.once('ready-to-show', () => {
      db.flights.findOne({id: id}, (err, doc) => {
        flightWindow.webContents.send('flight-data', doc)
        flightWindow.show()
      })
    })
    flightWindow.on('closed', () => {
      flightWindow = null
      loadFlights(parentWindow)
    })
  })

  ipcMain.on('flight-data-merge', (event, data) => {
    console.log('updating flight details for ' + data.id)
    db.flights.update({id: data.id}, {$set: data}, {}, (err, affected) => {
      if (affected) { console.log('flight updated') }
      if (err) { console.log('error ' + err) }
      flightWindow.close()
    })
  })
}

function setupNetwork() {
  db.config.findOne({}, (err, doc) => {

    const ip = (err || !doc) ? '127.0.0.1' : doc.ip
    console.log('Connecting to ' + ip + '...')
    const client = net.connect({
      host: ip,
      port: 45558,
      onread: {
        // Reuses a 4KiB Buffer for every read from the socket.
        buffer: Buffer.alloc(4 * 1024),
        callback: function(nread, buf) {
          // Received data is available in `buf` from 0 to `nread`.
        //  console.log(buf.toString('utf8', 0, nread))
          const dataPoint = JSON.parse(buf.toString('utf8', 0, nread))
          //console.log(util.inspect(dataPoint))
          ipcMain.emit('datapoint-received', dataPoint)
        }
      }
    })

    client.on('error', (err) => {
      console.log('network error: ' + err)
    })
    client.on('end', () => {
      ipcMain.emit('xplane-disconnected')
      console.log('x-plane connection ended')
    })
    client.on('close', () => {
      ipcMain.emit('xplane-disconnected')
      console.log('x-plane connection closed')
      setTimeout(setupNetwork, CONNECTION_RETRY_TIME * 1000)
    })
    client.on('connection', () => {
      ipcMain.emit('xplane-connected')
      console.log('x-plane connection established')
    })

  })

}

app.whenReady().then(() => {
  const mainWindow = createWindow()
  const recorder = new Recorder(db.flights, db.data)

  ipcMain.on('datapoint-received', (data) => {
    mainWindow.webContents.send('state-update', data)
    recorder.update(data)
  })

  ipcMain.on('recording-started', (data) => {
    loadFlights(mainWindow)
  })
  ipcMain.on('recording-stopped', (data) => {
    loadFlights(mainWindow)
  })

  setupSettings(mainWindow)

  loadFlights(mainWindow)
  setupFlightDetails(mainWindow)

  setupNetwork()

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})
