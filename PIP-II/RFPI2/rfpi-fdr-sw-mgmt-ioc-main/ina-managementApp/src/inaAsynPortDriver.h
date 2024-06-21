#include "asynPortDriver.h"

#include "management_master.hpp"

#define P_VERSION_R_String 		"VERSION_R"
#define P_CS0_SHORT_R_String 		"CS0_SHORT_R"
#define P_CS0_OPEN_R_String 		"CS0_OPEN_R"
#define P_CS0_FUSE_OK_R_String 		"CS0_FUSE_OK_R"
#define P_CS1_SHORT_R_String 		"CS1_SHORT_R"
#define P_CS1_OPEN_R_String 		"CS1_OPEN_R"
#define P_CS1_FUSE_OK_R_String 		"CS1_FUSE_OK_R"
#define P_SSA_SHORT_R_String 		"SSA_SHORT_R"
#define P_SSA_OPEN_R_String 		"SSA_OPEN_R"
#define P_SSA_FUSE_OK_R_String		"SSA_FUSE_OK_R"
//#define P_STARTUP_S_String 		"STARTUP_S"
#define P_POWER_SEQ_R_String		"POWER_SEQ_R"
#define P_POWER_S_String		"POWER_S"
#define P_VADJ_S_String			"VADJ_S"
#define P_VADJ_R_String			"VADJ_R"
#define P_TC_S_String			"TC_S"

#define FANS_NUM   3
#define CUR_CHANS  3

#define CREATE_AND_SET_INT(NAME, IND, VALUE)    {createParam(NAME, asynParamInt32,   &IND); setIntegerParam(IND, VALUE); }
#define CREATE_AND_SET_DIGITAL(NAME, IND, VALUE)    {createParam(NAME, asynParamUInt32Digital, &IND); setUIntDigitalParam(IND, VALUE, 0xff); }
#define CREATE_AND_SET_DOUBLE(NAME, IND, VALUE) {createParam(NAME, asynParamFloat64, &IND); setDoubleParam(IND, VALUE); }
#define CREATE_AND_SET_OCTET(NAME, IND, VALUE) {createParam(NAME, asynParamOctet, &IND); setStringParam(IND, VALUE); }


#define CREATE_SLOT_PARAMS(NAME) \
    int P_ ## NAME ## _CURRENT_BOARD_NAME_R; \
    int P_ ## NAME ## _EXPECTED_BOARD_NAME_R; \
    int P_ ## NAME ## _TEMPERATURE_R; \
    int P_ ## NAME ## _VOLTAGE_CH1_R; \
    int P_ ## NAME ## _VOLTAGE_CH2_R; \
    int P_ ## NAME ## _VOLTAGE_CH3_R; \
    int P_ ## NAME ## _CURRENT_CH1_R; \
    int P_ ## NAME ## _CURRENT_CH2_R; \
    int P_ ## NAME ## _CURRENT_CH3_R; \
    int P_ ## NAME ## _PRESENCE_R; \
    int P_ ## NAME ## _DETAILED_STATUS_R; \
    int P_ ## NAME ## _STATUS_R; \
    int P_ ## NAME ## _TYPE_OK_R; \
    int P_ ## NAME ## _VOLTAGE_OK_R; \
    int P_ ## NAME ## _CURRENT_OK_R; \
    int P_ ## NAME ## _HEALTH_R; \
    int P_ ## NAME ## _CURRENT_WARN_CH1_S; \
    int P_ ## NAME ## _CURRENT_WARN_CH2_S; \
    int P_ ## NAME ## _CURRENT_WARN_CH3_S; \
    int P_ ## NAME ## _CURRENT_CRIT_CH1_S; \
    int P_ ## NAME ## _CURRENT_CRIT_CH2_S; \
    int P_ ## NAME ## _CURRENT_CRIT_CH3_S; \
    int P_ ## NAME ## _VOLTAGE_LOW_S; \
    int P_ ## NAME ## _VOLTAGE_HIGH_S;\
    int P_ ## NAME ## _INA_EVENT_R;\
    int P_ ## NAME ## _POWER_EVENT_R;\
    int P_ ## NAME ## _ACK_EVENT_S;\
    int cur ## NAME ## Warns[3];\
    int cur ## NAME ## Crits[3];


#define CREATE_FAN_PARAMS(IDX) \
    int P_FAN_ ## IDX ## _STATUS_R;\
    int P_FAN_ ## IDX ## _SPEED_R;\
    int P_FAN_ ## IDX ## _SPEED_S;


#define CREATE_SLOT(NAME) { \
    char name[255] = {0};\
    sprintf(name, "%s_CURRENT_BOARD_NAME_R", #NAME);\
    CREATE_AND_SET_OCTET(name, P_ ## NAME ## _CURRENT_BOARD_NAME_R, ""); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_EXPECTED_BOARD_NAME_R", #NAME);\
    CREATE_AND_SET_OCTET(name, P_ ## NAME ## _EXPECTED_BOARD_NAME_R, ""); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TEMPERATURE_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _TEMPERATURE_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VOLTAGE_CH1_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VOLTAGE_CH1_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VOLTAGE_CH2_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VOLTAGE_CH2_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VOLTAGE_CH3_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VOLTAGE_CH3_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_CH1_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CH1_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_CH2_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CH2_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_CH3_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CH3_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_PRESENCE_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _PRESENCE_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_DETAILED_STATUS_R", #NAME);\
    CREATE_AND_SET_DIGITAL(name, P_ ## NAME ## _DETAILED_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TYPE_OK_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _TYPE_OK_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VOLTAGE_OK_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _VOLTAGE_OK_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_OK_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _CURRENT_OK_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_HEALTH_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _HEALTH_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_INA_EVENT_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _INA_EVENT_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_POWER_EVENT_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _POWER_EVENT_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_ACK_EVENT_S", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _ACK_EVENT_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_WARN_CH1_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_WARN_CH1_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Warns[0] = P_ ## NAME ## _CURRENT_WARN_CH1_S;\
\
    sprintf(name, "%s_CURRENT_WARN_CH2_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_WARN_CH2_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Warns[1] = P_ ## NAME ## _CURRENT_WARN_CH2_S;\
\
    sprintf(name, "%s_CURRENT_WARN_CH3_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_WARN_CH3_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Warns[2] = P_ ## NAME ## _CURRENT_WARN_CH3_S;\
\
    sprintf(name, "%s_CURRENT_CRIT_CH1_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CRIT_CH1_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Crits[0] = P_ ## NAME ## _CURRENT_WARN_CH1_S;\
\
    sprintf(name, "%s_CURRENT_CRIT_CH2_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CRIT_CH2_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Crits[1] = P_ ## NAME ## _CURRENT_WARN_CH2_S;\
\
    sprintf(name, "%s_CURRENT_CRIT_CH3_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _CURRENT_CRIT_CH3_S, 0.0); \
    memset(name, 0, 255);\
    cur ## NAME ## Crits[2] = P_ ## NAME ## _CURRENT_WARN_CH3_S;\
\
    sprintf(name, "%s_VOLTAGE_LOW_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VOLTAGE_LOW_S, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VOLTAGE_HIGH_S", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VOLTAGE_HIGH_S, 0.0); \
    memset(name, 0, 255); \
}


#define CREATE_FAN(IDX) { \
    char name[255] = {0};\
    sprintf(name, "FAN_%s_SPEED_R", #IDX);\
    CREATE_AND_SET_INT(name, P_FAN_ ## IDX ## _SPEED_R, 0); \
    memset(name, 0, 255);\
    fans[0][IDX-1] = P_FAN_ ## IDX ## _SPEED_R;\
\
    sprintf(name, "FAN_%s_SPEED_S", #IDX);\
    CREATE_AND_SET_INT(name, P_FAN_ ## IDX ## _SPEED_S, 0); \
    memset(name, 0, 255); \
    fans[1][IDX-1] = P_FAN_ ## IDX ## _SPEED_S; \
\
    sprintf(name, "FAN_%s_STATUS_R", #IDX);\
    CREATE_AND_SET_INT(name, P_FAN_ ## IDX ## _STATUS_R, 0); \
    memset(name, 0, 255);\
    fans[2][IDX-1] = P_FAN_ ## IDX ## _STATUS_R;\
}


class inaAsynPortDriver : public asynPortDriver {
public:
    inaAsynPortDriver(const char *portName, const char *configPath);
    ~inaAsynPortDriver();

    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);

    int isCurrentOK(pair<uint8_t, uint8_t>);  

    void paramsReadout(void);

    void terminateThreads();

    void static PowerMonitorCallback(void* eventData, map<string, void*> usrData);

protected:
    CREATE_SLOT_PARAMS(TOP0);
    CREATE_SLOT_PARAMS(TOP1);
    CREATE_SLOT_PARAMS(TOP2);
    CREATE_SLOT_PARAMS(BOT0);
    CREATE_SLOT_PARAMS(BOT1);
    CREATE_SLOT_PARAMS(BOT2);
    CREATE_SLOT_PARAMS(MAIN);
    CREATE_SLOT_PARAMS(INTER);

    CREATE_FAN_PARAMS(1);
    CREATE_FAN_PARAMS(2);
    CREATE_FAN_PARAMS(3);

    int P_VERSION_R;
    int P_CS0_SHORT_R;
    int P_CS0_OPEN_R;
    int P_CS0_FUSE_OK_R;
    int P_CS1_SHORT_R;
    int P_CS1_OPEN_R;
    int P_CS1_FUSE_OK_R;
    int P_SSA_SHORT_R;
    int P_SSA_FUSE_OK_R;
    //int P_STARTUP_S;
    int P_POWER_S;
    int P_POWER_SEQ_R;
    int P_VADJ_R;
    int P_VADJ_S;
    int P_TC_S;

private:
    ManagementMaster master;

    Slot top0;
    Slot top1;
    Slot top2;
    Slot bot0;
    Slot bot1;
    Slot bot2;
    Slot inter;
    KriaSlot main;

    I2CMaster i2c;
    GPIOMaster gpio;

    int fans[3][FANS_NUM];
    epicsEventId eventId_;

    uint8_t terminateWorkers;
    uint8_t ackFromWorkersCount;

    epicsMutexId terminateWorkersLock;

    pthread_mutex_t ackFromWorkersLock;
    pthread_cond_t ackFromWorkersCond;

};
