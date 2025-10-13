#include "runnable.hpp"
#include "pico/mutex.h"

typedef struct PWMLightConfig {
    int pin;
    bool isOn; //enables/disables the light
    bool isRamp; //enables/disables ramping
    int LMax; //max brightness level 0-255
    int rampUpTimeMs; //time to ramp up in milliseconds
} PWMLightConfig;

class PWMLightRunnable : public Runnable {
    public:
        PWMLightRunnable(char* ID);
        virtual ~PWMLightRunnable(){}

        virtual RuntimeExecutionStatus runtime();

        virtual char* getID();
        virtual RunnableType getType();

        virtual bool setConfig(const char* newConfig);
    private:
        constexpr static int MAX_RUNNABLE_ID_LEN = 64; //todo common define
        constexpr static int DEFAULT_PWM_LIGHT_PIN = 6; //TODO CHANGE
        constexpr static bool DEFAULT_PWM_LIGHT_STATE = false;
        constexpr static bool DEFAULT_PWM_LIGHT_RAMP = false;
        constexpr static int DEFAULT_PWM_LIGHT_LMAX = 255;
        constexpr static int DEFAULT_PWM_LIGHT_RAMPUPTIME_MS = 1000;

        char _runnableID[MAX_RUNNABLE_ID_LEN];
        RunnableType _runnableType;
        PWMLightConfig _pwmLightConfig;
        mutex_t _configMutex;

        void setLightOutput();
};



