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
XPWidgetID    UI::inputCruiseAltitude = NULL;
XPWidgetID    UI::inputRoute = NULL;


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
    
    int xx = x + BORDER_H;
    int yy = y - (BORDER_V + LINE_HEIGHT);

    XPCreateWidget(xx, y - BORDER_V , xx + 80, yy, 1, "Flight Number", 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    inputFlightNumber = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputFlightNumber, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 70, yy, 1, "Origin ICAO", 0, window, xpWidgetClass_Caption);
    xx += 70 + 10;
    inputOriginICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputOriginICAO, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 90, yy, 1, "Destination ICAO", 0, window, xpWidgetClass_Caption);
    xx += 90 + 10;
    inputDestinationICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputDestinationICAO, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 80, yy, 1, "Alternate ICAO", 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    inputAlternateICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputAlternateICAO, xpProperty_TextFieldType, xpTextEntryField);

    xx = x + BORDER_H;
    yy -= LINE_HEIGHT;

    XPCreateWidget(xx, yy, xx + 80, yy - LINE_HEIGHT, 1, "Cruise Altitude", 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    inputCruiseAltitude = XPCreateWidget(xx, yy - 2, xx + 60, yy - LINE_HEIGHT - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(UI::inputCruiseAltitude, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;
    
    XPCreateWidget(xx, yy, xx + 40, yy - LINE_HEIGHT, 1, "Route", 0, window, xpWidgetClass_Caption);
    xx += 40 + 10;
    inputRoute = XPCreateWidget(xx, yy - 2, x2 - BORDER_H, yy - LINE_HEIGHT - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPAddWidgetCallback(inputRoute, TextFieldHandler);
    XPSetWidgetProperty(UI::inputRoute, xpProperty_TextFieldType, xpTextEntryField);
    
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
        flight->origionICAO = string(buffer);

        XPGetWidgetDescriptor(inputDestinationICAO, buffer, sizeof(buffer));
        flight->destinationICAO = string(buffer);
        
        XPGetWidgetDescriptor(inputAlternateICAO, buffer, sizeof(buffer));
        flight->alternateICAO = string(buffer);
        
        XPGetWidgetDescriptor(inputCruiseAltitude, buffer, sizeof(buffer));
        try {
            flight->cruiseAltitude = stoi(string(buffer));
        }
        catch(...) {
            XPSetWidgetDescriptor(inputCruiseAltitude, to_string(flight->cruiseAltitude).c_str());
        }
        
        XPGetWidgetDescriptor(inputRoute, buffer, sizeof(buffer));
        flight->route = string(buffer);
        
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
            UI::PluginWindow = CreatePluginWindow(400, 500, 700, 400);    //left, top, right, bottom.
        }
        else if(!XPIsWidgetVisible(UI::PluginWindow)) {
            XPShowWidget(UI::PluginWindow);
        }
    }
}



