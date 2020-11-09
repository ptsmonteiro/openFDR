//
//  widgets.h
//  openFDR
//
//  Created by Pedro on 01/06/2018.
//

#include "XPWidgets.h"
#include "XPLMMenus.h"
#include "FDR.h"

const int BORDER_H = 10;
const int BORDER_V = 30;
const int LINE_SPACE   = 20;
const int LINE_HEIGHT  = 12;

class UI {
    
public:
    
    static Flight *flight;
    
    static XPWidgetID PluginWindow;
    static XPWidgetID inputFlightNumber;
    static XPWidgetID inputOriginICAO;
    static XPWidgetID inputDestinationICAO;
    static XPWidgetID inputAlternateICAO;
    static XPWidgetID inputCruiseAltitude;
    static XPWidgetID inputRoute;
    
    UI();
    ~UI();

    static void initMenus();
    static XPWidgetID CreatePluginWindow(int, int, int, int);
    static int PluginWindowHandler(XPWidgetMessage, XPWidgetID, intptr_t, intptr_t);
    static int TextFieldHandler(XPWidgetMessage, XPWidgetID, intptr_t, intptr_t);
    static void MenuHandler(void *, void *);
};
