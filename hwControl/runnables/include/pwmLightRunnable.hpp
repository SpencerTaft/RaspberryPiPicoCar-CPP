#include "runnable.hpp"
#include "pico/mutex.h"

typedef struct PWMLightConfig {
    int pin;
    bool isOn; //todo
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
        constexpr static int MAX_RUNNABLE_ID_LEN = 64;
        constexpr static int DEFAULT_LIGHT_PIN = 6;
        constexpr static bool DEFAULT_LIGHT_STATE = false;

        char _runnableID[MAX_RUNNABLE_ID_LEN];
        RunnableType _runnableType;
        PWMLightConfig _lightConfig;
        mutex_t _configMutex;

        void setLightOutput();
};



