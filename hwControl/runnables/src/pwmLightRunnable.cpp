#include "pwmLightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <iostream>

PWMLightRunnable::PWMLightRunnable(char* ID)
{
    strcpy(_runnableID, ID);
    _runnableType = PWMLIGHT;

    _pwmLightConfig.pin = DEFAULT_PWM_LIGHT_PIN;
    _pwmLightConfig.isOn = DEFAULT_PWM_LIGHT_STATE;
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

    if (configJSON.is_object() && configJSON.contains("pin") && configJSON.contains("isOn"))
    {
        _pwmLightConfig.pin = configJSON["pin"];
        _pwmLightConfig.isOn = configJSON["isOn"];

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
    gpio_init(_pwmLightConfig.pin);
    gpio_set_dir(_pwmLightConfig.pin, GPIO_OUT);

    if (_pwmLightConfig.isOn)
    {
        gpio_put(_pwmLightConfig.pin, 1);
    }
    else
    {
        gpio_put(_pwmLightConfig.pin, 0);
    }
}









