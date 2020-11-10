#include "TCPServer.h"
#include "FDR.h"
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"

const int LOOP_INTERVAL_SECONDS = 1;

int last_run = 0;
FDR *fdr = NULL;
TCPServer *server = NULL;


float FDRLoopCB(float elapsedMe, float elapsedSim, int counter, void *refCon)
{
	DataPoint* data;
    data = fdr->update(elapsedMe, elapsedSim, counter);
	if (data) {
		server->transmit(data->toJSON());
	}
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

    fdr = new FDR();

	server = new TCPServer();
	server->start();

    XPLMDebugString("openFDR: registering callback\n");
    XPLMRegisterFlightLoopCallback(FDRLoopCB, LOOP_INTERVAL_SECONDS, NULL);
    
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
    XPLMDebugString("openFDR: stopping\n");
    XPLMUnregisterFlightLoopCallback(FDRLoopCB, NULL);
    delete fdr;
	server->stop();
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
