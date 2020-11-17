'use strict'

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const path = require('path')

const Datastore = require('nedb')
const db = {
  config: new Datastore({ filename: 'config.db', autoload: true }),
  flights: new Datastore({ filename: 'flights.db', autoload: true }),
  data: new Datastore({ filename: 'telemetry.db', autoload: true }),
}

const util = require('util')

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 700,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('index.html')

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()
  return mainWindow
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  const mainWindow = createWindow()

  let settingsWindow
  ipcMain.on('open-settings', () => {
    if (settingsWindow) return
    settingsWindow = new BrowserWindow({
      webPreferences: { nodeIntegration: true, enableRemoteModule: true},
      width: 500,
      height: 500,
      show: false,
      modal: true,
      parent: mainWindow
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
    console.log('updating with doc ' + util.inspect(doc))
    db.config.update({}, doc, {upsert: true}, (err, doc) => {
      if (err) {
        console.log('error ' + err)
      }
      settingsWindow.close()
    })
  })


  let flightWindow
  ipcMain.on('open-flight', () => {
    if (flightWindow) return
    flightWindow = new BrowserWindow({
      webPreferences: { nodeIntegration: true, enableRemoteModule: true},
      width: 400,
      height: 500,
      show: false,
      modal: true,
      parent: mainWindow
    })
    flightWindow.loadFile('flight.html')
    flightWindow.once('ready-to-show', () => {
      flightWindow.show()
    })
    flightWindow.on('closed', () => {
      flightWindow = null
    })
  })

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
