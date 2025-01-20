#include "lightRunnable.hpp"

LightRunnable::LightRunnable(std::string ID)
{
    _runnableID = ID;
    _runnableType = LIGHT;
}

RunnableType LightRunnable::getType()
{
    return _runnableType;
}

RuntimeExecutionStatus LightRunnable::runtime()
{
    return RuntimeExecutionStatus::SUCCESS;
}

std::string LightRunnable::getID()
{
    return _runnableID;
}

std::string LightRunnable::getConfig()
{
    return _currentConfig;
}
bool LightRunnable::setConfig(std::string newConfig)
{
    _currentConfig = newConfig;
}









