#include "asynPortDriver.h"
#include "systemMaster.hpp"
#include <vector>

#include "ctrl_reg.h"
#include "adc_ctrl.h"
#include "ssa_ctrl.h"

#define RTD1 0
#define RTD2 1
#define FEP1 0
#define NIRP1 0
#define NIRP2 1
#define NIRP3 2
#define NIRP4 3
#define NIRP5 4
#define NIRP6 5
#define HVVOLT
#define HVCURR

#define CAVITY1 0
#define CAVITY2 1
#define CAVITY3 2
#define CAVITY4 3

#define CREATE_AND_SET_INT(NAME, IND, VALUE)    {createParam(NAME, asynParamInt32,   &IND); setIntegerParam(IND, VALUE); }
#define CREATE_AND_SET_DOUBLE(NAME, IND, VALUE) {createParam(NAME, asynParamFloat64, &IND); setDoubleParam(IND, VALUE); }
#define CREATE_AND_SET_OCTET(NAME, IND, VALUE)  {createParam(NAME, asynParamOctet,   &IND); setStringParam(IND, VALUE); }

#define CREATE_ANALOG_PARAMS(NAME)  \
    int P_ ## NAME ## _LATCH_STATUS_R; \
    int P_ ## NAME ## _CURRENT_STATUS_R; \
    int P_ ## NAME ## _TIMESTAMP_R; \
    int P_ ## NAME ## _TIMESTAMP_SEQ_R; \
    int P_ ## NAME ## _VALUE_SCAL_R; \
    int P_ ## NAME ## _VALUE_RAW_R; \
    int P_ ## NAME ## _ERROR_MSG_R; \
    int P_ ## NAME ## _RESET_S; \
    int P_ ## NAME ## _THRESHOLD_LOW_S; \
    int P_ ## NAME ## _THRESHOLD_HIGH_S; \
    int P_ ## NAME ## _THRESHOLD_LOW_RAW_R; \
    int P_ ## NAME ## _THRESHOLD_HIGH_RAW_R; \
    int P_ ## NAME ## _THRESHOLD_LOW_SCAL_R; \
    int P_ ## NAME ## _THRESHOLD_HIGH_SCAL_R; \
    int P_ ## NAME ## _THRESHOLD_SCALE_S; \
    int P_ ## NAME ## _THRESHOLD_SCALE_R; \
    int P_ ## NAME ## _THRESHOLD_OFFSET_S; \
    int P_ ## NAME ## _THRESHOLD_CNT_LOW_R; \
    int P_ ## NAME ## _THRESHOLD_CNT_HIGH_R; \
    int P_ ## NAME ## _THRESHOLD_CNT_LOW_TOTAL_R; \
    int P_ ## NAME ## _THRESHOLD_CNT_HIGH_TOTAL_R; \
    int P_ ## NAME ## _THRESHOLD_CNT_RESET_S; \
    int P_ ## NAME ## _MASKING_R; \
    int P_ ## NAME ## _MASKING_S;


#define CREATE_BINARY_PARAMS(NAME) \
    int P_ ## NAME ## _LATCH_STATUS_R; \
    int P_ ## NAME ## _CURRENT_STATUS_R; \
    int P_ ## NAME ## _TIMESTAMP_R; \
    int P_ ## NAME ## _TIMESTAMP_SEQ_R; \
    int P_ ## NAME ## _ERROR_MSG_R; \
    int P_ ## NAME ## _RESET_S; \
    int P_ ## NAME ## _MASKING_R; \
    int P_ ## NAME ## _MASKING_S;


#define CREATE_OUTPUT_PARAMS(NAME) \
    int P_ ## NAME ## _STATUS_R; \
    int P_ ## NAME ## _ERROR_MSG_R; \
    int P_ ## NAME ## _RESET_S; \
    int P_ ## NAME ## _FORCE_MODE_S; \
    int P_ ## NAME ## _FUSE_STATUS_R; \
    int P_ ## NAME ## _FUSE_STATUS_S; \
    int P_ ## NAME ## _FORCED_VALUE_S; \
    int P_ ## NAME ## _RESET_PERMIT_S;


#define CREATE_ANALOG(NAME, VECTOR) { \
    char name[255] = {0};\
    sprintf(name, "%s_LATCH_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _LATCH_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _CURRENT_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TIMESTAMP_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _TIMESTAMP_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TIMESTAMP_SEQ_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _TIMESTAMP_SEQ_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VALUE_SCAL_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _VALUE_SCAL_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_VALUE_RAW_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _VALUE_RAW_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_ERROR_MSG_R", #NAME); \
    CREATE_AND_SET_OCTET(name, P_ ## NAME ## _ERROR_MSG_R, "test"); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_RESET_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _RESET_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_LOW_S", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_LOW_S, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_HIGH_S", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_HIGH_S, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_LOW_RAW_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_LOW_RAW_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_HIGH_RAW_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_HIGH_RAW_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_LOW_SCAL_R", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_LOW_SCAL_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_HIGH_SCAL_R", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_HIGH_SCAL_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_SCALE_S", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_SCALE_S, 1.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_SCALE_R", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_SCALE_R, 1.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_OFFSET_S", #NAME); \
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _THRESHOLD_OFFSET_S, 1.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_CNT_LOW_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_CNT_LOW_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_CNT_HIGH_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_CNT_HIGH_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_CNT_LOW_TOTAL_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_CNT_LOW_TOTAL_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_CNT_HIGH_TOTAL_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_CNT_HIGH_TOTAL_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_THRESHOLD_CNT_RESET_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _THRESHOLD_CNT_RESET_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_MASKING_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _MASKING_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_MASKING_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _MASKING_S, 0); \
    memset(name, 0, 255);\
\
    TimeStampStruct t; \
    t.timestamp = 0.0; \
    t.latchedStatus = 0;\
    t.latchedStatusParam = P_ ## NAME ## _LATCH_STATUS_R; \
    t.timestampParam = P_ ## NAME ## _TIMESTAMP_R; \
    t.seqParam = P_ ## NAME ## _TIMESTAMP_SEQ_R; \
    VECTOR.push_back(t); \
}

#define CREATE_BINARY(NAME, VECTOR) { \
    char name[255] = {0};\
    sprintf(name, "%s_LATCH_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _LATCH_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_CURRENT_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _CURRENT_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TIMESTAMP_R", #NAME);\
    CREATE_AND_SET_DOUBLE(name, P_ ## NAME ## _TIMESTAMP_R, 0.0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_TIMESTAMP_SEQ_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _TIMESTAMP_SEQ_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_ERROR_MSG_R", #NAME); \
    CREATE_AND_SET_OCTET(name, P_ ## NAME ## _ERROR_MSG_R, "test"); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_RESET_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _RESET_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_MASKING_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _MASKING_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_MASKING_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _MASKING_S, 0); \
    memset(name, 0, 255);\
\
    TimeStampStruct t; \
    t.timestamp = 0.0; \
    t.latchedStatus = 0;\
    t.latchedStatusParam = P_ ## NAME ## _LATCH_STATUS_R; \
    t.timestampParam = P_ ## NAME ## _TIMESTAMP_R; \
    t.seqParam = P_ ## NAME ## _TIMESTAMP_SEQ_R; \
    VECTOR.push_back(t); \
}


#define CREATE_OUTPUT(NAME) { \
    char name[255] = {0};\
    sprintf(name, "%s_STATUS_R", #NAME);\
    CREATE_AND_SET_INT(name, P_ ## NAME ## _STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_ERROR_MSG_R", #NAME); \
    CREATE_AND_SET_OCTET(name, P_ ## NAME ## _ERROR_MSG_R, "test"); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_RESET_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _RESET_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_FORCE_MODE_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _FORCE_MODE_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_FORCED_VALUE_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _FORCED_VALUE_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_FUSE_STATUS_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _FUSE_STATUS_S, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_FUSE_STATUS_R", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _FUSE_STATUS_R, 0); \
    memset(name, 0, 255);\
\
    sprintf(name, "%s_RESET_PERMIT_S", #NAME); \
    CREATE_AND_SET_INT(name, P_ ## NAME ## _RESET_PERMIT_S, 0); \
    memset(name, 0, 255);\
\
}

struct TimeStampStruct {
    epicsFloat64 timestamp;
    int latchedStatus;
    int latchedStatusParam;
    int timestampParam;
    int seqParam;
};

typedef std::vector<TimeStampStruct> SequenceVector;

bool compareByTimestamp(const TimeStampStruct &a, const TimeStampStruct &b);

class kriaAsynPortDriver : public asynPortDriver {
public:
    kriaAsynPortDriver(const char *portName);
    ~kriaAsynPortDriver();

    void paramsReadout(void);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);

    void getFuse(int param, int cavity, SSAFusBits typ); 
    void getPermit(int param, int cavity, SSAOutEnBits typ); 
    void terminateThreads();

    void FEPinit();
    void SSAinit();

protected:
    CREATE_ANALOG_PARAMS(FEP1_C1);
    CREATE_ANALOG_PARAMS(FEP1_C2);
    CREATE_ANALOG_PARAMS(FEP1_C3);
    CREATE_ANALOG_PARAMS(FEP1_C4);
    CREATE_ANALOG_PARAMS(NIRP1);
    CREATE_ANALOG_PARAMS(NIRP2);
    CREATE_ANALOG_PARAMS(NIRP3);
    CREATE_ANALOG_PARAMS(NIRP4);
    CREATE_ANALOG_PARAMS(NIRP5);
    CREATE_ANALOG_PARAMS(NIRP6);

    CREATE_ANALOG_PARAMS(RTD1_C1);
    CREATE_ANALOG_PARAMS(RTD1_C2);
    CREATE_ANALOG_PARAMS(RTD1_C3);
    CREATE_ANALOG_PARAMS(RTD1_C4);
    CREATE_ANALOG_PARAMS(HVVOLT_C1);
    CREATE_ANALOG_PARAMS(HVVOLT_C2);
    CREATE_ANALOG_PARAMS(HVVOLT_C3);
    CREATE_ANALOG_PARAMS(HVVOLT_C4);
    CREATE_ANALOG_PARAMS(HVCURR_C1);
    CREATE_ANALOG_PARAMS(HVCURR_C2);
    CREATE_ANALOG_PARAMS(HVCURR_C3);
    CREATE_ANALOG_PARAMS(HVCURR_C4);

    CREATE_BINARY_PARAMS(PSP_C1);
    CREATE_BINARY_PARAMS(PSP_C2);
    CREATE_BINARY_PARAMS(PSP_C3);
    CREATE_BINARY_PARAMS(PSP_C4);
    CREATE_BINARY_PARAMS(VS);
    CREATE_BINARY_PARAMS(CVP_C1);
    CREATE_BINARY_PARAMS(CVP_C2);
    CREATE_BINARY_PARAMS(CVP_C3);
    CREATE_BINARY_PARAMS(CVP_C4);
    CREATE_BINARY_PARAMS(HE_LEVEL);
    CREATE_BINARY_PARAMS(CAS_C1);
    CREATE_BINARY_PARAMS(CAS_C2);
    CREATE_BINARY_PARAMS(CAS_C3);
    CREATE_BINARY_PARAMS(CAS_C4);
    CREATE_BINARY_PARAMS(SSA_C1);
    CREATE_BINARY_PARAMS(SSA_C2);
    CREATE_BINARY_PARAMS(SSA_C3);
    CREATE_BINARY_PARAMS(SSA_C4);

    CREATE_OUTPUT_PARAMS(LLRF_PERM_C1);
    CREATE_OUTPUT_PARAMS(LLRF_PERM_C2);
    CREATE_OUTPUT_PARAMS(LLRF_PERM_C3);
    CREATE_OUTPUT_PARAMS(LLRF_PERM_C4);
    CREATE_OUTPUT_PARAMS(SSA_PERM_C1);
    CREATE_OUTPUT_PARAMS(SSA_PERM_C2);
    CREATE_OUTPUT_PARAMS(SSA_PERM_C3);
    CREATE_OUTPUT_PARAMS(SSA_PERM_C4);
    CREATE_OUTPUT_PARAMS(SSADC_PERM_C1);
    CREATE_OUTPUT_PARAMS(SSADC_PERM_C2);
    CREATE_OUTPUT_PARAMS(SSADC_PERM_C3);
    CREATE_OUTPUT_PARAMS(SSADC_PERM_C4);
    CREATE_OUTPUT_PARAMS(MPS_PERM_C1);
    CREATE_OUTPUT_PARAMS(MPS_PERM_C2);
    CREATE_OUTPUT_PARAMS(MPS_PERM_C3);
    CREATE_OUTPUT_PARAMS(MPS_PERM_C4);

    int P_RTD1_C1_DAC_S;
    int P_RTD1_C1_PGA_S;
    int P_RTD1_C1_DAC_R;
    int P_RTD1_C1_PGA_R;
    int P_FEP1_C1_DAC_S;
    int P_FEP1_C1_DAC_R;

    int P_RTD1_C2_DAC_S;
    int P_RTD1_C2_PGA_S;
    int P_RTD1_C2_DAC_R;
    int P_RTD1_C2_PGA_R;
    int P_FEP1_C2_DAC_S;
    int P_FEP1_C2_DAC_R;

    int P_RTD1_C3_DAC_S;
    int P_RTD1_C3_PGA_S;
    int P_RTD1_C3_DAC_R;
    int P_RTD1_C3_PGA_R;
    int P_FEP1_C3_DAC_S;
    int P_FEP1_C3_DAC_R;

    int P_RTD1_C4_DAC_S;
    int P_RTD1_C4_PGA_S;
    int P_RTD1_C4_DAC_R;
    int P_RTD1_C4_PGA_R;
    int P_FEP1_C4_DAC_S;
    int P_FEP1_C4_DAC_R;

    int P_KRIA_ALARM_MSG_R;
    int P_KRIA_ALARM_R;
    int P_KRIA_ALARM_ACK_S;

    int P_HVVOLT_C1_RTD_READOUT_R;
    int P_HVVOLT_C2_RTD_READOUT_R;
    int P_HVVOLT_C3_RTD_READOUT_R;
    int P_HVVOLT_C4_RTD_READOUT_R;
    int P_HVCURR_C1_RTD_READOUT_R;
    int P_HVCURR_C2_RTD_READOUT_R;
    int P_HVCURR_C3_RTD_READOUT_R;
    int P_HVCURR_C4_RTD_READOUT_R;
    int P_RTD1_C1_RTD_READOUT_R;
    int P_RTD1_C2_RTD_READOUT_R;
    int P_RTD1_C3_RTD_READOUT_R;
    int P_RTD1_C4_RTD_READOUT_R;
    int P_RTD2_C1_RTD_READOUT_R;
    int P_RTD2_C2_RTD_READOUT_R;
    int P_RTD2_C3_RTD_READOUT_R;
    int P_RTD2_C4_RTD_READOUT_R;
    // fep
    int P_FEP1_C1_FEP_READOUT_R;
    int P_FEP1_C2_FEP_READOUT_R; 
    int P_FEP1_C3_FEP_READOUT_R;
    int P_FEP1_C4_FEP_READOUT_R;
    int P_FEP2_C1_FEP_READOUT_R;
    int P_FEP2_C2_FEP_READOUT_R; 
    int P_FEP2_C3_FEP_READOUT_R;
    int P_FEP2_C4_FEP_READOUT_R;
    int P_NIRP1_FEP_READOUT_R;
    int P_NIRP2_FEP_READOUT_R;
    int P_NIRP3_FEP_READOUT_R;
    int P_NIRP4_FEP_READOUT_R;
    int P_NIRP5_FEP_READOUT_R;
    int P_NIRP6_FEP_READOUT_R;
    int P_NIRP_SPARE1_FEP_READOUT_R;
    int P_NIRP_SPARE2_FEP_READOUT_R;

    SequenceVector vec;
    void markTimestamps();

private:
    SystemMaster *boardWrapper;

    CAdcCtrl *fep_reg;
    CAdcCtrl *rtd_reg;

    epicsEventId eventId_;

    uint8_t terminateWorkers;
    uint8_t ackFromWorkersCount;

    epicsMutexId terminateWorkersLock;
    epicsMutexId arrayLock;

    pthread_mutex_t ackFromWorkersLock;
    pthread_cond_t ackFromWorkersCond;

};
