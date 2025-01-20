#pragma once
#include <string>

typedef enum RuntimeExecutionStatus {
    SUCCESS,
    FAILURE,
    TIMEOUT
};

typedef enum RunnableType {
    LIGHT,
    PWMLIGHT
};

class Runnable 
{
    public:
        virtual ~Runnable(){}
        
        virtual RuntimeExecutionStatus runtime() = 0;

        virtual std::string getID() = 0;
        virtual RunnableType getType() = 0;

        virtual std::string getConfig() = 0;
        virtual bool setConfig(std::string newConfig) = 0;
};