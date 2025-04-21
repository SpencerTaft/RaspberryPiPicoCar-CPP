#include "runnable.hpp"
#include "pico/mutex.h"

typedef struct LightConfig {
    int pin;
    bool isOn;
} LightConfig;

class LightRunnable : public Runnable {
    public:
        LightRunnable(char* ID);
        virtual ~LightRunnable(){}

        virtual RuntimeExecutionStatus runtime();

        virtual char* getID();
        virtual RunnableType getType();

        virtual bool setConfig(const char* newConfig);
    private:
        constexpr static int MAX_RUNNABLE_ID_LEN = 64;
        constexpr static int DEFAULT_LIGHT_PIN = 16;
        constexpr static bool DEFAULT_LIGHT_STATE = false;

        char _runnableID[MAX_RUNNABLE_ID_LEN];
        RunnableType _runnableType;
        LightConfig _lightConfig;
        mutex_t _configMutex;

        void setLightOutput();
};



