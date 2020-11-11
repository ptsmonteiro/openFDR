//
//  widgets.c
//  openFDR
//
//  Created by Pedro on 01/06/2018.
//

#include <stdio.h>
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "ui.h"

Flight * UI::flight = NULL;

XPWidgetID    UI::PluginWindow = NULL;
XPWidgetID    UI::inputFlightNumber = NULL;
XPWidgetID    UI::inputOriginICAO = NULL;
XPWidgetID    UI::inputDestinationICAO = NULL;
XPWidgetID    UI::inputAlternateICAO = NULL;

XPWidgetID    UI::inputAircraftType = NULL;
XPWidgetID    UI::inputFlightRules = NULL;
XPWidgetID    UI::inputEOBT = NULL;
XPWidgetID    UI::inputEET = NULL;

XPWidgetID    UI::inputOnline = NULL;
XPWidgetID    UI::inputOnlineID = NULL;
XPWidgetID    UI::inputPax = NULL;
XPWidgetID    UI::inputCargo = NULL;

XPWidgetID    UI::inputCruiseAltitude = NULL;
XPWidgetID    UI::inputRoute = NULL;
XPWidgetID    UI::inputComment = NULL;

XPWidgetID    UI::buttonFlightInfo = NULL;

void UI::initMenus() {
    XPLMMenuID    PluginMenu;
    int           PluginSubMenuItem;
    
    PluginSubMenuItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "openFDR", NULL, 1);
    PluginMenu = XPLMCreateMenu("openFDR menu", XPLMFindPluginsMenu(), PluginSubMenuItem, MenuHandler, NULL);
    XPLMAppendMenuItem(PluginMenu, "Flight", (void *) +1, 1);
}


XPWidgetID UI::CreatePluginWindow(int x, int y, int w, int h)
{
    XPWidgetID window = NULL;
    
    int x2 = x + w;
    int y2 = y - h;
    
    // Create the Main Widget window.
    window = XPCreateWidget(x, y, x2, y2, 1, "openFDR", 1, NULL, xpWidgetClass_MainWindow);
    XPSetWidgetProperty(window, xpProperty_MainWindowHasCloseBoxes, 1);
    
    int xCol1 = x + BORDER_H;
    int xCol2 = xCol1 + 110;
    int xCol3 = xCol2 + 110;
    int xCol4 = xCol3 + 110;
    int xCol5 = xCol4 + 110;

    int yLine1 = y - LINE_SPACE;
    int yLine2 = yLine1 - LINE_SPACE;
    int yLine3 = yLine2 - LINE_SPACE;
    int yLine4 = yLine3 - LINE_SPACE;
    int yLine5 = yLine4 - LINE_SPACE;
    int yLine6 = yLine5 - LINE_SPACE;
    int yLine7 = yLine6 - LINE_SPACE;
    int yLine8 = yLine7 - LINE_SPACE;
    int yLine9 = yLine8 - LINE_SPACE;
    int yLine10 = yLine9 - LINE_SPACE;

    // flight number
    XPCreateWidget(xCol1, yLine1 , xCol1 + 80, yLine1 - LINE_HEIGHT, 1, "Flight Number", 0, window, xpWidgetClass_Caption);
    inputFlightNumber = XPCreateWidget(xCol1, yLine2, xCol1 + 80, yLine2 - LINE_HEIGHT, 1, "ZE999", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputFlightNumber, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputFlightNumber, TextFieldHandler);
    // origin
    XPCreateWidget(xCol2, yLine1 , xCol2 + 80, yLine1 - LINE_HEIGHT, 1, "Origin ICAO", 0, window, xpWidgetClass_Caption);
    inputOriginICAO = XPCreateWidget(xCol2, yLine2, xCol2 + 80, yLine2 - LINE_HEIGHT, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputOriginICAO, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputOriginICAO, TextFieldHandler);
    // dest
    XPCreateWidget(xCol3, yLine1 , xCol3 + 80, yLine1 - LINE_HEIGHT, 1, "Destination ICAO", 0, window, xpWidgetClass_Caption);
    inputDestinationICAO = XPCreateWidget(xCol3, yLine2, xCol3 + 80, yLine2 - LINE_HEIGHT, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputDestinationICAO, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputDestinationICAO, TextFieldHandler);
    // alternate
    XPCreateWidget(xCol4, yLine1 , xCol4 + 80, yLine1 - LINE_HEIGHT, 1, "Alternate ICAO", 0, window, xpWidgetClass_Caption);
    inputAlternateICAO = XPCreateWidget(xCol4, yLine2, xCol4 + 80, yLine2 - LINE_HEIGHT, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputAlternateICAO, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputAlternateICAO, TextFieldHandler);
    // get info button
    XPCreateWidget(xCol5, yLine1 , xCol5 + 80, yLine1 - LINE_HEIGHT, 1, "Flight Data", 0, window, xpWidgetClass_Caption);
    buttonFlightInfo = XPCreateWidget(xCol5, yLine2, xCol5 + 80, yLine2 - LINE_HEIGHT, 1, "Get Data", 0, window, xpWidgetClass_Button);
    //XPAddWidgetCallback(buttonFlightInfo, TextFieldHandler);

    // type
    XPCreateWidget(xCol1, yLine3 , xCol1 + 80, yLine3 - LINE_HEIGHT, 1, "Aircraft Type", 0, window, xpWidgetClass_Caption);
    inputAircraftType = XPCreateWidget(xCol1, yLine4, xCol1 + 80, yLine4 - LINE_HEIGHT, 1, flight->aircraftType.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputAircraftType, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputAircraftType, TextFieldHandler);
    // flight rules
    XPCreateWidget(xCol2, yLine3 , xCol2 + 80, yLine3 - LINE_HEIGHT, 1, "Flight Rules", 0, window, xpWidgetClass_Caption);
    inputFlightRules = XPCreateWidget(xCol2, yLine4, xCol2 + 80, yLine4 - LINE_HEIGHT, 1, flight->rules.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputFlightRules, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputFlightRules, TextFieldHandler);
    // cruise alt
    XPCreateWidget(xCol3, yLine3 , xCol3 + 80, yLine3 - LINE_HEIGHT, 1, "Cruise Altitude", 0, window, xpWidgetClass_Caption);
    inputCruiseAltitude = XPCreateWidget(xCol3, yLine4, xCol3 + 80, yLine4 - LINE_HEIGHT, 1, to_string(flight->cruiseAltitude).c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputCruiseAltitude, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputCruiseAltitude, TextFieldHandler);
    // eobt
    XPCreateWidget(xCol4, yLine3 , xCol4 + 80, yLine3 - LINE_HEIGHT, 1, "Off Block Time", 0, window, xpWidgetClass_Caption);
    inputEOBT = XPCreateWidget(xCol4, yLine4, xCol4 + 80, yLine4 - LINE_HEIGHT, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputEOBT, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputEOBT, TextFieldHandler);
    // eet
    XPCreateWidget(xCol5, yLine3 , xCol5 + 80, yLine3 - LINE_HEIGHT, 1, "Enroute Time", 0, window, xpWidgetClass_Caption);
    inputEET = XPCreateWidget(xCol5, yLine4, xCol5 + 80, yLine4 - LINE_HEIGHT, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputEET, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputEET, TextFieldHandler);

    // route
    XPCreateWidget(xCol1, yLine5 , x2 - BORDER_H, yLine5 - LINE_HEIGHT, 1, "Route", 0, window, xpWidgetClass_Caption);
    inputRoute = XPCreateWidget(xCol1, yLine6, x2 - BORDER_H, yLine6 - LINE_HEIGHT, 1, flight->route.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputRoute, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputRoute, TextFieldHandler);
    // comment
    XPCreateWidget(xCol1, yLine7, x2 - BORDER_H, yLine7 - LINE_HEIGHT, 1, "Comments", 0, window, xpWidgetClass_Caption);
    inputComment = XPCreateWidget(xCol1, yLine8, x2 - BORDER_H, yLine8 - LINE_HEIGHT, 1, flight->comments.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputComment, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputComment, TextFieldHandler);

    // vatsim ivao offline
    XPCreateWidget(xCol1, yLine9, xCol1 + 80, yLine9 - LINE_HEIGHT, 1, "VATSIM/IVAO", 0, window, xpWidgetClass_Caption);
    inputOnline = XPCreateWidget(xCol1, yLine10, xCol1 + 80, yLine10 - LINE_HEIGHT, 1, flight->online.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputOnline, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputOnline, TextFieldHandler);
    // ivao/vatsim accountid
    XPCreateWidget(xCol2, yLine9, xCol2 + 80, yLine9 - LINE_HEIGHT, 1, "VATSIM/IVAO ID", 0, window, xpWidgetClass_Caption);
    inputOnlineID = XPCreateWidget(xCol2, yLine10, xCol2 + 80, yLine10 - LINE_HEIGHT, 1, flight->onlineID.c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputOnlineID, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputOnlineID, TextFieldHandler);
    // pax
    XPCreateWidget(xCol3, yLine9, xCol3 + 80, yLine9 - LINE_HEIGHT, 1, "Pax", 0, window, xpWidgetClass_Caption);
    inputPax = XPCreateWidget(xCol3, yLine10, xCol3 + 80, yLine10 - LINE_HEIGHT, 1, to_string(flight->pax).c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputPax, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputPax, TextFieldHandler);
    // cargo
    XPCreateWidget(xCol4, yLine9, xCol4 + 80, yLine9 - LINE_HEIGHT, 1, "Cargo", 0, window, xpWidgetClass_Caption);
    inputCargo = XPCreateWidget(xCol4, yLine10, xCol4 + 80, yLine10 - LINE_HEIGHT, 1, to_string(flight->cargoKg).c_str(), 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputCargo, xpProperty_TextFieldType, xpTextEntryField);
    XPAddWidgetCallback(inputCargo, TextFieldHandler);

    
    // Register our widget handler
    XPAddWidgetCallback(window, PluginWindowHandler);
    
    return window;
}

int UI::TextFieldHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2) {
    char buffer[1024];
    
    if (inMessage == xpMsg_DescriptorChanged) {
        XPGetWidgetDescriptor(inputFlightNumber, buffer, sizeof(buffer));
        flight->flightNumber = string(buffer);
        
        XPGetWidgetDescriptor(inputOriginICAO, buffer, sizeof(buffer));
        flight->originICAO = string(buffer);

        XPGetWidgetDescriptor(inputDestinationICAO, buffer, sizeof(buffer));
        flight->destinationICAO = string(buffer);
        
        XPGetWidgetDescriptor(inputAlternateICAO, buffer, sizeof(buffer));
        flight->alternateICAO = string(buffer);
        
        XPGetWidgetDescriptor(inputAircraftType, buffer, sizeof(buffer));
        flight->aircraftType = string(buffer).substr(0, 4);

        XPGetWidgetDescriptor(inputFlightRules, buffer, sizeof(buffer));
        flight->rules = string(buffer);

        XPGetWidgetDescriptor(inputCruiseAltitude, buffer, sizeof(buffer));
        try {
            flight->cruiseAltitude = stoi(string(buffer));
        }
        catch(...) {
            XPSetWidgetDescriptor(inputCruiseAltitude, to_string(flight->cruiseAltitude).c_str());
        }
        
        XPGetWidgetDescriptor(inputEOBT, buffer, sizeof(buffer));
        flight->eobt = string(buffer);

        XPGetWidgetDescriptor(inputEET, buffer, sizeof(buffer));
        flight->eet = string(buffer);

        XPGetWidgetDescriptor(inputRoute, buffer, sizeof(buffer));
        flight->route = string(buffer);

        XPGetWidgetDescriptor(inputComment, buffer, sizeof(buffer));
        flight->comments = string(buffer);

        XPGetWidgetDescriptor(inputOnline, buffer, sizeof(buffer));
        flight->online = string(buffer);
        
        XPGetWidgetDescriptor(inputOnlineID, buffer, sizeof(buffer));
        flight->onlineID = string(buffer);
        
        XPGetWidgetDescriptor(inputPax, buffer, sizeof(buffer));
        try {
            flight->pax = stoi(string(buffer));
        }
        catch(...) {
            XPSetWidgetDescriptor(inputPax, to_string(flight->pax).c_str());
        }

        XPGetWidgetDescriptor(inputCargo, buffer, sizeof(buffer));
        try {
            flight->cargoKg = stoi(string(buffer));
        }
        catch(...) {
            XPSetWidgetDescriptor(inputCargo, to_string(flight->cargoKg).c_str());
        }

        return 1;
    }
    return 0;
}

int UI::PluginWindowHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, intptr_t  inParam1, intptr_t inParam2)
{
    if (inMessage == xpMessage_CloseButtonPushed)
    {
        XPHideWidget(UI::PluginWindow);
        return 1;
    }
    
    return 0;
}

void UI::MenuHandler(void * inMenuRef, void * inItemRef)
{
    if (inItemRef == (void *) 1) {
        if (!UI::PluginWindow) {
            UI::PluginWindow = CreatePluginWindow(400, 500, 550, 250);    //left, top, right, bottom.
        }
        else if(!XPIsWidgetVisible(UI::PluginWindow)) {
            XPShowWidget(UI::PluginWindow);
        }
    }
}



