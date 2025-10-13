#pragma once
#include "lightRunnable.hpp"
#include "pico/mutex.h"

typedef struct PWMLightConfig {
    bool isRamp; //enables/disables ramping
    int LMax; //max brightness level 0-255
    int rampUpTimeMs; //time to ramp up in milliseconds
} PWMLightConfig;

class PWMLightRunnable : public LightRunnable {
    public:
        PWMLightRunnable(char* ID);
        virtual ~PWMLightRunnable(){}

        virtual RuntimeExecutionStatus runtime();

        virtual char* getID();
        virtual RunnableType getType();

        virtual bool setConfig(const char* newConfig);
    protected:
        constexpr static bool DEFAULT_PWM_LIGHT_RAMP = false;
        constexpr static int DEFAULT_PWM_LIGHT_LMAX = 255;
        constexpr static int DEFAULT_PWM_LIGHT_RAMPUPTIME_MS = 1000;

        char _runnableID[MAX_RUNNABLE_ID_LEN];
        RunnableType _runnableType;
        PWMLightConfig _pwmLightConfig;
        mutex_t _configMutex;
};



