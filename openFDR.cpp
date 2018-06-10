#include <stdio.h>
#include <string.h>
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "XPLMMenus.h"
#include "ui.h"
#include "FDR.h"

const int LOOP_INTERVAL_SECONDS = 1;

int last_run = 0;
FDR *fdr;

void initMenus() {
    XPLMMenuID    PluginMenu;
    int           PluginSubMenuItem;
    
    // Create our menu
    PluginSubMenuItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "openFDR", NULL, 1);
    PluginMenu = XPLMCreateMenu("openFDR menu", XPLMFindPluginsMenu(),
                                PluginSubMenuItem, MenuHandler, NULL);
    XPLMAppendMenuItem(PluginMenu, "Data", (void *) +1, 1);
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
