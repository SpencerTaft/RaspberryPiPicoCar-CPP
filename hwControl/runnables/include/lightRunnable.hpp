#include "runnable.hpp"

typedef struct LightConfig {
    uint8_t pin;
    bool isOn;
} LightConfig;

class LightRunnable : public Runnable {
    public:
        LightRunnable(std::string ID);
        virtual ~LightRunnable(){}

        virtual RuntimeExecutionStatus runtime();

        virtual std::string getID();
        virtual RunnableType getType();

        virtual std::string getConfig();
        virtual bool setConfig(std::string newConfig);
    private:
        std::string _runnableID;
        RunnableType _runnableType;
        std::string _currentConfig;
        LightConfig _lightConfig;
};



