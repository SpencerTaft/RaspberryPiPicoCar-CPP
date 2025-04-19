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

        virtual bool setConfig(char* newConfig);
    private:
        char _runnableID[64]; //todo max length
        RunnableType _runnableType;
        LightConfig _lightConfig;
        mutex_t _configMutex;

        void setLightOutput();
};



