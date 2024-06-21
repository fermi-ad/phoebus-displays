#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <algorithm>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>

#include "kriaAsynPortDriver.h"

#define RTD1_MASK 1
#define RTD2_MASK 2
#define FEP1_MASK 1

static const char *driverName = "kriaAsynPortDriver";
void paramsReadout(void *drvPvt);

kriaAsynPortDriver::kriaAsynPortDriver(const char *portName)
    : asynPortDriver(portName,
                     1, /* maxAddr */
                     asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask, /* Interface mask */
                     asynInt32Mask | asynFloat64Mask | asynOctetMask,   /* Interrupt mask */
                     0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                     1, /* Autoconnect */
                     0, /* Default priority */
                     0) /* Default stack size*/
{
    asynStatus status = asynSuccess;

    eventId_ = epicsEventCreate(epicsEventEmpty);

    // Inputs
    CREATE_ANALOG(FEP1_C1, vec);
    CREATE_ANALOG(FEP1_C2, vec);
    CREATE_ANALOG(FEP1_C3, vec);
    CREATE_ANALOG(FEP1_C4, vec);
    CREATE_ANALOG(NIRP1, vec);
    CREATE_ANALOG(NIRP2, vec);
    CREATE_ANALOG(NIRP3, vec);
    CREATE_ANALOG(NIRP4, vec);
    CREATE_ANALOG(NIRP5, vec);
    CREATE_ANALOG(NIRP6, vec);
    CREATE_ANALOG(RTD1_C1, vec);
    CREATE_ANALOG(RTD1_C2, vec);
    CREATE_ANALOG(RTD1_C3, vec);
    CREATE_ANALOG(RTD1_C4, vec);
    CREATE_ANALOG(HVVOLT_C1, vec);
    CREATE_ANALOG(HVVOLT_C2, vec);
    CREATE_ANALOG(HVVOLT_C3, vec);
    CREATE_ANALOG(HVVOLT_C4, vec);
    CREATE_ANALOG(HVCURR_C1, vec);
    CREATE_ANALOG(HVCURR_C2, vec);
    CREATE_ANALOG(HVCURR_C3, vec);
    CREATE_ANALOG(HVCURR_C4, vec);

    // Inputs
    CREATE_BINARY(PSP_C1, vec);
    CREATE_BINARY(PSP_C2, vec);
    CREATE_BINARY(PSP_C3, vec);
    CREATE_BINARY(PSP_C4, vec);
    CREATE_BINARY(VS, vec);
    CREATE_BINARY(CVP_C1, vec);
    CREATE_BINARY(CVP_C2, vec);
    CREATE_BINARY(CVP_C3, vec);
    CREATE_BINARY(CVP_C4, vec);
    CREATE_BINARY(HE_LEVEL, vec);
    CREATE_BINARY(CAS_C1, vec);
    CREATE_BINARY(CAS_C2, vec);
    CREATE_BINARY(CAS_C3, vec);
    CREATE_BINARY(CAS_C4, vec);
    CREATE_BINARY(SSA_C1, vec);
    CREATE_BINARY(SSA_C2, vec);
    CREATE_BINARY(SSA_C3, vec);
    CREATE_BINARY(SSA_C4, vec);

    // Outputs
    CREATE_OUTPUT(LLRF_PERM_C1);
    CREATE_OUTPUT(LLRF_PERM_C2);
    CREATE_OUTPUT(LLRF_PERM_C3);
    CREATE_OUTPUT(LLRF_PERM_C4);
    CREATE_OUTPUT(SSA_PERM_C1);
    CREATE_OUTPUT(SSA_PERM_C2);
    CREATE_OUTPUT(SSA_PERM_C3);
    CREATE_OUTPUT(SSA_PERM_C4);
    CREATE_OUTPUT(SSADC_PERM_C1);
    CREATE_OUTPUT(SSADC_PERM_C2);
    CREATE_OUTPUT(SSADC_PERM_C3);
    CREATE_OUTPUT(SSADC_PERM_C4);
    CREATE_OUTPUT(MPS_PERM_C1);
    CREATE_OUTPUT(MPS_PERM_C2);
    CREATE_OUTPUT(MPS_PERM_C3);
    CREATE_OUTPUT(MPS_PERM_C4);

    // DACs and PGAs
    createParam("RTD1_C1_DAC_S", asynParamInt32, &P_RTD1_C1_DAC_S);
    createParam("RTD1_C1_DAC_R", asynParamInt32, &P_RTD1_C1_DAC_R);
    createParam("RTD1_C1_PGA_S", asynParamInt32, &P_RTD1_C1_PGA_S);
    createParam("RTD1_C1_PGA_R", asynParamInt32, &P_RTD1_C1_PGA_R);
    createParam("FEP1_C1_DAC_S", asynParamInt32, &P_FEP1_C1_DAC_S);
    createParam("FEP1_C1_DAC_R", asynParamInt32, &P_FEP1_C1_DAC_R);

    createParam("RTD1_C2_DAC_S", asynParamInt32, &P_RTD1_C2_DAC_S);
    createParam("RTD1_C2_DAC_R", asynParamInt32, &P_RTD1_C2_DAC_R);
    createParam("RTD1_C2_PGA_S", asynParamInt32, &P_RTD1_C2_PGA_S);
    createParam("RTD1_C2_PGA_R", asynParamInt32, &P_RTD1_C2_PGA_R);
    createParam("FEP1_C2_DAC_S", asynParamInt32, &P_FEP1_C2_DAC_S);
    createParam("FEP1_C2_DAC_R", asynParamInt32, &P_FEP1_C2_DAC_R);

    createParam("RTD1_C3_DAC_S", asynParamInt32, &P_RTD1_C3_DAC_S);
    createParam("RTD1_C3_DAC_R", asynParamInt32, &P_RTD1_C3_DAC_R);
    createParam("RTD1_C3_PGA_S", asynParamInt32, &P_RTD1_C3_PGA_S);
    createParam("RTD1_C3_PGA_R", asynParamInt32, &P_RTD1_C3_PGA_R);
    createParam("FEP1_C3_DAC_S", asynParamInt32, &P_FEP1_C3_DAC_S);
    createParam("FEP1_C3_DAC_R", asynParamInt32, &P_FEP1_C3_DAC_R);

    createParam("RTD1_C4_DAC_S", asynParamInt32, &P_RTD1_C4_DAC_S);
    createParam("RTD1_C4_DAC_R", asynParamInt32, &P_RTD1_C4_DAC_R);
    createParam("RTD1_C4_PGA_S", asynParamInt32, &P_RTD1_C4_PGA_S);
    createParam("RTD1_C4_PGA_R", asynParamInt32, &P_RTD1_C4_PGA_R);
    createParam("FEP1_C4_DAC_S", asynParamInt32, &P_FEP1_C4_DAC_S);
    createParam("FEP1_C4_DAC_R", asynParamInt32, &P_FEP1_C4_DAC_R);

    // Alarm
    createParam("KRIA_ALARM_R", asynParamInt32, &P_KRIA_ALARM_R);
    createParam("KRIA_ALARM_ACK_S", asynParamInt32, &P_KRIA_ALARM_ACK_S);
    createParam("KRIA_ALARM_MSG_R", asynParamOctet, &P_KRIA_ALARM_MSG_R);

    // ADC readouts
    createParam("HVVOLT_C1_RTD_READOUT_R", asynParamInt32, &P_HVVOLT_C1_RTD_READOUT_R);
    createParam("HVVOLT_C2_RTD_READOUT_R", asynParamInt32, &P_HVVOLT_C2_RTD_READOUT_R);
    createParam("HVVOLT_C3_RTD_READOUT_R", asynParamInt32, &P_HVVOLT_C3_RTD_READOUT_R);
    createParam("HVVOLT_C4_RTD_READOUT_R", asynParamInt32, &P_HVVOLT_C4_RTD_READOUT_R);
    createParam("HVCURR_C1_RTD_READOUT_R", asynParamInt32, &P_HVCURR_C1_RTD_READOUT_R);
    createParam("HVCURR_C2_RTD_READOUT_R", asynParamInt32, &P_HVCURR_C2_RTD_READOUT_R);
    createParam("HVCURR_C3_RTD_READOUT_R", asynParamInt32, &P_HVCURR_C3_RTD_READOUT_R);
    createParam("HVCURR_C4_RTD_READOUT_R", asynParamInt32, &P_HVCURR_C4_RTD_READOUT_R);
    createParam("RTD1_C1_RTD_READOUT_R", asynParamInt32, &P_RTD1_C1_RTD_READOUT_R);
    createParam("RTD1_C2_RTD_READOUT_R", asynParamInt32, &P_RTD1_C2_RTD_READOUT_R);
    createParam("RTD1_C3_RTD_READOUT_R", asynParamInt32, &P_RTD1_C3_RTD_READOUT_R);
    createParam("RTD1_C4_RTD_READOUT_R", asynParamInt32, &P_RTD1_C4_RTD_READOUT_R);
    createParam("RTD2_C1_RTD_READOUT_R", asynParamInt32, &P_RTD2_C1_RTD_READOUT_R);
    createParam("RTD2_C2_RTD_READOUT_R", asynParamInt32, &P_RTD2_C2_RTD_READOUT_R);
    createParam("RTD2_C3_RTD_READOUT_R", asynParamInt32, &P_RTD2_C3_RTD_READOUT_R);
    createParam("RTD2_C4_RTD_READOUT_R", asynParamInt32, &P_RTD2_C4_RTD_READOUT_R);
    createParam("FEP1_C1_FEP_READOUT_R", asynParamInt32, &P_FEP1_C1_FEP_READOUT_R);
    createParam("FEP1_C2_FEP_READOUT_R", asynParamInt32, &P_FEP1_C2_FEP_READOUT_R);
    createParam("FEP1_C3_FEP_READOUT_R", asynParamInt32, &P_FEP1_C3_FEP_READOUT_R);
    createParam("FEP1_C4_FEP_READOUT_R", asynParamInt32, &P_FEP1_C4_FEP_READOUT_R);
    createParam("FEP2_C1_FEP_READOUT_R", asynParamInt32, &P_FEP2_C1_FEP_READOUT_R);
    createParam("FEP2_C2_FEP_READOUT_R", asynParamInt32, &P_FEP2_C2_FEP_READOUT_R);
    createParam("FEP2_C3_FEP_READOUT_R", asynParamInt32, &P_FEP2_C3_FEP_READOUT_R);
    createParam("FEP2_C4_FEP_READOUT_R", asynParamInt32, &P_FEP2_C4_FEP_READOUT_R);
    createParam("NIRP1_FEP_READOUT_R", asynParamInt32, &P_NIRP1_FEP_READOUT_R);
    createParam("NIRP2_FEP_READOUT_R", asynParamInt32, &P_NIRP2_FEP_READOUT_R);
    createParam("NIRP3_FEP_READOUT_R", asynParamInt32, &P_NIRP3_FEP_READOUT_R);
    createParam("NIRP4_FEP_READOUT_R", asynParamInt32, &P_NIRP4_FEP_READOUT_R);
    createParam("NIRP5_FEP_READOUT_R", asynParamInt32, &P_NIRP5_FEP_READOUT_R);
    createParam("NIRP6_FEP_READOUT_R", asynParamInt32, &P_NIRP6_FEP_READOUT_R);
    createParam("NIRP_SPARE1_FEP_READOUT_R", asynParamInt32, &P_NIRP_SPARE1_FEP_READOUT_R);
    createParam("NIRP_SPARE2_READOUT_R", asynParamInt32, &P_NIRP_SPARE2_FEP_READOUT_R);

    // Init device
    boardWrapper = new SystemMaster();
    boardWrapper->OneTimeInit();

    FEPinit();
    SSAinit();

    terminateWorkers    = 0;
    ackFromWorkersCount = 0;

    terminateWorkersLock = epicsMutexMustCreate();
    arrayLock = epicsMutexMustCreate();

    pthread_mutex_init ( &ackFromWorkersLock, NULL );
    pthread_cond_init  ( &ackFromWorkersCond, NULL );

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



asynStatus kriaAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value) {
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;

    status = (asynStatus) setIntegerParam(function, value);

    try
    {
        if (function == P_PSP_C1_MASKING_S) {
            boardWrapper->SetPSPMask(CAVITY1, value);
        }

        if (function == P_PSP_C2_MASKING_S) {
            boardWrapper->SetPSPMask(CAVITY2, value);
        }

        if (function == P_PSP_C3_MASKING_S) {
            boardWrapper->SetPSPMask(CAVITY3, value);
        }

        if (function == P_PSP_C4_MASKING_S) {
            boardWrapper->SetPSPMask(CAVITY4, value);
        }

    //    if (function == P_HE_PRESSURE_MASKING_S) {
    //        boardWrapper->SetHePressureMask(value);
    //    }

        if (function == P_HE_LEVEL_MASKING_S) {
            //boardWrapper->SetHeLevelMask(value);
            boardWrapper->SetHePressureMask(value);
        }

        if (function == P_CVP_C1_MASKING_S) {
            boardWrapper->SetCouplerVacuumMask(CAVITY1, value);
        }

        if (function == P_CVP_C2_MASKING_S) {
            boardWrapper->SetCouplerVacuumMask(CAVITY2, value);
        }

        if (function == P_CVP_C3_MASKING_S) {
            boardWrapper->SetCouplerVacuumMask(CAVITY3, value);
        }

        if (function == P_CVP_C4_MASKING_S) {
            boardWrapper->SetCouplerVacuumMask(CAVITY3, value);
        }

        if (function == P_CAS_C1_MASKING_S) {
            boardWrapper->SetCouplerAirFlowMask(CAVITY1, value);
        }

        if (function == P_CAS_C2_MASKING_S) {
            boardWrapper->SetCouplerAirFlowMask(CAVITY2, value);
        }

        if (function == P_CAS_C3_MASKING_S) {
            boardWrapper->SetCouplerAirFlowMask(CAVITY3, value);
        }

        if (function == P_CAS_C4_MASKING_S) {
            boardWrapper->SetCouplerAirFlowMask(CAVITY4, value);
        }

        if (function == P_VS_MASKING_S) {
            boardWrapper->SetVacuumStatMask(value);
        }

        if (function == P_RTD1_C1_DAC_S) {
            // not implemented yet
            //boardWrapper->RTDSetDac(RTD1_MASK, value);
        }

        if (function == P_RTD1_C2_DAC_S) {
            // not implemented yet
            //boardWrapper->RTDSetDac(RTD1_MASK, value);
        }

        if (function == P_RTD1_C3_DAC_S) {
            // not implemented yet
            //boardWrapper->RTDSetDac(RTD1_MASK, value);
        }

        if (function == P_RTD1_C4_DAC_S) {
            // not implemented yet
            //boardWrapper->RTDSetDac(RTD1_MASK, value);
        }

        if (function == P_RTD1_C1_PGA_S) {
            // not implemented yet
            //boardWrapper->RTDSetPga(RTD1_MASK, value);
        }

        if (function == P_RTD1_C2_PGA_S) {
            // not implemented yet
            //boardWrapper->RTDSetPga(RTD1_MASK, value);
        }

        if (function == P_RTD1_C3_PGA_S) {
            // not implemented yet
            //boardWrapper->RTDSetPga(RTD1_MASK, value);
        }

        if (function == P_RTD1_C4_PGA_S) {
            // not implemented yet
            //boardWrapper->RTDSetPga(RTD1_MASK, value);
        }

        if (function == P_SSA_C1_RESET_S || function == P_SSADC_PERM_C1_RESET_S || function == P_LLRF_PERM_C1_RESET_S || function == P_SSA_PERM_C1_RESET_S || function == P_MPS_PERM_C1_RESET_S) {
            boardWrapper->SSA().ResetProt();
        }

        if (function == P_SSA_C2_RESET_S || function == P_SSADC_PERM_C2_RESET_S || function == P_LLRF_PERM_C2_RESET_S || function == P_SSA_PERM_C2_RESET_S || function == P_MPS_PERM_C2_RESET_S) {
            boardWrapper->SSA().ResetProt();
        }

        if (function == P_SSA_C3_RESET_S || function == P_SSADC_PERM_C3_RESET_S || function == P_LLRF_PERM_C3_RESET_S || function == P_SSA_PERM_C3_RESET_S || function == P_MPS_PERM_C3_RESET_S) {
            boardWrapper->SSA().ResetProt();
        }

        if (function == P_SSA_C4_RESET_S || function == P_SSADC_PERM_C4_RESET_S || function == P_LLRF_PERM_C4_RESET_S || function == P_SSA_PERM_C4_RESET_S || function == P_MPS_PERM_C4_RESET_S) {
            boardWrapper->SSA().ResetProt();
        }

        if (function == P_SSA_C1_MASKING_S) {
            boardWrapper->SetSSAReadyMask(CAVITY1, value);
        }

        if (function == P_SSA_C2_MASKING_S) {
            boardWrapper->SetSSAReadyMask(CAVITY2, value);
        }

        if (function == P_SSA_C3_MASKING_S) {
            boardWrapper->SetSSAReadyMask(CAVITY3, value);
        }

        if (function == P_SSA_C4_MASKING_S) {
            boardWrapper->SetSSAReadyMask(CAVITY4, value);
        }

        if (function == P_SSA_PERM_C1_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSA_PERM_C1_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::SSAPermitPermit, value);
            }
        }

        if (function == P_SSA_PERM_C2_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSA_PERM_C2_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::SSAPermitPermit, value);
            }
        }

        if (function == P_SSA_PERM_C3_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSA_PERM_C3_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::SSAPermitPermit, value);
            }
        }

        if (function == P_SSA_PERM_C4_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSA_PERM_C4_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::SSAPermitPermit, value);
            }
        }

        if (function == P_SSADC_PERM_C1_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSADC_PERM_C1_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::SSADCPermit, value);
            }
        }

        if (function == P_SSADC_PERM_C2_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSADC_PERM_C2_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::SSADCPermit, value);
            }
        }

        if (function == P_SSADC_PERM_C3_FORCED_VALUE_S) {

            int forced;
            getIntegerParam(P_SSADC_PERM_C3_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::SSADCPermit, value);
            }
        }

        if (function == P_SSADC_PERM_C4_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_SSADC_PERM_C4_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::SSADCPermit, value);
            }
        }

        if (function == P_LLRF_PERM_C1_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_LLRF_PERM_C1_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::LLRFPermit, value);
            }
        }

        if (function == P_LLRF_PERM_C2_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_LLRF_PERM_C2_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::LLRFPermit, value);
            }
        }

        if (function == P_LLRF_PERM_C3_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_LLRF_PERM_C3_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::LLRFPermit, value);
            }
        }

        if (function == P_LLRF_PERM_C4_FORCED_VALUE_S) {
            int forced;
            getIntegerParam(P_LLRF_PERM_C4_FORCE_MODE_S, &forced);
            if (forced) {
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::LLRFPermit, value);
            }
        }

        if (function == P_SSA_PERM_C1_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_SSA_PERM_C1_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::SSAPermitPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY1, SSAOutEnBits::SSAPermitPermit);
            }
        }

        if (function == P_SSA_PERM_C2_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_SSA_PERM_C2_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::SSAPermitPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY2, SSAOutEnBits::SSAPermitPermit);
            }
        }

        if (function == P_SSA_PERM_C3_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_SSA_PERM_C3_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::SSAPermitPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY3, SSAOutEnBits::SSAPermitPermit);
            }
        }

        if (function == P_SSA_PERM_C4_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_SSA_PERM_C4_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::SSAPermitPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY4, SSAOutEnBits::SSAPermitPermit);
            }
        }

        if (function == P_SSADC_PERM_C1_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_SSADC_PERM_C1_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::SSADCPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY1, SSAOutEnBits::SSADCPermit);
            }
        }

        if (function == P_SSADC_PERM_C2_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_SSADC_PERM_C2_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::SSADCPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY2, SSAOutEnBits::SSADCPermit);
            }
        }

        if (function == P_SSADC_PERM_C3_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_SSADC_PERM_C3_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::SSADCPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY3, SSAOutEnBits::SSADCPermit);
            }
        }

        if (function == P_SSADC_PERM_C4_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_SSADC_PERM_C4_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::SSADCPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY4, SSAOutEnBits::SSADCPermit);
            }
        }

        if (function == P_LLRF_PERM_C1_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_LLRF_PERM_C1_FORCED_VALUE_S, &state);
            boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::LLRFPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY1, SSAOutEnBits::LLRFPermit);
            }
        }

        if (function == P_LLRF_PERM_C2_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_LLRF_PERM_C2_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::LLRFPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY2, SSAOutEnBits::LLRFPermit);
            }
        }

        if (function == P_LLRF_PERM_C3_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_LLRF_PERM_C3_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::LLRFPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY3, SSAOutEnBits::LLRFPermit);
            }
        }

        if (function == P_LLRF_PERM_C4_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_LLRF_PERM_C4_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::LLRFPermit, state);
            } else {
                boardWrapper->ClearPermit(CAVITY4, SSAOutEnBits::LLRFPermit);
            }
        }

        if (function == P_MPS_PERM_C1_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_MPS_PERM_C1_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY1, SSAOutEnBits::MPS, state);
            } else {
                boardWrapper->ClearPermit(CAVITY1, SSAOutEnBits::MPS);
            }
        }

        if (function == P_MPS_PERM_C2_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_MPS_PERM_C2_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY2, SSAOutEnBits::MPS, state);
            } else {
                boardWrapper->ClearPermit(CAVITY2, SSAOutEnBits::MPS);
            }
        }

        if (function == P_MPS_PERM_C3_FORCE_MODE_S) {
            if (value) {
                int state;
                getIntegerParam(P_MPS_PERM_C3_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY3, SSAOutEnBits::MPS, state);
            } else {
                boardWrapper->ClearPermit(CAVITY3, SSAOutEnBits::MPS);
            }
        }

        if (function == P_MPS_PERM_C4_FORCE_MODE_S) {
            // not implemented yet
            if (value) {
                int state;
                getIntegerParam(P_MPS_PERM_C4_FORCED_VALUE_S, &state);
                boardWrapper->ForcePermit(CAVITY4, SSAOutEnBits::MPS, state);
            } else {
                boardWrapper->ClearPermit(CAVITY4, SSAOutEnBits::MPS);
            }
        }


        if (function == P_PSP_C1_RESET_S || function == P_PSP_C2_RESET_S || function == P_PSP_C3_RESET_S || function == P_PSP_C4_RESET_S || function == P_VS_RESET_S || function == P_CVP_C1_RESET_S || function == P_CVP_C2_RESET_S || function == P_CVP_C3_RESET_S || function == P_CVP_C4_RESET_S ||  function == P_HE_LEVEL_RESET_S || function == P_CAS_C1_RESET_S || function == P_CAS_C2_RESET_S || function == P_CAS_C3_RESET_S || function == P_CAS_C4_RESET_S) {
            boardWrapper->CS().ResetProt();
        }

        if (function == P_RTD1_C1_RESET_S || function == P_RTD1_C2_RESET_S || function == P_RTD1_C3_RESET_S || function == P_RTD1_C4_RESET_S || function == P_HVVOLT_C1_RESET_S || function == P_HVVOLT_C2_RESET_S || function == P_HVVOLT_C3_RESET_S || function == P_HVVOLT_C4_RESET_S || function == P_HVCURR_C1_RESET_S || function == P_HVCURR_C2_RESET_S || function == P_HVCURR_C3_RESET_S || function == P_HVCURR_C4_RESET_S) {
            // not implemented yet
            //boardWrapper->RTD().ResetProt();
        }

        if (function == P_RTD1_C1_THRESHOLD_CNT_RESET_S || function == P_RTD1_C2_THRESHOLD_CNT_RESET_S || function == P_RTD1_C3_THRESHOLD_CNT_RESET_S || function == P_RTD1_C4_THRESHOLD_CNT_RESET_S || function == P_HVVOLT_C1_THRESHOLD_CNT_RESET_S || function == P_HVVOLT_C2_THRESHOLD_CNT_RESET_S || function == P_HVVOLT_C3_THRESHOLD_CNT_RESET_S || function == P_HVVOLT_C4_THRESHOLD_CNT_RESET_S || function == P_HVCURR_C1_THRESHOLD_CNT_RESET_S || function == P_HVCURR_C2_THRESHOLD_CNT_RESET_S || function == P_HVCURR_C3_THRESHOLD_CNT_RESET_S || function == P_HVCURR_C4_THRESHOLD_CNT_RESET_S) {
            boardWrapper->RTDResetCnt();
        }

        if (function == P_NIRP1_THRESHOLD_CNT_RESET_S || function == P_NIRP2_THRESHOLD_CNT_RESET_S || function == P_NIRP3_THRESHOLD_CNT_RESET_S || function == P_NIRP4_THRESHOLD_CNT_RESET_S || function == P_NIRP5_THRESHOLD_CNT_RESET_S || function == P_NIRP6_THRESHOLD_CNT_RESET_S || function == P_FEP1_C1_THRESHOLD_CNT_RESET_S) {
            boardWrapper->FEPResetCnt();
            //boardWrapper->ADCResetCnt();
        }

        if (function == P_NIRP1_RESET_S || function == P_NIRP2_RESET_S || function == P_NIRP3_RESET_S || function == P_NIRP4_RESET_S || function == P_NIRP5_RESET_S || function == P_NIRP6_RESET_S || function == P_FEP1_C1_RESET_S || function == P_FEP1_C2_RESET_S || function == P_FEP1_C3_RESET_S || function == P_FEP1_C4_RESET_S) {
            boardWrapper->FEP().ResetProt();
            //boardWrapper->ADC().ResetProt();
        }

        if (function == P_HVVOLT_C1_MASKING_S) {
            boardWrapper->SetHVCplBiasVoltMask(CAVITY1, value);
        }

        if (function == P_HVVOLT_C2_MASKING_S) {
            boardWrapper->SetHVCplBiasVoltMask(CAVITY2, value);
        }

        if (function == P_HVVOLT_C3_MASKING_S) {
            boardWrapper->SetHVCplBiasVoltMask(CAVITY3, value);
        }

        if (function == P_HVVOLT_C4_MASKING_S) {
            boardWrapper->SetHVCplBiasVoltMask(CAVITY4, value);
        }

        if (function == P_HVCURR_C1_MASKING_S) {
            boardWrapper->SetHVCplBiasCurrMask(CAVITY1, value);
        }

        if (function == P_HVCURR_C2_MASKING_S) {
            boardWrapper->SetHVCplBiasCurrMask(CAVITY2, value);
        }

        if (function == P_HVCURR_C3_MASKING_S) {
            boardWrapper->SetHVCplBiasCurrMask(CAVITY3, value);
        }

        if (function == P_HVCURR_C4_MASKING_S) {
            boardWrapper->SetHVCplBiasCurrMask(CAVITY4, value);
        }

        if (function == P_RTD1_C1_MASKING_S) {
            boardWrapper->SetTemperatureMask(CAVITY1, RTD1, value);
        }

        if (function == P_RTD1_C2_MASKING_S) {
            boardWrapper->SetTemperatureMask(CAVITY2, RTD1, value);
        }

        if (function == P_RTD1_C3_MASKING_S) {
            boardWrapper->SetTemperatureMask(CAVITY3, RTD1, value);
        }

        if (function == P_RTD1_C4_MASKING_S) {
            boardWrapper->SetTemperatureMask(CAVITY4, RTD1, value);
        }

        if (function == P_NIRP1_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP1, value);
        }

        if (function == P_NIRP2_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP2, value);
        }

        if (function == P_NIRP3_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP3, value);
        }

        if (function == P_NIRP4_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP4, value);
        }

        if (function == P_NIRP5_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP5, value);
        }

        if (function == P_NIRP6_MASKING_S) {
            boardWrapper->SetNIRPMask(NIRP6, value);
        }

        if (function == P_FEP1_C1_MASKING_S) {
            boardWrapper->SetFEPMask(FEP1, value);
        }

        if (function == P_FEP1_C2_MASKING_S) {
            boardWrapper->SetFEPMask(FEP1, value);
        }

        if (function == P_FEP1_C3_MASKING_S) {
            boardWrapper->SetFEPMask(FEP1, value);
        }

        if (function == P_FEP1_C4_MASKING_S) {
            boardWrapper->SetFEPMask(FEP1, value);
        }

        if (function == P_FEP1_C1_DAC_S) {
            // not implemented yet
            //boardWrapper->FEPSetDac(FEP1_MASK, value);
        }

        if (function == P_FEP1_C2_DAC_S) {
            // not implemented yet
            //boardWrapper->FEPSetDac(FEP1_MASK, value);
        }

        if (function == P_FEP1_C3_DAC_S) {
            // not implemented yet
            //boardWrapper->FEPSetDac(FEP1_MASK, value);
        }

        if (function == P_FEP1_C4_DAC_S) {
            // not implemented yet
            //boardWrapper->FEPSetDac(FEP1_MASK, value);
        }

        if (function == P_SSA_PERM_C1_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY1, SSAFusBits::SSAPermitFus, value);
        }

        if (function == P_SSA_PERM_C2_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY2, SSAFusBits::SSAPermitFus, value);
        }

        if (function == P_SSA_PERM_C3_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY3, SSAFusBits::SSAPermitFus, value);
        }

        if (function == P_SSA_PERM_C4_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY4, SSAFusBits::SSAPermitFus, value);
        }

        if (function == P_LLRF_PERM_C1_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY1, SSAFusBits::LLRFFus, value);
        }

        if (function == P_LLRF_PERM_C2_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY2, SSAFusBits::LLRFFus, value);
        }

        if (function == P_LLRF_PERM_C3_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY3, SSAFusBits::LLRFFus, value);
        }

        if (function == P_LLRF_PERM_C4_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY4, SSAFusBits::LLRFFus, value);
        }

        if (function == P_MPS_PERM_C1_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY1, SSAFusBits::MPSFus, value);
        }

        if (function == P_MPS_PERM_C2_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY2, SSAFusBits::MPSFus, value);
        }

        if (function == P_MPS_PERM_C3_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY3, SSAFusBits::MPSFus, value);
        }

        if (function == P_MPS_PERM_C4_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY4, SSAFusBits::MPSFus, value);
        }

        if (function == P_SSADC_PERM_C1_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY1, SSAFusBits::SSADCFus, value);
        }

        if (function == P_SSADC_PERM_C2_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY2, SSAFusBits::SSADCFus, value);
        }

        if (function == P_SSADC_PERM_C3_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY3, SSAFusBits::SSADCFus, value);
        }

        if (function == P_SSADC_PERM_C4_FUSE_STATUS_S) {
            boardWrapper->SetFuse(CAVITY4, SSAFusBits::SSADCFus, value);
        }

        if (function == P_KRIA_ALARM_ACK_S) {
            setStringParam(P_KRIA_ALARM_MSG_R, "");
            setIntegerParam(P_KRIA_ALARM_R, 0);
        }

	if (function == P_MPS_PERM_C1_RESET_PERMIT_S || function == P_SSADC_PERM_C1_RESET_PERMIT_S || function == P_SSA_PERM_C1_RESET_PERMIT_S || function == P_LLRF_PERM_C1_RESET_PERMIT_S) {
	    boardWrapper->ResetSSAPermit(CAVITY1);
        }

	if (function == P_MPS_PERM_C2_RESET_PERMIT_S || function == P_SSADC_PERM_C2_RESET_PERMIT_S || function == P_SSA_PERM_C2_RESET_PERMIT_S || function == P_LLRF_PERM_C2_RESET_PERMIT_S) {
	    boardWrapper->ResetSSAPermit(CAVITY2);
        }

	if (function == P_MPS_PERM_C3_RESET_PERMIT_S || function == P_SSADC_PERM_C3_RESET_PERMIT_S || function == P_SSA_PERM_C3_RESET_PERMIT_S || function == P_LLRF_PERM_C3_RESET_PERMIT_S) {
	    boardWrapper->ResetSSAPermit(CAVITY3);
        }

	if (function == P_MPS_PERM_C4_RESET_PERMIT_S || function == P_SSADC_PERM_C4_RESET_PERMIT_S || function == P_SSA_PERM_C4_RESET_PERMIT_S || function == P_LLRF_PERM_C4_RESET_PERMIT_S) {
	    boardWrapper->ResetSSAPermit(CAVITY4);
        }

    }
    catch(const string& s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const char* s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::runtime_error& re)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::exception& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(SsaLatticeCommException& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(...)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }

    callParamCallbacks();
    return status;
}

asynStatus kriaAsynPortDriver::writeFloat64(asynUser *pasynUser, epicsFloat64 value) {
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;

    status = (asynStatus) setDoubleParam(function, value);

    try
    {
        if (function == P_RTD1_C1_THRESHOLD_SCALE_S) {
            boardWrapper->SetTemperatureScaling(CAVITY1, RTD1, value);
        }

        if (function == P_RTD1_C2_THRESHOLD_SCALE_S) {
            boardWrapper->SetTemperatureScaling(CAVITY2, RTD1, value);
        }

        if (function == P_RTD1_C3_THRESHOLD_SCALE_S) {
            boardWrapper->SetTemperatureScaling(CAVITY3, RTD1, value);
         }

        if (function == P_RTD1_C4_THRESHOLD_SCALE_S) {
            boardWrapper->SetTemperatureScaling(CAVITY4, RTD1, value);
        }

        if (function == P_HVVOLT_C1_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasVoltScaling(CAVITY1, value);
        }

        if (function == P_HVVOLT_C2_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasVoltScaling(CAVITY2, value);
        }

        if (function == P_HVVOLT_C3_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasVoltScaling(CAVITY3, value);
        }

        if (function == P_HVVOLT_C4_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasVoltScaling(CAVITY4, value);
        }

        if (function == P_HVCURR_C1_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasCurrScaling(CAVITY1, value);
        }

        if (function == P_HVCURR_C2_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasCurrScaling(CAVITY2, value);
        }

        if (function == P_HVCURR_C3_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasCurrScaling(CAVITY3, value);
        }

        if (function == P_HVCURR_C4_THRESHOLD_SCALE_S) {
            boardWrapper->SetHVCplBiasCurrScaling(CAVITY4, value);
        }

        if (function == P_NIRP1_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP1, value);
        }

        if (function == P_NIRP2_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP2, value);
        }

        if (function == P_NIRP3_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP3, value);
        }

        if (function == P_NIRP4_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP4, value);
        }

        if (function == P_NIRP5_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP5, value);
        }

        if (function == P_NIRP6_THRESHOLD_SCALE_S) {
            boardWrapper->SetNIRPScaling(NIRP6, value);
        }

        if (function == P_FEP1_C1_THRESHOLD_SCALE_S) {
            boardWrapper->SetFEPScaling(FEP1, value);
        }

        if (function == P_FEP1_C2_THRESHOLD_SCALE_S) {
            boardWrapper->SetFEPScaling(FEP1, value);
        }

        if (function == P_FEP1_C3_THRESHOLD_SCALE_S) {
            boardWrapper->SetFEPScaling(FEP1, value);
        }

        if (function == P_FEP1_C4_THRESHOLD_SCALE_S) {
            boardWrapper->SetFEPScaling(FEP1, value);
        }

        if (function == P_RTD1_C1_THRESHOLD_LOW_S) {
            boardWrapper->SetTemperatureThresholdLoRaw(CAVITY1, RTD1, value);
        }

        if (function == P_RTD1_C1_THRESHOLD_HIGH_S) {
            boardWrapper->SetTemperatureThresholdHiRaw(CAVITY1, RTD1, value);
        }

        if (function == P_RTD1_C2_THRESHOLD_LOW_S) {
            boardWrapper->SetTemperatureThresholdLoRaw(CAVITY2, RTD1, value);
        }

        if (function == P_RTD1_C2_THRESHOLD_HIGH_S) {
            boardWrapper->SetTemperatureThresholdHiRaw(CAVITY2, RTD1, value);
        }

        if (function == P_RTD1_C3_THRESHOLD_LOW_S) {
            boardWrapper->SetTemperatureThresholdLoRaw(CAVITY3, RTD1, value);
        }

        if (function == P_RTD1_C3_THRESHOLD_HIGH_S) {
            boardWrapper->SetTemperatureThresholdHiRaw(CAVITY3, RTD1, value);
        }

        if (function == P_RTD1_C4_THRESHOLD_LOW_S) {
            boardWrapper->SetTemperatureThresholdLoRaw(CAVITY4, RTD1, value);
        }

        if (function == P_RTD1_C4_THRESHOLD_HIGH_S) {
            boardWrapper->SetTemperatureThresholdHiRaw(CAVITY4, RTD1, value);
        }

        if (function == P_HVVOLT_C1_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasVoltThresholdLoRaw(CAVITY1, value);
        }

        if (function == P_HVCURR_C1_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasCurrThresholdLoRaw(CAVITY1, value);
        }

        if (function == P_HVVOLT_C1_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasVoltThresholdHiRaw(CAVITY1, value);
        }

        if (function == P_HVCURR_C1_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasCurrThresholdHiRaw(CAVITY1, value);
        }

        if (function == P_HVVOLT_C2_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasVoltThresholdLoRaw(CAVITY2, value);
        }

        if (function == P_HVCURR_C2_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasCurrThresholdLoRaw(CAVITY2, value);
        }

        if (function == P_HVVOLT_C2_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasVoltThresholdHiRaw(CAVITY2, value);
        }

        if (function == P_HVCURR_C2_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasCurrThresholdHiRaw(CAVITY2, value);
        }

        if (function == P_HVVOLT_C3_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasVoltThresholdLoRaw(CAVITY3, value);
        }

        if (function == P_HVCURR_C3_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasCurrThresholdLoRaw(CAVITY3, value);
        }

        if (function == P_HVVOLT_C3_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasVoltThresholdHiRaw(CAVITY3, value);
        }

        if (function == P_HVCURR_C3_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasCurrThresholdHiRaw(CAVITY3, value);
        }

        if (function == P_HVVOLT_C4_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasVoltThresholdLoRaw(CAVITY4, value);
        }

        if (function == P_HVCURR_C4_THRESHOLD_LOW_S) {
            boardWrapper->SetHVCplBiasCurrThresholdLoRaw(CAVITY4, value);
        }

        if (function == P_HVVOLT_C4_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasVoltThresholdHiRaw(CAVITY4, value);
        }

        if (function == P_HVCURR_C4_THRESHOLD_HIGH_S) {
            boardWrapper->SetHVCplBiasCurrThresholdHiRaw(CAVITY4, value);
        }

        if (function == P_FEP1_C1_THRESHOLD_LOW_S) {
            boardWrapper->SetFEPThresholdLo(FEP1, value);
        }

        if (function == P_FEP1_C2_THRESHOLD_LOW_S) {
            boardWrapper->SetFEPThresholdLoRaw(FEP1, value);
        }

        if (function == P_FEP1_C3_THRESHOLD_LOW_S) {
            boardWrapper->SetFEPThresholdLoRaw(FEP1, value);
        }

        if (function == P_FEP1_C4_THRESHOLD_LOW_S) {
            boardWrapper->SetFEPThresholdLoRaw(FEP1, value);
        }

        if (function == P_NIRP1_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP1, value);
        }

        if (function == P_NIRP2_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP2, value);
        }

        if (function == P_NIRP3_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP3, value);
        }

        if (function == P_NIRP4_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP4, value);
        }
    
        if (function == P_NIRP5_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP5, value);
        }
    
        if (function == P_NIRP6_THRESHOLD_LOW_S) {
            boardWrapper->SetNIRPThresholdLoRaw(NIRP6, value);
        }

        if (function == P_FEP1_C1_THRESHOLD_HIGH_S) {
            boardWrapper->SetFEPThresholdHiRaw(FEP1, value);
        }

        if (function == P_FEP1_C2_THRESHOLD_HIGH_S) {
            boardWrapper->SetFEPThresholdHiRaw(FEP1, value);
        }

        if (function == P_FEP1_C3_THRESHOLD_HIGH_S) {
            boardWrapper->SetFEPThresholdHiRaw(FEP1, value);
        }

        if (function == P_FEP1_C4_THRESHOLD_HIGH_S) {
            boardWrapper->SetFEPThresholdHiRaw(FEP1, value);
        }

        if (function == P_NIRP1_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP1, value);
        }

        if (function == P_NIRP2_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP2, value);
        }

        if (function == P_NIRP3_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP3, value);
        }

        if (function == P_NIRP4_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP4, value);
        }

        if (function == P_NIRP5_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP4, value);
        }

        if (function == P_NIRP6_THRESHOLD_HIGH_S) {
            boardWrapper->SetNIRPThresholdHiRaw(NIRP4, value);
        }
    }
    catch(const string& s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const char* s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::runtime_error& re)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::exception& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(SsaLatticeCommException& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(...)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }

    callParamCallbacks();
    return status;
}

void kriaAsynPortDriver::markTimestamps() {
    // Update timestamp sequence
    for(unsigned int i=0; i<vec.size(); i++)
    {
        getDoubleParam(vec[i].timestampParam, &vec[i].timestamp);
        getIntegerParam(vec[i].latchedStatusParam, &vec[i].latchedStatus);
    }
    std::sort(vec.begin(), vec.end(), compareByTimestamp);
    int cnt = 1;
    for(unsigned int i=0; i<vec.size(); i++)
    {
        if(vec[i].timestamp <= 0.0 || vec[i].latchedStatus == 0)
        {
            setIntegerParam(vec[i].seqParam, 0);
        }
        else
        {
            setIntegerParam(vec[i].seqParam, cnt);
            cnt++;
        }
    }
}
bool compareByTimestamp(const TimeStampStruct &a, const TimeStampStruct &b) {
    return a.timestamp < b.timestamp;
}

void kriaAsynPortDriver::getFuse(int param, int cavity, SSAFusBits typ) {
    try
    { 
        setIntegerParam(param, boardWrapper->GetFuse(cavity, typ));
    }
    catch(const string& s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const char* s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::runtime_error& re)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::exception& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(SsaLatticeCommException& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(...)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
}

void kriaAsynPortDriver::getPermit(int param, int cavity, SSAOutEnBits typ) {
    try
    { 
        setIntegerParam(param, boardWrapper->GetPermit(cavity, typ));
    }
    catch(const string& s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const char* s)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, s);
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::runtime_error& re)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(const std::exception& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(SsaLatticeCommException& ex)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
    catch(...)
    {
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");
        setIntegerParam(P_KRIA_ALARM_R, 1);
    }
}

void kriaAsynPortDriver::FEPinit() {
    printf("FEP initialisation\n");

    const unsigned c_rtd_spi_data_width_addr = 3;
    const unsigned c_rtd_control_addr = 4;
    const unsigned c_rtd_spi_tx_data_addr = 5;

    // FEP test
    fep_reg = new CAdcCtrl(REG_FEP_DT_NODE); 
    
    //CAdcCtrl fep_reg(REG_FEP_DT_NODE);
    fep_reg->set_reset();
    (*fep_reg)[c_adc_reset_addr] = 0x3;
    (*fep_reg)[c_adc_reset_addr] = 0x1;

    // RTD test
    rtd_reg = new CAdcCtrl(REG_RTD_DT_NODE); 
    //CAdcCtrl rtd_reg(REG_RTD_DT_NODE);
    rtd_reg->set_reset();
    (*rtd_reg)[c_adc_reset_addr] = 0x3;
    (*rtd_reg)[c_adc_reset_addr] = 0x1;

    const uint8_t addr_00h = 0x0;
    const uint8_t SPI_RESET = 1<<0;
    const uint8_t REG_READ_EN = 1<<1;
    const uint16_t cfg_00h_reset = SPI_RESET;
    const uint16_t cfg_00h_read = REG_READ_EN;
    const uint16_t cfg_00h_write = 0;

    const uint8_t addr_01h = 0x1;
    const uint16_t LVDS_RATE_2X = 1<<14;
    const uint16_t INPUT_MODE_8  = 1<<7 | 1<<4 | 1<<2;
    const uint16_t INPUT_MODE_16 = 0<<7 | 1<<4 | 1<<2;
    const uint16_t cfg_01h = LVDS_RATE_2X | INPUT_MODE_16;

    const uint8_t addr_02h = 0x2;

    const uint16_t PAT_MODE_SYNC   = 1<<7;
    const uint16_t PAT_MODE_DESKEW = 2<<7;
    const uint16_t PAT_MODE_CUSTOM = 3<<7;
    const uint16_t PAT_MODE_RAMP   = 7<<7;
    const uint16_t PAT_MODE_TOGGLE = 5<<7;

    const uint16_t PAT_MODE_FCLK_NORMAL = 0<<13 ;
    const uint16_t PAT_MODE_FCLK_DESKEW = 2<<13;
    const uint16_t PAT_MODE_FCLK_CUSTOM = 3<<13;

    const uint16_t cfg_02h = PAT_MODE_DESKEW*0 | PAT_MODE_FCLK_NORMAL;

    const uint8_t addr_03h = 0x3;
    const uint16_t SER_DATA_RATE_14 = 1<<13;
    const uint16_t SER_DATA_RATE_16 = 4<<13;
    const uint16_t cfg_03h = SER_DATA_RATE_14;

    const uint8_t addr_04h = 0x4;
    const uint16_t ADC_RES_14 = 1<<0;

    const uint16_t MSB_FIRST = 1<<4;

    const uint16_t cfg_04h = ADC_RES_14 | MSB_FIRST;

    const uint8_t addr_05h = 0x5;
    const uint16_t CUSTOM_PATTERN = 0xCA04;
    const uint16_t cfg_05h = CUSTOM_PATTERN;

    const uint8_t addr_0Ah = 0xA;
    const uint16_t INIT2_INIT1 = 0x3000;
    const uint16_t cfg_0Ah = INIT2_INIT1;

    const uint8_t addr_43h = 0x43;
    const uint16_t DCLK_DELAY_zero = 0x0;   //0000 =  110 ps
    const uint16_t DCLK_DELAY_p110 = 0x1;   //0001 =  110 ps
    const uint16_t DCLK_DELAY_p220 = 0x2;   //0010 =  220 ps
    const uint16_t DCLK_DELAY_p330 = 0x3;   //0011 =  330 ps
    const uint16_t DCLK_DELAY_p770 = 0x7;   //0111 =  770 ps
    const uint16_t DCLK_DELAY_n770 = 0x8;   //1000 = -770 ps
    const uint16_t DCLK_DELAY_n220 = 0xE;   //1110 = -220 ps
    const uint16_t DCLK_DELAY_n110 = 0xF;   //1111 = -110 ps
    const uint16_t LVDS_DCLK_DELAY_PROG = DCLK_DELAY_p770 << 1;
    const uint16_t cfg_43h = LVDS_DCLK_DELAY_PROG;

    struct addr_cfg {uint8_t addr; uint16_t cfg;};

    struct addr_cfg write_config[] = {
      {addr_0Ah, cfg_0Ah},
      {addr_00h, cfg_00h_reset},
      {addr_00h, cfg_00h_write},
      {addr_01h, cfg_01h},
      {addr_02h, cfg_02h},
      {addr_03h, cfg_03h},
      {addr_04h, cfg_04h},
      {addr_05h, cfg_05h},
      {addr_43h, cfg_43h},
    };

    struct addr_cfg read_config[] = {
      {addr_0Ah, cfg_0Ah},
      {addr_00h, cfg_00h_read},
      {addr_01h, cfg_01h},
      {addr_02h, cfg_02h},
      {addr_03h, cfg_03h},
      {addr_04h, cfg_04h},
      {addr_05h, cfg_05h},
      {addr_43h, cfg_43h},
      {addr_00h, cfg_00h_write},
    };

    const uint16_t  ADC_RESET      = 1<<0;
    const uint16_t  ADC_TX_TRIG    = 1<<1;
    const uint16_t  ADC_SPI_DIG_EN = 1<<4;
    const uint16_t  ADC_S_CPOL     = 1<<5;
    const uint16_t  ADC_S_CPHA     = 1<<6;
    uint16_t adc_ctrl;

    // setting adc spi dig en
    adc_ctrl = ADC_SPI_DIG_EN | ADC_S_CPOL | ADC_S_CPHA;

    (*fep_reg)[c_adc_ctrl_addr] = adc_ctrl;
    (*rtd_reg)[c_adc_ctrl_addr] = adc_ctrl;

    // Generatirng pul on adr reset
    adc_ctrl = ADC_RESET | ADC_SPI_DIG_EN | ADC_S_CPOL | ADC_S_CPHA;

    (*fep_reg)[c_adc_ctrl_addr] = adc_ctrl;
    (*rtd_reg)[c_adc_ctrl_addr] = adc_ctrl;

    adc_ctrl =             ADC_SPI_DIG_EN | ADC_S_CPOL | ADC_S_CPHA;

    (*fep_reg)[c_adc_ctrl_addr] = adc_ctrl;
    (*rtd_reg)[c_adc_ctrl_addr] = adc_ctrl;

    // Transferring adc configuration
    unsigned write_config_length = sizeof(write_config) / sizeof(addr_cfg);
    for(unsigned i = 0; i < write_config_length; ++i)
    {
        uint8_t addr = write_config[i].addr;
        uint16_t cfg = write_config[i].cfg;
        (*fep_reg)[c_adc_tx_addr] = addr << 16 | cfg;
        (*rtd_reg)[c_adc_tx_addr] = addr << 16 | cfg;
    }

    // Checking ADC configuration
    unsigned read_config_length = sizeof(read_config) / sizeof(addr_cfg);
    for(unsigned i = 0; i < read_config_length; ++i)
    {
        uint8_t addr = read_config[i].addr;
        uint16_t cfg = read_config[i].cfg;
        (*fep_reg)[c_adc_tx_addr] = addr << 16 | cfg;
    }

    // Generating pulse on tx_trig
    adc_ctrl = ADC_TX_TRIG | ADC_SPI_DIG_EN | ADC_S_CPOL | ADC_S_CPHA;

    (*fep_reg)[c_adc_ctrl_addr] = adc_ctrl;
    (*rtd_reg)[c_adc_ctrl_addr] = adc_ctrl;

    adc_ctrl =               ADC_SPI_DIG_EN | ADC_S_CPOL | ADC_S_CPHA;

    (*fep_reg)[c_adc_ctrl_addr] = adc_ctrl;
    (*rtd_reg)[c_adc_ctrl_addr] = adc_ctrl;

    // "RESETTING AND RELEASING RESET OF FEP DMA (active low)
    (*fep_reg)[c_adc_reset_addr] = 0x0;
    (*fep_reg)[c_adc_reset_addr] = 0x1;

    // "RESETTING AND RELEASING RESET OF RTD DMA (active low)
    (*rtd_reg)[c_adc_reset_addr] = 0x0;
    (*rtd_reg)[c_adc_reset_addr] = 0x1;

    unsigned spi_data_width = 24;
    unsigned s_rtd_cpol = 1;
    unsigned s_rtd_cpha = 1;
    unsigned s_rtd_slave = 8;
    unsigned en_4v5 = 1;

    (*rtd_reg)[c_rtd_spi_data_width_addr] = spi_data_width;
    (*rtd_reg)[c_rtd_control_addr] = ((en_4v5 & 0x1) << 6) | ((s_rtd_cpol & 0x1) << 5) | ((s_rtd_cpha & 0x1)  << 4) | ((s_rtd_slave & 0xf) << 0);

    unsigned int sample = 1365;

    {
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x000000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x080000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x100000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x180000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x200000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x280000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x300000 |  (sample & 0xfff);
        (*rtd_reg)[c_rtd_spi_tx_data_addr] =  0x380000 |  (sample & 0xfff);
    }

  for(int i=0;i<8; ++i)
  {
      spi_data_width = 16;
      s_rtd_slave = i;
      (*rtd_reg)[c_rtd_spi_data_width_addr] = spi_data_width;
      (*rtd_reg)[c_rtd_control_addr] = ((en_4v5 & 0x1) << 6) | ((s_rtd_cpol & 0x1) << 5) | ((s_rtd_cpha & 0x1)  << 4) | ((s_rtd_slave & 0xf) << 0);

      (*rtd_reg)[c_rtd_spi_tx_data_addr] = 0x4000 ;
  }

    spi_data_width = 24;
    s_rtd_cpol = 1;
    s_rtd_cpha = 1;
    s_rtd_slave = 0;
    en_4v5 = 1;

    (*fep_reg)[c_rtd_spi_data_width_addr] = 24;
    (*fep_reg)[c_rtd_control_addr] =  (1 << 8) | (1 << 7) | ((en_4v5 & 0x1) << 6) | ((s_rtd_cpol & 0x1) << 5) | ((s_rtd_cpha & 0x1)  << 4) | ((s_rtd_slave & 0xf) << 0);

    sample = 0x0;
    {
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x000000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x080000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x100000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x180000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x200000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x280000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x300000 |  (sample & 0xfff);
        (*fep_reg)[c_rtd_spi_tx_data_addr] =  0x380000 |  (sample & 0xfff);
    }
}

void kriaAsynPortDriver::SSAinit() {
    CSsaCtrl reg_ssa(REG_SSA_DT_NODE);
    reg_ssa.set_reset(0);
    reg_ssa.set_reset(1);

    reg_ssa[c_ssa_control_addr] = 0x00;
    reg_ssa[c_ssa_control_addr] = 0xFF;

    reg_ssa[c_ssa_control_addr] = 0x00;
    reg_ssa[c_ssa_control_addr] = 0xF0;
}

void kriaAsynPortDriver::terminateThreads() {
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

kriaAsynPortDriver::~kriaAsynPortDriver() {
    delete fep_reg;
    delete rtd_reg;
    delete boardWrapper;
}
