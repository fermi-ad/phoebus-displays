#include "kriaAsynPortDriver.h"

#define COMMA , 

#define ANALOG_GETTERS_CAV(NAME, TYPE, CAVITY_NO) {\
    std::pair <uint32_t, double> p1;\
    std::pair <bool, uint32_t> p2;\
    try\
    {\
       p1 = boardWrapper-> Get ## TYPE(CAVITY ## CAVITY_NO );\
       setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _VALUE_RAW_R, p1.first);\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    { \
        epicsFloat64 _scale = 0 ;\
        epicsFloat64 _offset = 0 ;\
        \
        getDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_SCALE_S , &_scale);\
        getDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_OFFSET_S, &_offset);\
        \
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _VALUE_SCAL_R, _scale * p1.first + _offset); \
        \
        try\
        {\
            p1 = boardWrapper->Get ## TYPE ## ThresholdLo(CAVITY ## CAVITY_NO);\
            setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_LOW_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_LOW_RAW_R, p1.first);\
            \
            p1 = boardWrapper->Get ## TYPE ## ThresholdHi(CAVITY ## CAVITY_NO);\
            setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_HIGH_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_HIGH_RAW_R, p1.first);\
        }\
        catch(const string& s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const char* s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::runtime_error& re)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::exception& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(SsaLatticeCommException& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(...)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
    } \
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _MASKING_R, boardWrapper->Get ## TYPE ## Mask(CAVITY ## CAVITY_NO));\
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_SCALE_R, boardWrapper->Get ## TYPE ## Scaling(CAVITY ## CAVITY_NO));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _LATCH_STATUS_R, boardWrapper->Get ## TYPE ## Interlock(CAVITY ## CAVITY_NO));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_LOW_R, boardWrapper->Get ## TYPE ## TripLoCount(CAVITY ## CAVITY_NO));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_HIGH_R, boardWrapper->Get ## TYPE ## TripHiCount(CAVITY ## CAVITY_NO));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_LOW_TOTAL_R, boardWrapper->Get ## TYPE ## TripLoSpan(CAVITY ## CAVITY_NO));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_HIGH_TOTAL_R, boardWrapper->Get ## TYPE ## TripHiSpan(CAVITY ## CAVITY_NO));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}

#define ANALOG_GETTERS_CAV_WITH_MASK(NAME, TYPE, CAVITY_NO, MASK) {\
    std::pair <uint32_t, double> p1;\
    std::pair <bool, uint32_t> p2;\
    try\
    {\
        p1 = boardWrapper-> Get ## TYPE(CAVITY ## CAVITY_NO COMMA MASK);\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _VALUE_RAW_R, p1.first);\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    {\
        epicsFloat64 _scale = 0 ;\
        epicsFloat64 _offset = 0 ;\
        \
        getDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_SCALE_S , &_scale);\
        getDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_OFFSET_S, &_offset);\
        \
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _VALUE_SCAL_R, _scale * p1.first + _offset); \
        \
        try\
        {\
            p1 = boardWrapper->Get ## TYPE ## ThresholdLo(CAVITY ## CAVITY_NO COMMA MASK);\
            setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_LOW_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_LOW_RAW_R, p1.first);\
            \
            p1 = boardWrapper->Get ## TYPE ## ThresholdHi(CAVITY ## CAVITY_NO COMMA MASK);\
            setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_HIGH_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_HIGH_RAW_R, p1.first);\
        }\
        catch(const string& s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const char* s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::runtime_error& re)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::exception& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(SsaLatticeCommException& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(...)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
    } \
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _MASKING_R, boardWrapper->Get ## TYPE ## Mask(CAVITY ## CAVITY_NO COMMA MASK));\
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_SCALE_R, boardWrapper->Get ## TYPE ## Scaling(CAVITY ## CAVITY_NO COMMA MASK));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _LATCH_STATUS_R, boardWrapper->Get ## TYPE ## Interlock(CAVITY ## CAVITY_NO COMMA MASK));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_LOW_R, boardWrapper->Get ## TYPE ## TripLoCount(CAVITY ## CAVITY_NO COMMA MASK));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_HIGH_R, boardWrapper->Get ## TYPE ## TripHiCount(CAVITY ## CAVITY_NO COMMA MASK));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_LOW_TOTAL_R, boardWrapper->Get ## TYPE ## TripLoSpan(CAVITY ## CAVITY_NO COMMA MASK));\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _THRESHOLD_CNT_HIGH_TOTAL_R, boardWrapper->Get ## TYPE ## TripHiSpan(CAVITY ## CAVITY_NO COMMA MASK));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}

#define ANALOG_GETTERS(NAME, TYPE) {\
    std::pair <uint32_t, double> p1;\
    std::pair <bool, uint32_t> p2;\
    try\
    {\
        p1 = boardWrapper-> Get ## TYPE(NAME);\
        setIntegerParam(P_ ## NAME ## _VALUE_RAW_R, p1.first);\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    { \
        epicsFloat64 _scale = 0 ;\
        epicsFloat64 _offset = 0 ;\
        \
        getDoubleParam(P_ ## NAME ##_THRESHOLD_SCALE_S , &_scale);\
        getDoubleParam(P_ ## NAME ##_THRESHOLD_OFFSET_S, &_offset);\
        setDoubleParam(P_ ## NAME ## _VALUE_SCAL_R, _scale * p1.first + _offset); \
        \
        try\
        {\
            p1 = boardWrapper->Get ## TYPE ## ThresholdLo(NAME);\
            setDoubleParam(P_ ## NAME ## _THRESHOLD_LOW_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _THRESHOLD_LOW_RAW_R, p1.first);\
            \
            p1 = boardWrapper->Get ## TYPE ## ThresholdHi(NAME);\
            setDoubleParam(P_ ## NAME ## _THRESHOLD_HIGH_SCAL_R, _scale * p1.first + _offset );\
            setIntegerParam(P_ ## NAME ## _THRESHOLD_HIGH_RAW_R, p1.first);\
        }\
        catch(const string& s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const char* s)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, s);\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::runtime_error& re)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(const std::exception& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(SsaLatticeCommException& ex)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
        catch(...)\
        {\
            setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
            setIntegerParam(P_KRIA_ALARM_R, 1);\
        }\
    } \
    try\
    {\
        setIntegerParam(P_ ## NAME ## _MASKING_R, boardWrapper->Get ## TYPE ## Mask(NAME));\
        setDoubleParam(P_ ## NAME ## _THRESHOLD_SCALE_R, boardWrapper->Get ## TYPE ## Scaling(NAME));\
        setIntegerParam(P_ ## NAME ## _LATCH_STATUS_R, boardWrapper->Get ## TYPE ## Interlock(NAME));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    try\
    {\
        setIntegerParam(P_ ## NAME ## _THRESHOLD_CNT_LOW_R, boardWrapper->Get ## TYPE ## TripLoCount(NAME));\
        setIntegerParam(P_ ## NAME ## _THRESHOLD_CNT_HIGH_R, boardWrapper->Get ## TYPE ## TripHiCount(NAME));\
        setIntegerParam(P_ ## NAME ## _THRESHOLD_CNT_LOW_TOTAL_R, boardWrapper->Get ## TYPE ## TripLoSpan(NAME));\
        setIntegerParam(P_ ## NAME ## _THRESHOLD_CNT_HIGH_TOTAL_R, boardWrapper->Get ## TYPE ## TripHiSpan(NAME));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}


#define BINARY_GETTERS(NAME, TYPE, CAVITY_NO) {\
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _CURRENT_STATUS_R, boardWrapper->Get ## TYPE( CAVITY ## CAVITY_NO));\
        p1 = boardWrapper->Get ## TYPE ## WithTimeStamp( CAVITY ## CAVITY_NO);\
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _TIMESTAMP_R, (epicsFloat64)p1.second);\
        setIntegerParam(P_## NAME ## _C ## CAVITY_NO ## _LATCH_STATUS_R, p1.first);\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}

#define BINARY_GETTERS(NAME, TYPE, CAVITY_NO) {\
    try\
    {\
        setIntegerParam(P_ ## NAME ## _C ## CAVITY_NO ## _CURRENT_STATUS_R, boardWrapper->Get ## TYPE( CAVITY ## CAVITY_NO));\
        p1 = boardWrapper->Get ## TYPE ## WithTimeStamp( CAVITY ## CAVITY_NO);\
        setDoubleParam(P_ ## NAME ## _C ## CAVITY_NO ## _TIMESTAMP_R, (epicsFloat64)p1.second);\
        setIntegerParam(P_## NAME ## _C ## CAVITY_NO ## _LATCH_STATUS_R, p1.first);\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}

#define BINARY_GETTERS_SSA_READY(CAVITY_NO) {\
    try\
    {\
        setIntegerParam(P_SSA_C ## CAVITY_NO ## _CURRENT_STATUS_R, boardWrapper->GetSSAReady(CAVITY ## CAVITY_NO));\
        setIntegerParam(P_SSA_C ## CAVITY_NO ## _MASKING_R, boardWrapper->GetSSAReadyMask(CAVITY ## CAVITY_NO));\
    }\
    catch(const string& s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const char* s)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, s);\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::runtime_error& re)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, re.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(const std::exception& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, ex.what());\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(SsaLatticeCommException& ex)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "SSA communication lattice error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
    catch(...)\
    {\
        setStringParam(P_KRIA_ALARM_MSG_R, "Unknown fatal error");\
        setIntegerParam(P_KRIA_ALARM_R, 1);\
    }\
}


#define SLEEP_TIME 1


void paramsReadout(void *drvPvt) {
    kriaAsynPortDriver *pPvt = (kriaAsynPortDriver *)drvPvt;
    pPvt->paramsReadout();
}


void kriaAsynPortDriver::paramsReadout(void) {
    epicsMutexMustLock(terminateWorkersLock);

    while( terminateWorkers == 0 ) {
        epicsMutexUnlock(terminateWorkersLock);
        epicsThreadSleep(SLEEP_TIME);
        lock();

        std::pair <bool, uint32_t> p1;
        // Binary part
        // SSA
        BINARY_GETTERS_SSA_READY(1);
        BINARY_GETTERS_SSA_READY(2);
        BINARY_GETTERS_SSA_READY(3);
        BINARY_GETTERS_SSA_READY(4);

        // CS
        BINARY_GETTERS(PSP, PSP, 1);
        BINARY_GETTERS(PSP, PSP, 2);
        BINARY_GETTERS(PSP, PSP, 3);
        BINARY_GETTERS(PSP, PSP, 4);

        BINARY_GETTERS(CAS, CouplerAirFlow, 1);
        BINARY_GETTERS(CAS, CouplerAirFlow, 2);
        BINARY_GETTERS(CAS, CouplerAirFlow, 3);
        BINARY_GETTERS(CAS, CouplerAirFlow, 4);

        BINARY_GETTERS(CVP, CouplerVacuum, 1);
        BINARY_GETTERS(CVP, CouplerVacuum, 2);
        BINARY_GETTERS(CVP, CouplerVacuum, 3);
        BINARY_GETTERS(CVP, CouplerVacuum, 4);

        try
        {
            setIntegerParam(P_HE_LEVEL_CURRENT_STATUS_R, boardWrapper->GetHePressure());
            setIntegerParam(P_HE_LEVEL_MASKING_R, boardWrapper->GetHePressureMask());
            p1 = boardWrapper->GetHePressureWithTimeStamp();
            setDoubleParam(P_HE_LEVEL_TIMESTAMP_R, (epicsFloat64)p1.second);
            setIntegerParam(P_HE_LEVEL_LATCH_STATUS_R, p1.first);
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

        // Permits
        getPermit(P_LLRF_PERM_C1_STATUS_R, CAVITY1, SSAOutEnBits::LLRFPermit);
        getPermit(P_LLRF_PERM_C2_STATUS_R, CAVITY2, SSAOutEnBits::LLRFPermit);
        getPermit(P_LLRF_PERM_C3_STATUS_R, CAVITY3, SSAOutEnBits::LLRFPermit);
        getPermit(P_LLRF_PERM_C4_STATUS_R, CAVITY4, SSAOutEnBits::LLRFPermit);
        getPermit(P_SSA_PERM_C1_STATUS_R, CAVITY1, SSAOutEnBits::SSAPermitPermit);
        getPermit(P_SSA_PERM_C2_STATUS_R, CAVITY2, SSAOutEnBits::SSAPermitPermit);
        getPermit(P_SSA_PERM_C3_STATUS_R, CAVITY3, SSAOutEnBits::SSAPermitPermit);
        getPermit(P_SSA_PERM_C4_STATUS_R, CAVITY4, SSAOutEnBits::SSAPermitPermit);
        getPermit(P_SSADC_PERM_C1_STATUS_R, CAVITY1, SSAOutEnBits::SSADCPermit);
        getPermit(P_SSADC_PERM_C2_STATUS_R, CAVITY2, SSAOutEnBits::SSADCPermit);
        getPermit(P_SSADC_PERM_C3_STATUS_R, CAVITY3, SSAOutEnBits::SSADCPermit);
        getPermit(P_SSADC_PERM_C4_STATUS_R, CAVITY4, SSAOutEnBits::SSADCPermit);
        getPermit(P_MPS_PERM_C1_STATUS_R, CAVITY1, SSAOutEnBits::MPS);
        getPermit(P_MPS_PERM_C2_STATUS_R, CAVITY2, SSAOutEnBits::MPS);
        getPermit(P_MPS_PERM_C3_STATUS_R, CAVITY3, SSAOutEnBits::MPS);
        getPermit(P_MPS_PERM_C4_STATUS_R, CAVITY4, SSAOutEnBits::MPS);

        // RTD High Voltage Bias
        ANALOG_GETTERS_CAV(HVVOLT, HVCplBiasVolt, 1);
        ANALOG_GETTERS_CAV(HVVOLT, HVCplBiasVolt, 2);
        ANALOG_GETTERS_CAV(HVVOLT, HVCplBiasVolt, 3);
        ANALOG_GETTERS_CAV(HVVOLT, HVCplBiasVolt, 4);

        // RTD High Voltage Bias
        ANALOG_GETTERS_CAV(HVCURR, HVCplBiasCurr, 1);
        ANALOG_GETTERS_CAV(HVCURR, HVCplBiasCurr, 2);
        ANALOG_GETTERS_CAV(HVCURR, HVCplBiasCurr, 3);
        ANALOG_GETTERS_CAV(HVCURR, HVCplBiasCurr, 4);

        ANALOG_GETTERS_CAV_WITH_MASK(RTD1, Temperature, 1, RTD1);
        ANALOG_GETTERS_CAV_WITH_MASK(RTD1, Temperature, 2, RTD1);
        ANALOG_GETTERS_CAV_WITH_MASK(RTD1, Temperature, 3, RTD1);
        ANALOG_GETTERS_CAV_WITH_MASK(RTD1, Temperature, 4, RTD1);

        // NIRP
        ANALOG_GETTERS(NIRP1, NIRP);
        ANALOG_GETTERS(NIRP2, NIRP);
        ANALOG_GETTERS(NIRP3, NIRP);
        ANALOG_GETTERS(NIRP4, NIRP);
        ANALOG_GETTERS(NIRP5, NIRP);
        ANALOG_GETTERS(NIRP6, NIRP);

        // Fuses
	getFuse(P_LLRF_PERM_C1_FUSE_STATUS_R, CAVITY1, SSAFusBits::LLRFFus);
	getFuse(P_LLRF_PERM_C2_FUSE_STATUS_R, CAVITY2, SSAFusBits::LLRFFus);
	getFuse(P_LLRF_PERM_C3_FUSE_STATUS_R, CAVITY3, SSAFusBits::LLRFFus);
	getFuse(P_LLRF_PERM_C4_FUSE_STATUS_R, CAVITY4, SSAFusBits::LLRFFus);

	getFuse(P_SSA_PERM_C1_FUSE_STATUS_R, CAVITY1, SSAFusBits::SSAPermitFus);
	getFuse(P_SSA_PERM_C2_FUSE_STATUS_R, CAVITY2, SSAFusBits::SSAPermitFus);
	getFuse(P_SSA_PERM_C3_FUSE_STATUS_R, CAVITY3, SSAFusBits::SSAPermitFus);
	getFuse(P_SSA_PERM_C4_FUSE_STATUS_R, CAVITY4, SSAFusBits::SSAPermitFus);

	getFuse(P_SSADC_PERM_C4_FUSE_STATUS_R, CAVITY1, SSAFusBits::SSADCFus);
	getFuse(P_SSADC_PERM_C4_FUSE_STATUS_R, CAVITY2, SSAFusBits::SSADCFus);
	getFuse(P_SSADC_PERM_C4_FUSE_STATUS_R, CAVITY3, SSAFusBits::SSADCFus);
	getFuse(P_SSADC_PERM_C4_FUSE_STATUS_R, CAVITY4, SSAFusBits::SSADCFus);

	getFuse(P_MPS_PERM_C1_FUSE_STATUS_R, CAVITY1, SSAFusBits::MPSFus);
	getFuse(P_MPS_PERM_C2_FUSE_STATUS_R, CAVITY2, SSAFusBits::MPSFus);
	getFuse(P_MPS_PERM_C3_FUSE_STATUS_R, CAVITY3, SSAFusBits::MPSFus);
	getFuse(P_MPS_PERM_C4_FUSE_STATUS_R, CAVITY4, SSAFusBits::MPSFus);

        // from wile loop
	// rtd
        setIntegerParam(P_HVVOLT_C1_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+0])); 
        setIntegerParam(P_HVVOLT_C2_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+1])); 
        setIntegerParam(P_HVVOLT_C3_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+2])); 
        setIntegerParam(P_HVVOLT_C4_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+3])); 
        setIntegerParam(P_HVCURR_C1_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+4])); 
        setIntegerParam(P_HVCURR_C2_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+5])); 
        setIntegerParam(P_HVCURR_C3_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+6])); 
        setIntegerParam(P_HVCURR_C4_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+7])); 
        setIntegerParam(P_RTD1_C1_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+8])); 
        setIntegerParam(P_RTD1_C2_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+9])); 
        setIntegerParam(P_RTD1_C3_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+10])); 
        setIntegerParam(P_RTD1_C4_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+11])); 
        setIntegerParam(P_RTD2_C1_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+12])); 
        setIntegerParam(P_RTD2_C2_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+13])); 
        setIntegerParam(P_RTD2_C3_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+14])); 
        setIntegerParam(P_RTD2_C4_RTD_READOUT_R, (int16_t)((uint16_t)(*rtd_reg)[13+15])); 
	// fep
	setIntegerParam(P_FEP1_C1_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+0])); 
	setIntegerParam(P_FEP1_C2_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+1]));  
	setIntegerParam(P_FEP1_C3_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+2]));  
	setIntegerParam(P_FEP1_C4_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+3]));
	setIntegerParam(P_FEP2_C1_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+4])); 
	setIntegerParam(P_FEP2_C2_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+5]));  
	setIntegerParam(P_FEP2_C3_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+6]));  
	setIntegerParam(P_FEP2_C4_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+7]));
	setIntegerParam(P_NIRP1_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+8]));
	setIntegerParam(P_NIRP2_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+9]));
	setIntegerParam(P_NIRP3_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+10]));
	setIntegerParam(P_NIRP4_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+11]));
	setIntegerParam(P_NIRP5_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+12]));
	setIntegerParam(P_NIRP6_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+13]));
	setIntegerParam(P_NIRP_SPARE1_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+14]));
	setIntegerParam(P_NIRP_SPARE2_FEP_READOUT_R, (int16_t)((uint16_t)(*fep_reg)[13+15]));

        //markTimestamps();
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
