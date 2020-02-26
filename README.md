# openFDR
X-Plane plugin for flight analysis

The X-Plane community needed an opensource ACARS plugin for all the operating systems supported by X-Plane. The alternatives are closed source and not actively maintained. This is an effort to provide a viable alternative that can be continuously developed by the community and evolve in time.

At this point only the basic flight data recording is working. This project is work in progress and any extra help will be more than welcome.

## Features

* Compatible with X-Plane 11 (v10 not tested yet)
* Compatible with Mac, Linux and Windows

* Detailed flight data recording and upload for post-flight event analysis (upload not working yet)
* Loading of flight plan via company site or XSquawkBox (not implemented yet)
* Flight plan export to XSquawkBox (not implemented yet)
* Live ACARS reports (compatible with the xACARS format) (not implemented yet)
* Final flight report (PIREP) upload (not implemented yet)

## Build instructions

The general idea is that we're compiling the source files using the SDK header files and linking them with the XPLM and XPWidgets libraries.
The final product should be a binary file named mac.xpl for macOS, win.xpl for Windows and lin.xpl for Linux what should be in <your x-plane base directory>/Resources/plugins/openFDR/64.

You can debug by firing up X-Plane and attaching your IDE (Xcode, Visual Studio or other) debugger to the running X-Plane executable. You can configure your IDE to this when you press the debug/run buttons.

### macOS

Xcode will work just fine for macOS. For initial project setup pay special attention to:

* Project -> Build Settings -> Other Linker Flags:
    $(OTHER_LDFLAGS) -Wl,-exported_symbol -Wl,_XPluginStart -Wl,-exported_symbol -Wl,_XPluginEnable -Wl,-exported_symbol -Wl,_XPluginReceiveMessage -Wl,-exported_symbol -Wl,_XPluginDisable -Wl,-exported_symbol -Wl,_XPluginStop

* Project -> Build Settings -> Packaging:
    Executable Extension -> "xpl"

* Project -> Build Settings -> Header Search Paths:
    $(XPSDK_ROOT)/CHeaders/Widgets
    $(XPSDK_ROOT)/CHeaders/XPLM
    
* Project -> Build Settings -> Preprocessor Macros:
    APL=1
    IBM=0
    LIN=0
    XPLM200=1
    XPLM210=1
    
* Project -> Build Settings -> User-Defined:
        XPSDK_ROOT = SDK
        
* Target name: Rename target to "mac"
    
* Target -> Build Settings -> Deployment
    Set Installation Directory to <your x-plane base>"/Resources/plugins/openFDR/64"

* Target -> Build Phases -> Linking Binary With Libraries
    Add XPLM and XPWidgets libraries


### Windows

You can use Visual Studio for building the plugin. No detailled instructions are available for the moment but you should be able to setup Visual Studio with the information available above.

### Linux

For the moment no work has been made to build the project for Linux but the process shouldn't be far from the general idea described above.
