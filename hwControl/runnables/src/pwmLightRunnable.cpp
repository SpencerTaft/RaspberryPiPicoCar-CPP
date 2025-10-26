#include "pwmLightRunnable.hpp"
#include <pico/stdlib.h>
#include <cstring>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <iostream>
//todo new includes to resolve
#include <hardware/pwm.h>

PWMLightRunnable::PWMLightRunnable(char *ID) : LightRunnable(ID)
{
    strcpy(_runnableID, ID);
    _runnableType = PWMLIGHT; //overwrites base class constructor value

    //default config values
    _pwmLightConfig.isRamp = DEFAULT_PWM_LIGHT_RAMP;
    _pwmLightConfig.LMax = DEFAULT_PWM_LIGHT_LMAX;
    _pwmLightConfig.rampUpTimeMs = DEFAULT_PWM_LIGHT_RAMPUPTIME_MS;

    //set private ramp parameters to defaults
    _lastRampUpdateTimeMs = 0;
    int _lastRampLevel = 0;
    bool _isLastRampDown = false;

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

    if (!_pwmLightConfig.isRamp)
    {
        // simple on/off
        setLightOutput();
    }
    else
    {
        // ramping logic
        calculateRamp();
    }

    mutex_exit(&_configMutex);

    return RuntimeExecutionStatus::SUCCESS;
}

char *PWMLightRunnable::getID()
{
    return &_runnableID[0];
}

bool PWMLightRunnable::setConfig(const char *newConfig)
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

        setConfigSuccess = true;
    }
    else
    {
        printf("Invalid configuration received");
    }

    mutex_exit(&_configMutex);
    return setConfigSuccess;
}

void PWMLightRunnable::calculateRamp()
{
    //todo get current time, and use absolute value of time difference
    int currentTimeMs = to_ms_since_boot(get_absolute_time());
    int rampTimeDeltaMs = std::abs(currentTimeMs - _lastRampUpdateTimeMs);
    
    int rampDeltaL = rampTimeDeltaMs / _pwmLightConfig.rampUpTimeMs; //percentage of ramp completed since last update
    rampDeltaL = rampDeltaL * (_pwmLightConfig.LMax); //convert to L level change

    if (_isLastRampDown)
    {
        _lastRampLevel -= rampDeltaL;
        if (_lastRampLevel < 0)
        {
            _lastRampLevel = 0;
            _isLastRampDown = false; //switch direction
        }
    }
    else
    {
        _lastRampLevel += rampDeltaL;
        if (_lastRampLevel > _pwmLightConfig.LMax)
        {
            _lastRampLevel = _pwmLightConfig.LMax;
            _isLastRampDown = true; //switch direction
        }
    }

    _lastRampUpdateTimeMs = currentTimeMs;

    setPWMLightOutput(_lastRampLevel);
}

void PWMLightRunnable::setPWMLightOutput(int level)
{
    //TODO first pass of copilot, manual adjustments todo
    gpio_set_function(_lightConfig.pin, GPIO_FUNC_PWM); // Set pin to PWM function
    uint slice_num = pwm_gpio_to_slice_num(_lightConfig.pin); // Get PWM slice for the pin

    pwm_config config = pwm_get_default_config(); // Get default PWM config
    pwm_config_set_clkdiv(&config, 4.0f); // Set clock divider for PWM frequency (clock speed / divider)
    pwm_config_set_wrap(&config, 65535); // Set wrap value for 16-bit resolution (16 bit resolution)
    pwm_init(slice_num, &config, true); // Initialize PWM with the config

    // Scale LMax to 16-bit range (expects 0-100, then multiplies by 65535.  Need to update, since resolution is lost)
    //https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pwm_1ga279d1ba7dcc8f19619f389317efb41fd
    uint16_t level = static_cast<uint16_t>((_pwmLightConfig.LMax / 100.0f) * 65535); 
    pwm_set_gpio_level(_lightConfig.pin, level); // Set PWM level
}
