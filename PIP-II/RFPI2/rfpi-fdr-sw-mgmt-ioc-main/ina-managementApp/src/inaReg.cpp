#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>

#include <epicsTypes.h>
#include <epicsExport.h>
#include <epicsExit.h>
#include <iocsh.h>

#include "inaAsynPortDriver.h"


extern "C" {

inaAsynPortDriver *drv = NULL;

void exitFunc(void*)
{
    delete drv;
}

int inaAsynPortDriverConfigure(const char *portName, const char *configPath)
{
    if ( drv != NULL ) return asynSuccess;
    epicsAtExit(exitFunc, 0);

    drv = new inaAsynPortDriver(portName, configPath);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "configPath",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0, &initArg1};
static const iocshFuncDef initFuncDef = {"inaAsynPortDriverConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    inaAsynPortDriverConfigure(args[0].sval, args[1].sval);
}

void inaAsynPortDriverRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(inaAsynPortDriverRegister);

}
