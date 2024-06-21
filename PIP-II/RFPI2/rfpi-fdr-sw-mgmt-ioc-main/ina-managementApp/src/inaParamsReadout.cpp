#include "inaAsynPortDriver.h"

#define SLEEP_TIME 1

void paramsReadout(void *drvPvt) {
    inaAsynPortDriver *pPvt = (inaAsynPortDriver *)drvPvt;
    pPvt->paramsReadout();
}


void inaAsynPortDriver::paramsReadout(void) {
    epicsMutexMustLock(terminateWorkersLock);

    while(terminateWorkers == 0)
    {
        epicsMutexUnlock(terminateWorkersLock);
        epicsThreadSleep(SLEEP_TIME);

        lock();
	/*for (int i = 0; i < FANS_NUM; i++)
        {
            setIntegerParam(fans[0][i], master.GetFanSpeed(i));
            setIntegerParam(fans[2][i], master.GetFanStatus(i));
        }*/
        // read temperatures
        //setDoubleParam(P_TOP0_TEMPERATURE_R, epicsFloat64(top0.GetTemperature()));
        //setDoubleParam(P_TOP1_TEMPERATURE_R, epicsFloat64(top1.GetTemperature()));
        //setDoubleParam(P_TOP2_TEMPERATURE_R, epicsFloat64(top2.GetTemperature()));
        //setDoubleParam(P_BOT0_TEMPERATURE_R, epicsFloat64(bot0.GetTemperature()));
        //setDoubleParam(P_BOT1_TEMPERATURE_R, epicsFloat64(bot1.GetTemperature()));
        //setDoubleParam(P_BOT2_TEMPERATURE_R, epicsFloat64(bot2.GetTemperature()));
        //setDoubleParam(P_MAIN_TEMPERATURE_R, epicsFloat64(main.GetTemperature()));

        // read current
        setDoubleParam(P_TOP0_CURRENT_CH1_R, epicsFloat64(top0.GetCurrent(0)));
        setDoubleParam(P_TOP0_CURRENT_CH2_R, epicsFloat64(top0.GetCurrent(1)));
        setDoubleParam(P_TOP0_CURRENT_CH3_R, epicsFloat64(top0.GetCurrent(2)));

        setDoubleParam(P_TOP1_CURRENT_CH1_R, epicsFloat64(top1.GetCurrent(0)));
        setDoubleParam(P_TOP1_CURRENT_CH2_R, epicsFloat64(top1.GetCurrent(1)));
        setDoubleParam(P_TOP1_CURRENT_CH3_R, epicsFloat64(top1.GetCurrent(2)));

        setDoubleParam(P_TOP2_CURRENT_CH1_R, epicsFloat64(top2.GetCurrent(0)));
        setDoubleParam(P_TOP2_CURRENT_CH2_R, epicsFloat64(top2.GetCurrent(1)));
        setDoubleParam(P_TOP2_CURRENT_CH3_R, epicsFloat64(top2.GetCurrent(2)));

        setDoubleParam(P_BOT0_CURRENT_CH1_R, epicsFloat64(bot0.GetCurrent(0)));
        setDoubleParam(P_BOT0_CURRENT_CH2_R, epicsFloat64(bot0.GetCurrent(1)));
        setDoubleParam(P_BOT0_CURRENT_CH3_R, epicsFloat64(bot0.GetCurrent(2)));

        setDoubleParam(P_BOT1_CURRENT_CH1_R, epicsFloat64(bot1.GetCurrent(0)));
        setDoubleParam(P_BOT1_CURRENT_CH2_R, epicsFloat64(bot1.GetCurrent(1)));
        setDoubleParam(P_BOT1_CURRENT_CH3_R, epicsFloat64(bot1.GetCurrent(2)));

        setDoubleParam(P_BOT2_CURRENT_CH1_R, epicsFloat64(bot2.GetCurrent(0)));
        setDoubleParam(P_BOT2_CURRENT_CH2_R, epicsFloat64(bot2.GetCurrent(1)));
        setDoubleParam(P_BOT2_CURRENT_CH3_R, epicsFloat64(bot2.GetCurrent(2)));

        setDoubleParam(P_INTER_CURRENT_CH1_R, epicsFloat64(inter.GetCurrent(0)));
        setDoubleParam(P_INTER_CURRENT_CH2_R, epicsFloat64(inter.GetCurrent(1)));
        setDoubleParam(P_INTER_CURRENT_CH3_R, epicsFloat64(inter.GetCurrent(2)));

        setDoubleParam(P_MAIN_CURRENT_CH1_R, epicsFloat64(main.GetCurrent(0)));
        setDoubleParam(P_MAIN_CURRENT_CH2_R, epicsFloat64(main.GetCurrent(1)));
        setDoubleParam(P_MAIN_CURRENT_CH3_R, epicsFloat64(main.GetCurrent(2)));
        
        // read voltage
        setDoubleParam(P_TOP0_VOLTAGE_CH1_R, epicsFloat64(top0.GetVoltage(0)));
        setDoubleParam(P_TOP0_VOLTAGE_CH2_R, epicsFloat64(top0.GetVoltage(1)));
        setDoubleParam(P_TOP0_VOLTAGE_CH3_R, epicsFloat64(top0.GetVoltage(2)));

        setDoubleParam(P_TOP1_VOLTAGE_CH1_R, epicsFloat64(top1.GetVoltage(0)));
        setDoubleParam(P_TOP1_VOLTAGE_CH2_R, epicsFloat64(top1.GetVoltage(1)));
        setDoubleParam(P_TOP1_VOLTAGE_CH3_R, epicsFloat64(top1.GetVoltage(2)));

        setDoubleParam(P_TOP2_VOLTAGE_CH1_R, epicsFloat64(top2.GetVoltage(0)));
        setDoubleParam(P_TOP2_VOLTAGE_CH2_R, epicsFloat64(top2.GetVoltage(1)));
        setDoubleParam(P_TOP2_VOLTAGE_CH3_R, epicsFloat64(top2.GetVoltage(2)));

        setDoubleParam(P_BOT0_VOLTAGE_CH1_R, epicsFloat64(bot0.GetVoltage(0)));
        setDoubleParam(P_BOT0_VOLTAGE_CH2_R, epicsFloat64(bot0.GetVoltage(1)));
        setDoubleParam(P_BOT0_VOLTAGE_CH3_R, epicsFloat64(bot0.GetVoltage(2)));

        setDoubleParam(P_BOT1_VOLTAGE_CH1_R, epicsFloat64(bot1.GetVoltage(0)));
        setDoubleParam(P_BOT1_VOLTAGE_CH2_R, epicsFloat64(bot1.GetVoltage(1)));
        setDoubleParam(P_BOT1_VOLTAGE_CH3_R, epicsFloat64(bot1.GetVoltage(2)));

        setDoubleParam(P_BOT2_VOLTAGE_CH1_R, epicsFloat64(bot2.GetVoltage(0)));
        setDoubleParam(P_BOT2_VOLTAGE_CH2_R, epicsFloat64(bot2.GetVoltage(1)));
        setDoubleParam(P_BOT2_VOLTAGE_CH3_R, epicsFloat64(bot2.GetVoltage(2)));

        setDoubleParam(P_INTER_VOLTAGE_CH1_R, epicsFloat64(inter.GetVoltage(0)));
        setDoubleParam(P_INTER_VOLTAGE_CH2_R, epicsFloat64(inter.GetVoltage(1)));
        setDoubleParam(P_INTER_VOLTAGE_CH3_R, epicsFloat64(inter.GetVoltage(2)));

        setDoubleParam(P_MAIN_VOLTAGE_CH1_R, epicsFloat64(main.GetVoltage(0)));
        setDoubleParam(P_MAIN_VOLTAGE_CH2_R, epicsFloat64(main.GetVoltage(1)));
        setDoubleParam(P_MAIN_VOLTAGE_CH3_R, epicsFloat64(main.GetVoltage(2)));

        // read status
        uint flags;
        bool res;

        res = top0.GetStatus(&flags);
        setIntegerParam(P_TOP0_STATUS_R, res);
        setUIntDigitalParam(P_TOP0_DETAILED_STATUS_R, flags, 0xff);

        res = top1.GetStatus(&flags);
        setIntegerParam(P_TOP1_STATUS_R, res);
        setUIntDigitalParam(P_TOP1_DETAILED_STATUS_R, flags, 0xff);

        res = top2.GetStatus(&flags);
        setIntegerParam(P_TOP2_STATUS_R, res);
        setUIntDigitalParam(P_TOP2_DETAILED_STATUS_R, flags, 0xff);

        res = bot0.GetStatus(&flags);
        setIntegerParam(P_BOT0_STATUS_R, res);
        setUIntDigitalParam(P_BOT0_DETAILED_STATUS_R, flags, 0xff);

        res = bot1.GetStatus(&flags);
        setIntegerParam(P_BOT1_STATUS_R, res);
        setUIntDigitalParam(P_BOT1_DETAILED_STATUS_R, flags, 0xff);

        res = bot2.GetStatus(&flags);
        setIntegerParam(P_BOT2_STATUS_R, res);
        setUIntDigitalParam(P_BOT2_DETAILED_STATUS_R, flags, 0xff);

        res = inter.GetStatus(&flags);
        setIntegerParam(P_INTER_STATUS_R, res);
        setUIntDigitalParam(P_INTER_DETAILED_STATUS_R, flags, 0xff);

        res = main.GetStatus(&flags);
        setIntegerParam(P_MAIN_STATUS_R, res);
        setUIntDigitalParam(P_MAIN_DETAILED_STATUS_R, flags, 0xff);

        // read presence
        setIntegerParam(P_TOP0_PRESENCE_R, top0.IsPresent());
        setIntegerParam(P_TOP1_PRESENCE_R, top1.IsPresent());
        setIntegerParam(P_TOP2_PRESENCE_R, top2.IsPresent());
        setIntegerParam(P_BOT0_PRESENCE_R, bot0.IsPresent());
        setIntegerParam(P_BOT1_PRESENCE_R, bot1.IsPresent());
        setIntegerParam(P_BOT2_PRESENCE_R, bot2.IsPresent());
        setIntegerParam(P_INTER_PRESENCE_R, inter.IsPresent());
        setIntegerParam(P_MAIN_PRESENCE_R, main.IsPresent());

        // read type ok
        setIntegerParam(P_TOP0_TYPE_OK_R, top0.IsTypeOK());
        setIntegerParam(P_TOP1_TYPE_OK_R, top1.IsTypeOK());
        setIntegerParam(P_TOP2_TYPE_OK_R, top2.IsTypeOK());
        setIntegerParam(P_BOT0_TYPE_OK_R, bot0.IsTypeOK());
        setIntegerParam(P_BOT1_TYPE_OK_R, bot1.IsTypeOK());
        setIntegerParam(P_BOT2_TYPE_OK_R, bot2.IsTypeOK());
        setIntegerParam(P_INTER_TYPE_OK_R, inter.IsTypeOK());
        setIntegerParam(P_MAIN_TYPE_OK_R, main.IsTypeOK());

        // read voltage ok
        setIntegerParam(P_TOP0_VOLTAGE_OK_R, top0.IsVoltageOK());
        setIntegerParam(P_TOP1_VOLTAGE_OK_R, top1.IsVoltageOK());
        setIntegerParam(P_TOP2_VOLTAGE_OK_R, top2.IsVoltageOK());
        setIntegerParam(P_BOT0_VOLTAGE_OK_R, bot0.IsVoltageOK());
        setIntegerParam(P_BOT1_VOLTAGE_OK_R, bot1.IsVoltageOK());
        setIntegerParam(P_BOT2_VOLTAGE_OK_R, bot2.IsVoltageOK());
        setIntegerParam(P_INTER_VOLTAGE_OK_R, inter.IsVoltageOK());
        setIntegerParam(P_MAIN_VOLTAGE_OK_R, main.IsVoltageOK());

        // read current ok
        setIntegerParam(P_TOP0_CURRENT_OK_R, isCurrentOK(top0.GetCurrentAlert()));
        setIntegerParam(P_TOP1_CURRENT_OK_R, isCurrentOK(top1.GetCurrentAlert()));
        setIntegerParam(P_TOP2_CURRENT_OK_R, isCurrentOK(top2.GetCurrentAlert()));
        setIntegerParam(P_BOT0_CURRENT_OK_R, isCurrentOK(bot0.GetCurrentAlert()));
        setIntegerParam(P_BOT1_CURRENT_OK_R, isCurrentOK(bot1.GetCurrentAlert()));
        setIntegerParam(P_BOT2_CURRENT_OK_R, isCurrentOK(bot2.GetCurrentAlert()));
        setIntegerParam(P_INTER_CURRENT_OK_R, isCurrentOK(inter.GetCurrentAlert()));
        setIntegerParam(P_MAIN_CURRENT_OK_R, isCurrentOK(main.GetCurrentAlert()));

        // read healthy
        setIntegerParam(P_TOP0_HEALTH_R, top0.IsHealthy());
        setIntegerParam(P_TOP1_HEALTH_R, top1.IsHealthy());
        setIntegerParam(P_TOP2_HEALTH_R, top2.IsHealthy());
        setIntegerParam(P_BOT0_HEALTH_R, bot0.IsHealthy());
        setIntegerParam(P_BOT1_HEALTH_R, bot1.IsHealthy());
        setIntegerParam(P_BOT2_HEALTH_R, bot2.IsHealthy());
        setIntegerParam(P_INTER_HEALTH_R, inter.IsHealthy());
        setIntegerParam(P_MAIN_HEALTH_R, main.IsHealthy());

        // get short open circuit info cs/ssa
/*        setIntegerParam(P_CS0_OPEN_R, csBoard0.GetOpen());
        setIntegerParam(P_CS0_SHORT_R, csBoard0.GetShort());
        setIntegerParam(P_CS0_FUSE_OK_R, csBoard0.GetFuseOK());
        setIntegerParam(P_CS1_OPEN_R, csBoard1.GetOpen());
        setIntegerParam(P_CS1_SHORT_R, csBoard1.GetShort());
        setIntegerParam(P_CS1_FUSE_OK_R, csBoard1.GetFuseOK());
        setIntegerParam(P_SSA_SHORT_R, ssaBoard.GetShort());
        setIntegerParam(P_SSA_FUSE_OK_R, ssaBoard.GetFuseOK());
*/
        // get vadj
        setIntegerParam(P_VADJ_R, main.GetVadj());

        unlock();
        callParamCallbacks();

        epicsMutexMustLock(terminateWorkersLock);
    }

    epicsMutexUnlock(terminateWorkersLock);

    pthread_mutex_lock(&ackFromWorkersLock);
    ++ackFromWorkersCount;
    pthread_cond_signal(&ackFromWorkersCond);
    pthread_mutex_unlock(&ackFromWorkersLock);
}

