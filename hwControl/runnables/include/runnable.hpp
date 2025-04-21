#pragma once

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

        virtual bool setConfig(const char* newConfig) = 0;
};