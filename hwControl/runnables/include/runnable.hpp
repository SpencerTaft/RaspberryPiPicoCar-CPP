#pragma once
#include <string>
#include <nlohmann/json.hpp>

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

        virtual bool setConfig(nlohmann::json newConfig) = 0;
};