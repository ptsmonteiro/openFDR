'use strict'

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')
const net = require('net')
const util = require('util')
const axios = require('axios')
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
    width: 800,
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
  db.flights.find({}).sort({timeOut: -1}).exec((err, docs) => {
    if (err) { console.log('error ' + err) }
    //console.log('got documents: ' + util.inspect(docs))
    mainWindow.webContents.send('flight-list', docs)
  })
}

function clearFlights() {
  return new Promise((resolve, reject) => {
    db.flights.find({}).sort({timeOut: -1}).exec((err, flights) => {
      if(err) {
        resolve(err)
      }
      else {
        let toRemove = []
        for (const f of flights) {
          if (!f.timeIn || !f.timeOff) {
            // skip the unfinished still recording flight
            if (f.id === flights[0].id && recorder.isRecording) {
              console.log('Skipping flight ' + f.id)
              continue
            }
            toRemove.push(f.id)
          }
          else if (f.sent) {
            toRemove.push(f.id)
          }
        }
        console.log('Removing flights: ' + util.inspect(toRemove))
        db.flights.remove({id: {$in: toRemove}}, {multi: true}, (err, nrRemoved) => {
          if (err) {
            reject(err)
          } else {
            resolve(nrRemoved)
          }
        })
      }
    })
  })
}

function syncFlights(config) {
  const vaURL = config.url
  return new Promise((resolve, reject) => {
    db.flights.find({}).sort({timeOut: 1}).exec(async (err, flights) => {
      for (const f of flights) {
        // TODO Better check required
        if (f.sent || !f.timeOut && !f.timeOff) continue
        console.log(`Submitting flight: ${f.id}`)
        await axios({
          method: 'post',
          url: vaURL + '/flight',
          data: { flight: f },
          headers: {
            'X-openFDR-Username': config.username,
            'X-openFDR-Password': config.password
          }
        })
          .then(res => {
            console.log(util.inspect(res.data))
            console.log(`Flight ${f.id} submitted sucessfully (server id '${res.data.data.id}')`)
            //db.flights.update({ id:f.id }, { $set: { sent: 1 } }, {})
          })
          .catch(error => {
            console.error(util.inspect(error.response.data))
            reject(error)
          })
      }
      resolve();
    })
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

function setupNetwork(mainWindow) {
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
          const message = buf.toString('utf8', 0, nread)
          //console.log(message)
          try {
            const dataPoint = JSON.parse(message)
            //console.log(util.inspect(dataPoint))
            ipcMain.emit('datapoint-received', dataPoint)
          } catch (error) {
              console.error(error)
          }
        }
      }
    })
    client.on('connect', () => {
      ipcMain.emit('connection-state-changed', {connected: true})
      console.log('network connected')
    })

    client.on('error', (err) => {
      console.log('network error: ' + err)
    })
    client.on('end', () => {
      ipcMain.emit('connection-state-changed', {connected: false})
      console.log('x-plane connection ended')
    })
    client.on('close', () => {
      ipcMain.emit('connection-state-changed', {connected: false})
      console.log('x-plane connection closed')
      setTimeout(setupNetwork, CONNECTION_RETRY_TIME * 1000)
    })
    client.on('connection', () => {
      ipcMain.emit('connection-state-changed', {connected: false})
      console.log('x-plane connection established')
    })

  })

}

app.whenReady().then(() => {
  const mainWindow = createWindow()
  const recorder = new Recorder(db.flights, db.data)

  mainWindow.once('ready-to-show', () => {
    ipcMain.on('datapoint-received', (data) => {
      mainWindow.webContents.send('data-update', data)
      recorder.update(data)
    })

    ipcMain.on('recording-started', () => {
      mainWindow.webContents.send('recording-state-update', {recording: true})
      loadFlights(mainWindow)
    })
    ipcMain.on('recording-stopped', () => {
      mainWindow.webContents.send('recording-state-update', {recording: false})
      loadFlights(mainWindow)
    })

    setupSettings(mainWindow)

    setupFlightDetails(mainWindow)
    loadFlights(mainWindow)

    ipcMain.on('sync-flights', () => {
      //mainWindow.webContents.send('recording-state-update', {recording: false})
      db.config.findOne({}, (err, config) => {
        syncFlights(config)
          .then(() => {
            loadFlights(mainWindow)
          })
          .catch((error) => {
            console.error(`Flight sync failed: ${error}`)
          })
      })

    })
    ipcMain.on('clear-flights', () => {
      //mainWindow.webContents.send('recording-state-update', {recording: false})
      clearFlights().then(() => {
        loadFlights(mainWindow)
      })
    })

    ipcMain.on('connection-state-changed', (status) => {
      mainWindow.webContents.send('connection-state-changed', status)
    })
    setupNetwork(mainWindow)
  })

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})
