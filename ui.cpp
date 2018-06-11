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
XPWidgetID    widgetFlightNumber = NULL;
XPWidgetID    widgetOriginICAO = NULL;
XPWidgetID    widgetDestinationICAO = NULL;
XPWidgetID    widgetAlternateICAO = NULL;
XPWidgetID    widgetCruiseAltitude = NULL;
XPWidgetID    widgetRoute = NULL;

char textAbout[] = "openFDR v1.0 by Pedro Monteiro, 2018.";

char captionFltNumber[] = "Flight Number";
char inputFltNumber[255] = "";

char captionOrigin[] = "Origin ICAO";
char inputOrigin[255] = "";

char captionDest[] = "Destination ICAO";
char inputDest[255] = "";

char captionAlt[] = "Alternate ICAO";
char inputAlt[255] = "";

char captionAltitude[] = "Cruise Altitude";
char inputAltitude[255] = "";

char captionRoute[] = "Route";
char inputRoute[255] = "";

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

    XPCreateWidget(xx, y - BORDER_V , xx + 80, yy, 1, captionFltNumber, 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    widgetFlightNumber = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, inputFltNumber, 0, window, xpWidgetClass_TextField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 70, yy, 1, captionOrigin, 0, window, xpWidgetClass_Caption);
    xx += 70 + 10;
    widgetOriginICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, inputOrigin, 0, window, xpWidgetClass_TextField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 90, yy, 1, captionDest, 0, window, xpWidgetClass_Caption);
    xx += 90 + 10;
    widgetDestinationICAO = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, inputDest, 0, window, xpWidgetClass_TextField);
    xx += 60 + 20;

    XPCreateWidget(xx, y - BORDER_V , xx + 80, yy, 1, captionAlt, 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    widgetCruiseAltitude = XPCreateWidget(xx, y - BORDER_V - 2, xx + 60, yy - 2, 1, inputAlt, 0, window, xpWidgetClass_TextField);

    xx = x + BORDER_H;
    yy -= LINE_HEIGHT;

    XPCreateWidget(xx, yy, xx + 80, yy - LINE_HEIGHT, 1, captionAltitude, 0, window, xpWidgetClass_Caption);
    xx += 80 + 10;
    widgetCruiseAltitude = XPCreateWidget(xx, yy - 2, xx + 60, yy - LINE_HEIGHT - 2, 1, inputAltitude, 0, window, xpWidgetClass_TextField);
    xx += 60 + 20;
    
    XPCreateWidget(xx, yy, xx + 40, yy - LINE_HEIGHT, 1, captionRoute, 0, window, xpWidgetClass_Caption);
    xx += 40 + 10;
    widgetRoute = XPCreateWidget(xx, yy - 2, x2 - BORDER_H, yy - LINE_HEIGHT - 2, 1, inputRoute, 0, window, xpWidgetClass_TextField);

    
    // Create a text field
    //XPCreateWidget(x + BORDER_H, y - BORDER_V , x + BORDER_H + 60, yy,
    //               1, captionFltNumber, 0, window, xpWidgetClass_Caption);
    //XPCreateWidget(x + BORDER_H, yy, x2 - BORDER_H, yy - LINE_SPACE,
    //               1, captionRoute, 0, window, xpWidgetClass_TextField);
    
    
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



