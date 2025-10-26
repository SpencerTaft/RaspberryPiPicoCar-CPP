#include "lightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <iostream>

LightRunnable::LightRunnable(char* ID)
{
    strcpy(_runnableID, ID);
    _runnableType = LIGHT;

    _lightConfig.pin = DEFAULT_LIGHT_PIN;
    _lightConfig.isOn = DEFAULT_LIGHT_STATE;

    mutex_init(&_configMutex);
}

RunnableType LightRunnable::getType()
{
    return _runnableType;
}

RuntimeExecutionStatus LightRunnable::runtime()
{
    mutex_enter_blocking(&_configMutex);
    std::cout << "LightRunnable runtime" << std::endl;

    setLightOutput();

    mutex_exit(&_configMutex);

    return RuntimeExecutionStatus::SUCCESS;
}

char* LightRunnable::getID()
{
    return &_runnableID[0];
}

bool LightRunnable::setConfig(const char* newConfig)
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

        //setLightOutput();
        setConfigSuccess = true;
    }
    else
    {
        printf("Invalid configuration received");
    }

    mutex_exit(&_configMutex);
    return setConfigSuccess;
}

void LightRunnable::setLightOutput()
{
    gpio_init(_lightConfig.pin);
    gpio_set_dir(_lightConfig.pin, GPIO_OUT);

    if (_lightConfig.isOn)
    {
        gpio_put(_lightConfig.pin, 1);
    }
    else
    {
        gpio_put(_lightConfig.pin, 0);
    }
}









