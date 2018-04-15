#include <stdio.h>
#include <string.h>
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "FDR.h"

const int LOOP_INTERVAL_SECONDS = 1;

int last_run = 0;
FDR *fdr;

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
	strcpy(outName, "openFDR");
	strcpy(outSig, "zonexecutive.openfdr");
	strcpy(outDesc, "Flight Data Recorder plugin");

    fdr = new FDR();
    XPLMRegisterFlightLoopCallback(FDRLoopCB, LOOP_INTERVAL_SECONDS, NULL);
    
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
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

