#ifndef SERIAL_COMMAND_HPP
#define SERIAL_COMMAND_HPP

#include <Cmd.h>
#include <Arduino.h>
#include <parameter.hpp>
#include <torpedoControl.hpp>

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

void serialCMDInitCommands()
{
    cmdAdd("arm", serialArmTorpedo);
    cmdAdd("disarm", serialDisarmTorpedo);
    cmdAdd("fire", serialFireTorpedo);
    cmdAdd("setdemo", serialSetDemo);
    cmdAdd("printparam", serialPrintParams);
}

#endif