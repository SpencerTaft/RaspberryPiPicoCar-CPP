#include "pwmLightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <iostream>

//TODO NEXT TIME combine with lightRunnable.cpp, inheritance, use lightConfig for the basic stuff

PWMLightRunnable::PWMLightRunnable(char* ID) : LightRunnable(ID)
{
    strcpy(_runnableID, ID);
    _runnableType = PWMLIGHT;

    _pwmLightConfig.isRamp = DEFAULT_PWM_LIGHT_RAMP;
    _pwmLightConfig.LMax = DEFAULT_PWM_LIGHT_LMAX;
    _pwmLightConfig.rampUpTimeMs = DEFAULT_PWM_LIGHT_RAMPUPTIME_MS;

    mutex_init(&_configMutex);
}

RunnableType PWMLightRunnable::getType()
{
    return _runnableType;
}

RuntimeExecutionStatus PWMLightRunnable::runtime()
{
    mutex_enter_blocking(&_configMutex);
    std::cout << "PWM LightRunnable runtime" << std::endl;

    setLightOutput();

    mutex_exit(&_configMutex);

    return RuntimeExecutionStatus::SUCCESS;
}

char* PWMLightRunnable::getID()
{
    return &_runnableID[0];
}

bool PWMLightRunnable::setConfig(const char* newConfig)
{
    bool setConfigSuccess = false;

    mutex_enter_blocking(&_configMutex);

    nlohmann::json configJSON = nlohmann::json::parse(newConfig);

    if (configJSON.is_object() && configJSON.contains("pin") && configJSON.contains("isOn")) //todo update one value at a time
    {
        _lightConfig.pin = configJSON["pin"];
        _lightConfig.isOn = configJSON["isOn"];
        _pwmLightConfig.isRamp = configJSON["isRamp"];
        _pwmLightConfig.LMax = configJSON["LMax"];
        _pwmLightConfig.rampUpTimeMs = configJSON["rampUpTimeMs"];

        //setLightOutput();//todo rem
        setConfigSuccess = true;
    }
    else
    {
        printf("Invalid configuration received");
    }

    mutex_exit(&_configMutex);
    return setConfigSuccess;
}










