#include "lightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <iostream>

LightRunnable::LightRunnable(char* ID)
{
    strcpy(_runnableID, ID);
    _runnableType = LIGHT;

    _lightConfig.pin = 16;//todo temp
    _lightConfig.isOn = false;//todo temp

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

    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);

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
    mutex_enter_blocking(&_configMutex);
    printf("RECEIVED CONFIG %s\n", newConfig);

    nlohmann::json configJSON = nlohmann::json::parse(newConfig);

    if (configJSON.is_object() && configJSON.contains("pin") && configJSON.contains("isOn"))
    {
        _lightConfig.pin = configJSON["pin"];
        _lightConfig.isOn = configJSON["isOn"];

        gpio_init(_lightConfig.pin);
        gpio_set_dir(_lightConfig.pin, GPIO_OUT);

        setLightOutput();

        mutex_exit(&_configMutex);
        return true;
    }
    else
    {
        mutex_exit(&_configMutex);
        std::cout << "Invalid configuration received" << std::endl;
        return false;
    }
}

void LightRunnable::setLightOutput()
{
    if (_lightConfig.isOn)
    {
        gpio_put(_lightConfig.pin, 1);
    }
    else
    {
        gpio_put(_lightConfig.pin, 0);
    }
}









