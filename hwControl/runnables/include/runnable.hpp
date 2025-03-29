#pragma once
//#include <nlohmann/json.hpp>

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

        virtual char* getID() = 0;
        virtual RunnableType getType() = 0;

        virtual bool setConfig(char* newConfig) = 0;
};