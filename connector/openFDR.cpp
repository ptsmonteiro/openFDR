#include "TCPServer.h"
#include "DataPoint.h"
#include "utilities.h"
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"

const int LOOP_INTERVAL_SECONDS = 1;

int last_run = 0;
TCPServer *server = NULL;


float FDRLoopCB(float elapsedMe, float elapsedSim, int counter, void *refCon)
{
	DataPoint* dp;

	if (readDataI("sim/time/paused") || elapsedSim < 1 || readDataI("sim/operation/prefs/replay_mode")) {
		return LOOP_INTERVAL_SECONDS;
	}

	dp = new DataPoint(elapsedSim);
	server->transmit(dp->toJSON());
	delete dp;
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
