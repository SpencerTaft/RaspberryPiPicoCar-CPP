#include "lightRunnable.hpp"
#include "pico/stdlib.h"
#include <nlohmann/json.hpp>
#include <iostream>

LightRunnable::LightRunnable(std::string ID)
{
    _runnableID = ID;
    _runnableType = LIGHT;

    _lightConfig.pin = 16;//todo temp
    _lightConfig.isOn = false;//todo temp
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

std::string LightRunnable::getID()
{
    return _runnableID;
}

bool LightRunnable::setConfig(nlohmann::json newConfig)
{
    _lightConfig.pin = newConfig["pin"];
    _lightConfig.isOn = newConfig["isOn"];

    return true;
}









