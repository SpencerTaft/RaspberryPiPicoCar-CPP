#include "runnable.hpp"

class LightRunnable : public Runnable {
    public:
        LightRunnable(std::string ID);
        virtual ~LightRunnable();

        virtual RuntimeExecutionStatus runtime();

        virtual std::string getID();

        virtual std::string getConfig();
        virtual bool setConfig(std::string newConfig);
    private:
        std::string _runnableID;
        std::string _currentConfig;
};



