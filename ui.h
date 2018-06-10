//
//  widgets.h
//  openFDR
//
//  Created by Pedro on 01/06/2018.
//

#include "XPWidgets.h"

const int BORDER_H = 10;
const int BORDER_V = 30;
const int LINE_SPACE   = 20;
const int LINE_HEIGHT  = 12;

XPWidgetID CreatePluginWindow(int, int, int, int);
int PluginWindowHandler(XPWidgetMessage, XPWidgetID, long, long);
void MenuHandler(void *, void *);
