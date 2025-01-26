#include "runnable.hpp"

typedef struct LightConfig {
    int pin;
    bool isOn;
} LightConfig;

class LightRunnable : public Runnable {
    public:
        LightRunnable(std::string ID);
        virtual ~LightRunnable(){}

        virtual RuntimeExecutionStatus runtime();

        virtual std::string getID();
        virtual RunnableType getType();

        virtual bool setConfig(nlohmann::json newConfig);
    private:
        std::string _runnableID;
        RunnableType _runnableType;
        LightConfig _lightConfig;
};



