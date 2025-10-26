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

    if (configJSON.is_object())
    {
        if (configJSON.contains(PIN))
        {
            _lightConfig.pin = configJSON[PIN];
        }
        if (configJSON.contains(IS_ON))
        {
            _lightConfig.isOn = configJSON[IS_ON];
        }
        if (configJSON.contains(IS_RAMP))
        {
            _pwmLightConfig.isRamp = configJSON[IS_RAMP];
        }
        if (configJSON.contains(LMAX))
        {
            _pwmLightConfig.LMax = configJSON[LMAX];
        }
        if (configJSON.contains(RAMP_UP_TIME_MS))
        {
            _pwmLightConfig.rampUpTimeMs = configJSON[RAMP_UP_TIME_MS];
        }

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










