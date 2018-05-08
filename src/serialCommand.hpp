#ifndef SERIAL_COMMAND_HPP
#define SERIAL_COMMAND_HPP

#include <Cmd.h>
#include <Arduino.h>
#include <parameter.hpp>
#include <torpedoControl.hpp>
#include <batteryStatus.hpp>
#include <pressureStatus.hpp>

void serialArmTorpedo(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            armTorpedo(TORPEDO_0);
        }
        else if(argValue == 1)
        {
            armTorpedo(TORPEDO_1);
        }
        else
        {
            s->println("Invalid torpedo number");
        }
    }
    else
    {
        s->println("Please specify torpedo number");
    }
}

void serialDisarmTorpedo(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            disarmTorpedo(TORPEDO_0);
        }
        else if(argValue == 1)
        {
            disarmTorpedo(TORPEDO_1);
        }
        else
        {
            s->println("Invalid torpedo number");
        }
    }
    else
    {
        s->println("Please specify torpedo number");
    }
}

void serialFireTorpedo(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            requestLaunch(TORPEDO_0);
        }
        else if(argValue == 1)
        {
            requestLaunch(TORPEDO_1);
        }
        else
        {
            s->println("Invalid torpedo number");
        }
    }
    else
    {
        s->println("Please specify torpedo number");
    }
}

void serialSetDemo(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            boardConfig[PARAM_INDEX_DEMO_MODE].paramValue = 0;
        }
        else if(argValue == 1)
        {
            boardConfig[PARAM_INDEX_DEMO_MODE].paramValue = 1;
        }
        else
        {
            s->println("Invalid value. Use 0 to disable, 1 to enable.");
        }
    }
    else
    {
        s->println("Please specify demo mode value. Use 0 to disable, 1 to enable.");
    }
}

void serialPrintParams(int arg_cnt, char **args)
{
    printParameters();
}

void serialResetToDefaultParams(int arg_cnt, char **args)
{
    resetConfig();
}

void serialSaveParams(int arg_cnt, char **args)
{
    saveConfig();
}

void serialDisplayVoltages(int arg_cnt, char **args)
{
    displayVoltages();
}

void serialEnableLowVoltIndicator(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            boardConfig[PARAM_INDEX_ENABLE_LOW_VOLT_INDICATOR].paramValue = 0;
        }
        else if(argValue == 1)
        {
            boardConfig[PARAM_INDEX_ENABLE_LOW_VOLT_INDICATOR].paramValue = 1;
        }
        else
        {
            s->println("Invalid value. Use 0 to disable, 1 to enable.");
        }
    }
    else
    {
        s->println("Please specify valid value. Use 0 to disable, 1 to enable.");
    }
}

void serialEnablePressureAlert(int arg_cnt, char **args)
{
    int argValue;
    Stream *s = cmdGetStream();
    if(arg_cnt > 1)
    {
        argValue = cmdStr2Num(args[1], 10);
        if(argValue == 0)
        {
            boardConfig[PARAM_INDEX_ENABLE_PRESSURE_ALERT].paramValue = 0;
        }
        else if(argValue == 1)
        {
            boardConfig[PARAM_INDEX_ENABLE_PRESSURE_ALERT].paramValue = 1;
        }
        else
        {
            s->println("Invalid value. Use 0 to disable, 1 to enable.");
        }
    }
    else
    {
        s->println("Please specify valid value. Use 0 to disable, 1 to enable.");
    }

}

void serialCMDInitCommands()
{
    cmdAdd("arm", serialArmTorpedo); //arm [0 or 1]
    cmdAdd("disarm", serialDisarmTorpedo); //disarm [0 or 1]
    cmdAdd("fire", serialFireTorpedo); //fire [0 or 1]
    cmdAdd("setdemo", serialSetDemo); //setdemo [0 or 1]
    cmdAdd("printparam", serialPrintParams); //no arguments
    cmdAdd("restoreparam", serialResetToDefaultParams); //no arguments
    cmdAdd("saveparam", serialSaveParams); //no arguments
    cmdAdd("displayvoltages", serialDisplayVoltages);
    cmdAdd("enablelvindicator", serialEnableLowVoltIndicator); //enable(1) or disable(0)
    cmdAdd("enablepressurealert", serialEnablePressureAlert); //enable(1) or disable(0)
}

#endif