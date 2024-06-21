#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <unistd.h>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>
#include <errlog.h>

#include "kriaAsynPortDriver.h"
#include <epicsExport.h>
#include <epicsExit.h>


extern "C" {

kriaAsynPortDriver *drv = NULL;

void exitFunc(void*)
{
    drv->terminateThreads();
    delete drv;
}

int kriaAsynPortDriverConfigure(const char *portName)
{
    if ( drv != NULL ) return asynSuccess;
    epicsAtExit(exitFunc, 0);

    drv = new kriaAsynPortDriver(portName);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0};
static const iocshFuncDef initFuncDef = {"kriaAsynPortDriverConfigure",1,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    kriaAsynPortDriverConfigure(args[0].sval);
}

void kriaAsynPortDriverRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(kriaAsynPortDriverRegister);

}
