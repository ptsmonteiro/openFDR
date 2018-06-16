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

XPWidgetID    PluginWindow = NULL;
XPWidgetID    inputFlightNumber = NULL;
XPWidgetID    inputOriginICAO = NULL;
XPWidgetID    inputDestinationICAO = NULL;
XPWidgetID    inputAlternateICAO = NULL;
XPWidgetID    inputCruiseAltitude = NULL;
XPWidgetID    inputRoute = NULL;

// This will create our widget dialog.
XPWidgetID CreatePluginWindow(int x, int y, int w, int h)
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
    XPSetWidgetProperty(inputFlightNumber, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 70, yy, 1, "Origin ICAO", 0, window, xpWidgetClass_Caption);
    xx += 70 + 10;
    inputOriginICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(inputOriginICAO, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 90, yy, 1, "Destination ICAO", 0, window, xpWidgetClass_Caption);
    xx += 90 + 10;
    inputDestinationICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(inputDestinationICAO, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 80, yy, 1, "Alternate ICAO", 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    inputAlternateICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(inputAlternateICAO, xpProperty_TextFieldType, xpTextEntryField);

    xx = x + BORDER_H;
    yy -= LINE_HEIGHT;

    XPCreateWidget(xx, yy, xx + 80, yy - LINE_HEIGHT, 1, "Cruise Altitude", 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    inputCruiseAltitude = XPCreateWidget(xx, yy - 2, xx + 60, yy - LINE_HEIGHT - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(inputCruiseAltitude, xpProperty_TextFieldType, xpTextEntryField);
    xx += 60 + 20;
    
    XPCreateWidget(xx, yy, xx + 40, yy - LINE_HEIGHT, 1, "Route", 0, window, xpWidgetClass_Caption);
    xx += 40 + 10;
    inputRoute = XPCreateWidget(xx, yy - 2, x2 - BORDER_H, yy - LINE_HEIGHT - 2, 1, "", 0, window, xpWidgetClass_TextField);
    XPSetWidgetProperty(inputRoute, xpProperty_TextFieldType, xpTextEntryField);
    
    // Register our widget handler
    XPAddWidgetCallback(window, PluginWindowHandler);
    
    return window;
}

// This is our widget handler.  In this example we are only interested when the close box is pressed.
int PluginWindowHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, long  inParam1, long  inParam2)
{
    if (inMessage == xpMessage_CloseButtonPushed)
    {
        XPHideWidget(PluginWindow);
        return 1;
    }
    
    return 0;
}

void MenuHandler(void * inMenuRef, void * inItemRef)
{
    if (inItemRef == (void *) 1) {
        if (!PluginWindow) {
            PluginWindow = CreatePluginWindow(400, 500, 700, 400);    //left, top, right, bottom.
        }
        else if(!XPIsWidgetVisible(PluginWindow)) {
            XPShowWidget(PluginWindow);
        }
    }
}



