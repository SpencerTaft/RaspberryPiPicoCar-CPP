#include "pwmLightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <iostream>

PWMLightRunnable::PWMLightRunnable(char* ID)
{
    strcpy(_runnableID, ID);
    _runnableType = PWMLIGHT;

    _lightConfig.pin = DEFAULT_LIGHT_PIN;
    _lightConfig.isOn = DEFAULT_LIGHT_STATE;

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

    if (configJSON.is_object() && configJSON.contains("pin") && configJSON.contains("isOn"))
    {
        _lightConfig.pin = configJSON["pin"];
        _lightConfig.isOn = configJSON["isOn"];

        setLightOutput();
        setConfigSuccess = true;
    }
    else
    {
        printf("Invalid configuration received");
    }

    mutex_exit(&_configMutex);
    return setConfigSuccess;
}

void PWMLightRunnable::setLightOutput()
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









