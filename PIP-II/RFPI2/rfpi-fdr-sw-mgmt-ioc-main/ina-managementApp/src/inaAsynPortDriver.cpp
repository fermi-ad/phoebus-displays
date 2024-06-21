#include <epicsEvent.h>

#include "inaAsynPortDriver.h"
#include <string.h>


void paramsReadout(void *drvPvt);

static const char *driverName = "inaAsynPortDriver";


inaAsynPortDriver::inaAsynPortDriver(const char *portName, const char *configPath)
    : asynPortDriver(portName,
                     1, /* maxAddr */
                     asynInt32Mask | asynFloat64Mask | asynUInt32DigitalMask | asynOctetMask | asynDrvUserMask, /* Interface mask */
                     asynInt32Mask | asynFloat64Mask | asynUInt32DigitalMask | asynOctetMask,   /* Interrupt mask */
                     0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                     1, /* Autoconnect */
                     0, /* Default priority */
                     0) /* Default stack size*/
{
    asynStatus status = asynSuccess;

    eventId_ = epicsEventCreate(epicsEventEmpty);
    
    terminateWorkers    = 0;
    ackFromWorkersCount = 0;

    terminateWorkersLock = epicsMutexMustCreate();

    pthread_mutex_init (&ackFromWorkersLock, NULL);
    pthread_cond_init  (&ackFromWorkersCond, NULL);

    CREATE_SLOT(TOP0);
    CREATE_SLOT(TOP1);
    CREATE_SLOT(TOP2);
    CREATE_SLOT(BOT0);
    CREATE_SLOT(BOT1);
    CREATE_SLOT(BOT2);
    CREATE_SLOT(INTER);
    CREATE_SLOT(MAIN);

    CREATE_FAN(1);
    CREATE_FAN(2);
    CREATE_FAN(3);

    createParam(P_VERSION_R_String,     	asynParamOctet,   &P_VERSION_R);
    createParam(P_CS0_OPEN_R_String,     	asynParamInt32,   &P_CS0_OPEN_R);
    createParam(P_CS0_SHORT_R_String,    	asynParamInt32,   &P_CS0_SHORT_R);
    createParam(P_CS0_FUSE_OK_R_String,  	asynParamInt32,   &P_CS0_FUSE_OK_R);
    createParam(P_CS1_OPEN_R_String,     	asynParamInt32,   &P_CS1_OPEN_R);
    createParam(P_CS1_SHORT_R_String,    	asynParamInt32,   &P_CS1_SHORT_R);
    createParam(P_CS1_FUSE_OK_R_String,  	asynParamInt32,   &P_CS1_FUSE_OK_R);
    createParam(P_SSA_OPEN_R_String,    	asynParamInt32,   &P_SSA_SHORT_R);
    createParam(P_SSA_SHORT_R_String,   	asynParamInt32,   &P_SSA_SHORT_R);
    createParam(P_SSA_FUSE_OK_R_String, 	asynParamInt32,   &P_SSA_FUSE_OK_R);
    //createParam(P_STARTUP_S_String, 		asynParamInt32,   &P_STARTUP_S);
    createParam(P_POWER_SEQ_R_String, 		asynParamInt32,   &P_POWER_SEQ_R);
    createParam(P_POWER_S_String, 		asynParamInt32,   &P_POWER_S);
    createParam(P_VADJ_S_String, 		asynParamInt32,   &P_VADJ_S);
    createParam(P_VADJ_R_String, 		asynParamInt32,   &P_VADJ_R);
    createParam(P_TC_S_String, 	 		asynParamInt32,   &P_TC_S);

    i2c.SetConfig({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    gpio.SetConfig({32, 26, 32, 22});
    
    AddressI2C::SetMaster(&i2c);
    AddressGPIO::SetMaster(&gpio);
    
    top0.SetType(SlotEnum::top0);
    top1.SetType(SlotEnum::top1);
    top2.SetType(SlotEnum::top2);
    bot0.SetType(SlotEnum::bot0);
    bot1.SetType(SlotEnum::bot1);
    bot2.SetType(SlotEnum::bot2);
    inter.SetType(SlotEnum::inter);
    main.SetType(SlotEnum::main);

    master.AddSlot(top0);
    master.AddSlot(top1);
    master.AddSlot(top2);
    master.AddSlot(bot0);
    master.AddSlot(bot1);
    master.AddSlot(bot2);
    master.AddSlot(inter);
    master.AddSlot(main);

    master.AddParser(BoardCS::Parse);
    master.AddParser(BoardSSA::Parse);
    master.AddParser(BoardFEP::Parse);
    master.AddParser(BoardInter::Parse);
    master.AddParser(BoardRTD::Parse);

    master.SetFanCount(FANS_NUM);
    master.SetConfig(configPath);

    uint major, minor, build;
    master.GetVersion(&major, &minor, &build);
    char buf[30];
    sprintf(buf, "Lib version: %d.%d.%d", major, minor, build);
    setStringParam(P_VERSION_R, buf);

    lock();
    setIntegerParam(P_POWER_SEQ_R, 0);
    setStringParam(P_TOP0_CURRENT_BOARD_NAME_R, top0.GetBoard()->GetName());
    setStringParam(P_TOP1_CURRENT_BOARD_NAME_R, top1.GetBoard()->GetName());
    setStringParam(P_BOT0_CURRENT_BOARD_NAME_R, bot0.GetBoard()->GetName());
    setStringParam(P_TOP2_CURRENT_BOARD_NAME_R, top2.GetBoard()->GetName());
    setStringParam(P_BOT1_CURRENT_BOARD_NAME_R, bot1.GetBoard()->GetName());
    setStringParam(P_BOT2_CURRENT_BOARD_NAME_R, bot2.GetBoard()->GetName());
    setStringParam(P_INTER_CURRENT_BOARD_NAME_R, inter.GetBoard()->GetName());
    setStringParam(P_MAIN_CURRENT_BOARD_NAME_R, main.GetBoard()->GetName());
    unlock();

    callParamCallbacks();

    map<string, void*> data;
    data["driver"] = static_cast<void*>(this);
    master.MonitorHealth(PowerMonitorCallback, data);

    status = (asynStatus)(epicsThreadCreate("paramsReadout",
                                            epicsThreadPriorityMedium,
                                            epicsThreadGetStackSize(epicsThreadStackMedium),
                                            (EPICSTHREADFUNC)::paramsReadout,
                                            this) == NULL);

    if (status) {
        printf("%s:paramsReadout: epicsThreadCreate failure\n", driverName);
        return;
    }
}

asynStatus inaAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value) {
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;

    status = (asynStatus) setIntegerParam(function, value);

    if (function == P_VADJ_S)
    {
       main.SetVadj(value);
    }

    if (function == P_TC_S)
    {
        master.DisableTC();
    }

    if (function == P_POWER_S)
    {
        string msg;
        // on all boards
        if (value == 1)
        {
            if (master.PerformStartupSequence(&msg))
	    {
                lock();
                setIntegerParam(P_POWER_SEQ_R, 2);
                unlock();
                asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, msg.c_str(), NULL); 
            }
            lock();
            setIntegerParam(P_POWER_SEQ_R, 0);
            unlock();
        }
        // off all boards
        else
        {
            if (master.PerformShutdownSequence(&msg))
	    {
                lock();
                setIntegerParam(P_POWER_SEQ_R, 2);
                unlock();
                asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, msg.c_str(), NULL); 
            }
            lock();
            setIntegerParam(P_POWER_SEQ_R, 0);
            unlock();
        }
    }
    
    if (function == P_BOT0_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_BOT0_INA_EVENT_R, 0);
        setIntegerParam(P_BOT0_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_BOT1_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_BOT1_INA_EVENT_R, 0);
        setIntegerParam(P_BOT1_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_BOT2_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_BOT2_INA_EVENT_R, 0);
        setIntegerParam(P_BOT2_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_TOP0_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_TOP0_INA_EVENT_R, 0);
        setIntegerParam(P_TOP0_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_TOP1_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_TOP1_INA_EVENT_R, 0);
        setIntegerParam(P_TOP1_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_TOP2_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_TOP2_INA_EVENT_R, 0);
        setIntegerParam(P_TOP2_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_INTER_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_INTER_INA_EVENT_R, 0);
        setIntegerParam(P_INTER_POWER_EVENT_R, 0);
        unlock();
    }

    if (function == P_MAIN_ACK_EVENT_S)
    {
        lock();
        setIntegerParam(P_MAIN_INA_EVENT_R, 0);
        setIntegerParam(P_MAIN_POWER_EVENT_R, 0);
        unlock();
    }

    callParamCallbacks();
    return status;
}

asynStatus inaAsynPortDriver::writeFloat64(asynUser *pasynUser, epicsFloat64 value) {
    int function = pasynUser->reason;

    asynStatus status = asynSuccess;

    status = (asynStatus) setDoubleParam(function, value);

    // check fans
    for(int i = 0; i < FANS_NUM; i++)
    {
        if (function == fans[1][i])
        {
            // set fan speed
            master.SetFanSpeed(i, value);
            break;
	}
    }

    for(int i = 0; i < CUR_CHANS; i++)
    {
        if (function == curTOP0Warns[i])
        {
	    top0.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curTOP0Crits[i])
        {
	    top0.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curTOP1Warns[i])
        {
	    top1.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curTOP1Crits[i])
        {
	    top1.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curTOP2Warns[i])
        {
	    top2.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curTOP2Crits[i])
        {
	    top2.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curBOT0Warns[i])
        {
	    bot0.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curBOT0Crits[i])
        {
	    bot0.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curBOT1Warns[i])
        {
	    bot1.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curBOT1Crits[i])
        {
	    bot1.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curBOT2Warns[i])
        {
	    bot2.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curBOT2Crits[i])
        {
	    bot2.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curINTERWarns[i])
        {
	    inter.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curINTERCrits[i])
        {
	    inter.SetMonitoringCritical(i, value);
            break;
	}

        if (function == curMAINWarns[i])
        {
	    main.SetMonitoringWarning(i, value);
            break;
	}

        if (function == curMAINCrits[i])
        {
	    main.SetMonitoringCritical(i, value);
            break;
	}
    }

    if (function == P_TOP0_VOLTAGE_LOW_S)
    {
        top0.SetVoltageLow(value);
    }

    if (function == P_TOP0_VOLTAGE_HIGH_S)
    {
        top0.SetVoltageHigh(value);
    }

    if (function == P_TOP1_VOLTAGE_LOW_S)
    {
        top1.SetVoltageLow(value);
    }

    if (function == P_TOP1_VOLTAGE_HIGH_S)
    {
        top1.SetVoltageHigh(value);
    }

    if (function == P_TOP2_VOLTAGE_LOW_S)
    {
        top2.SetVoltageLow(value);
    }

    if (function == P_TOP2_VOLTAGE_HIGH_S)
    {
        top2.SetVoltageHigh(value);
    }

    if (function == P_BOT0_VOLTAGE_LOW_S)
    {
        bot0.SetVoltageLow(value);
    }

    if (function == P_BOT0_VOLTAGE_HIGH_S)
    {
        bot0.SetVoltageHigh(value);
    }

    if (function == P_BOT1_VOLTAGE_LOW_S)
    {
        bot1.SetVoltageLow(value);
    }

    if (function == P_BOT1_VOLTAGE_HIGH_S)
    {
        bot1.SetVoltageHigh(value);
    }

    if (function == P_BOT2_VOLTAGE_LOW_S)
    {
        bot2.SetVoltageLow(value);
    }

    if (function == P_BOT2_VOLTAGE_HIGH_S)
    {
        bot2.SetVoltageHigh(value);
    }

    if (function == P_INTER_VOLTAGE_LOW_S)
    {
        inter.SetVoltageLow(value);
    }

    if (function == P_INTER_VOLTAGE_HIGH_S)
    {
        inter.SetVoltageHigh(value);
    }

    if (function == P_MAIN_VOLTAGE_LOW_S)
    {
        main.SetVoltageLow(value);
    }

    if (function == P_MAIN_VOLTAGE_HIGH_S)
    {
        main.SetVoltageHigh(value);
    }

    callParamCallbacks();
    return status;
}

int inaAsynPortDriver::isCurrentOK(pair<uint8_t, uint8_t> res) {
    auto alerts = res;
    if (alerts.first == 0 && alerts.second == 0) 
        return 0;
    else
        return 1;
}

void inaAsynPortDriver::terminateThreads() {
    epicsMutexMustLock(terminateWorkersLock);
    terminateWorkers = 1 ;
    epicsMutexUnlock(terminateWorkersLock);
    pthread_mutex_lock(&ackFromWorkersLock);

    do {
        if (ackFromWorkersCount == 1) {
            break;
        } else {
            pthread_cond_wait(&ackFromWorkersCond, &ackFromWorkersLock);
        }
    } while (1) ;

    pthread_mutex_unlock(&ackFromWorkersLock);
}

inaAsynPortDriver::~inaAsynPortDriver() {
}

void inaAsynPortDriver::PowerMonitorCallback(void* eventData, map<string, void*> usrData)
{
    HealthAlert* alert = (HealthAlert*)usrData.at("internal");
    std::string slot = Slot::ParseEnum(alert->slot);
    std::string type = Slot::ParseEnum(alert->event); 
    cout << "Power fault: source-> " << slot << " type-> " << type << endl;
    inaAsynPortDriver* driver = (inaAsynPortDriver*)usrData.at("driver");

    if (alert->slot == SlotEnum::top0)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP0_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP0_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::top1)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP1_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP1_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::top2)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP2_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_TOP2_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::bot0)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT0_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT0_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::bot1)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT1_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT1_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::bot2)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT2_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_BOT2_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::inter)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_INTER_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_INTER_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    else if (alert->slot == SlotEnum::main)
    {
       if (alert->event == HealthEvent::inaEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_MAIN_INA_EVENT_R, 1);
           driver->unlock();
       }
       else if (alert->event == HealthEvent::pwrGdEvent)
       {
           driver->lock();
           driver->setIntegerParam(driver->P_MAIN_POWER_EVENT_R, 1);
           driver->unlock();
       } 
    }
    driver->callParamCallbacks();
}

