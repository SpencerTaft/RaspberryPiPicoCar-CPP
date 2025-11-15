#pragma once
#include "lightRunnable.hpp"
#include "pico/mutex.h"

typedef struct PWMLightConfig {
    bool isRamp; //enables/disables ramping
    int LMax; //max brightness level 0-100%
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

        const char* IS_RAMP = "isRamp";
        const char* LMAX = "LMax";
        const char* RAMP_UP_TIME_MS = "rampUpTimeMs";

    protected:
        constexpr static bool DEFAULT_PWM_LIGHT_RAMP = true;
        constexpr static int DEFAULT_PWM_LIGHT_LMAX = 100;
        constexpr static int DEFAULT_PWM_LIGHT_RAMPUPTIME_MS = 500;

        char _runnableID[MAX_RUNNABLE_ID_LEN];
        PWMLightConfig _pwmLightConfig;

        void calculateRamp();
        void setPWMLightOutput(int level);

    private:
        //private ramp parameters
        int _rampLastUpdateTimeMs;
        float _rampLevel;
        bool _isLastRampDown;
};



