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
    std::cout << "LightRunnable runtime" << std::endl;

    _lightConfig.isOn = !_lightConfig.isOn; //temporary toggling the pin

    if (_lightConfig.isOn)
    {
        gpio_put(_lightConfig.pin, 1);
    }
    else
    {
        gpio_put(_lightConfig.pin, 0);
    }

    return RuntimeExecutionStatus::SUCCESS;
}

char* LightRunnable::getID()
{
    return &_runnableID[0];
}

bool LightRunnable::setConfig(char* newConfig)
{
    mutex_enter_blocking(&_configMutex);
    printf("RECEIVED CONFIG %s\n", newConfig);

    nlohmann::json configJSON = nlohmann::json::parse(newConfig);

    if (configJSON.is_object() && configJSON.contains("pin") && configJSON.contains("isOn"))
    {
        _lightConfig.pin = configJSON["pin"];
        _lightConfig.isOn = configJSON["isOn"];

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









