#include <stdio.h>
#include <string.h>
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "XPLMMenus.h"
#include "FDR.h"

const int LOOP_INTERVAL_SECONDS = 1;

int last_run = 0;
FDR *fdr;

int gMenuItem;

XPWidgetID    InstructionsWidget = NULL;
XPWidgetID    InstructionsWindow = NULL;

char textAbout[] = "openFDR v1.0 by Pedro Monteiro, 2018.";

void InstructionsMenuHandler(void *, void *);
void CreateInstructionsWidget(int x1, int y1, int w, int h);
int InstructionsHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, long  inParam1, long  inParam2);

// This will create our widget dialog.
void CreateInstructionsWidget(int x, int y, int w, int h)
{
    int x2 = x + w;
    int y2 = y - h;
    
    // Create the Main Widget window.
    InstructionsWidget = XPCreateWidget(x, y, x2, y2, 1, "openFDR - About", 1, NULL, xpWidgetClass_MainWindow);
    XPSetWidgetProperty(InstructionsWidget, xpProperty_MainWindowHasCloseBoxes, 1);
    
    // Create a text widget
    XPCreateWidget(x+10, y-30 , x2-10, y-42, 1, textAbout, 0, InstructionsWidget, xpWidgetClass_Caption);
    
    // Register our widget handler
    XPAddWidgetCallback(InstructionsWidget, InstructionsHandler);
}

void InstructionsMenuHandler(void * inMenuRef, void * inItemRef)
{
    if (inItemRef == (void *) 1) {
        if (gMenuItem == 0)
        {
            CreateInstructionsWidget(400, 500, 700, 400);    //left, top, right, bottom.
            gMenuItem = 1;
        }
        else
        {
            if(!XPIsWidgetVisible(InstructionsWidget))
                XPShowWidget(InstructionsWidget);
        }
    }
}

// This is our widget handler.  In this example we are only interested when the close box is pressed.
int InstructionsHandler(XPWidgetMessage  inMessage, XPWidgetID  inWidget, long  inParam1, long  inParam2)
{
    if (inMessage == xpMessage_CloseButtonPushed)
    {
        if (gMenuItem == 1)
        {
            XPHideWidget(InstructionsWidget);
        }
        return 1;
    }
    
    return 0;
}

void initMenus() {
    XPLMMenuID    PluginMenu;
    int           PluginSubMenuItem;
    
    // Create our menu
    PluginSubMenuItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "openFDR", NULL, 1);
    PluginMenu = XPLMCreateMenu("openFDR menu", XPLMFindPluginsMenu(),
                                PluginSubMenuItem, InstructionsMenuHandler, NULL);
    XPLMAppendMenuItem(PluginMenu, "About", (void *) +1, 1);
    
    // Flag to tell us if the widget is being displayed.
    gMenuItem = 0;
}


float FDRLoopCB(float elapsedMe, float elapsedSim, int counter, void *refCon)
{
    fdr->update(elapsedMe, elapsedSim, counter);
    return LOOP_INTERVAL_SECONDS;
}

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
    XPLMDebugString("openFDR: starting\n");
	strcpy(outName, "openFDR");
	strcpy(outSig, "zonexecutive.openfdr");
	strcpy(outDesc, "Flight Data Recorder plugin");

    initMenus();
    
    fdr = new FDR();

    XPLMDebugString("openFDR: registering callback\n");

    XPLMRegisterFlightLoopCallback(FDRLoopCB, LOOP_INTERVAL_SECONDS, NULL);
    
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
    XPLMDebugString("openFDR: stopping\n");
    XPLMUnregisterFlightLoopCallback(FDRLoopCB, NULL);
    delete fdr;
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
}
