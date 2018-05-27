#include <libARVPpwm.h>
#include <IntervalTimer.h>
#include <torpedoControl.hpp>
#include <batteryStatus.hpp>
#include <servoControl.hpp>
#include <ledIndicationControl.hpp>
#include <Metro.h>
#include <teensy_uavcan.hpp>
#include <subscriber.hpp>
#include <parameter.hpp>
#include <uavcanNodeIDs.h>
#include <watchdog.h>
#include <Cmd.h>
#include <serialCommand.hpp>
#include <pressureStatus.hpp>

#define BATTERY_VOLTAGE_POOR_VALUE 13.25
#define BATTERY_VOLTAGE_DANGER_VALUE 12.75

#define DEFAULT_STROBE_INTERVAL 250

// UAVCAN Node settings
static constexpr uint32_t nodeID = UAVCAN_NODE_ID_TORPEDO_BOARD;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.arvp.dropperTorpedo";

// UAVCAN application settings
static constexpr float framerate = 100;

//PCA9685 object, uses default address 0x40
Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(PCA9685_BASEADDR);

//strobe light timer
Metro strobeLightTimer = Metro(DEFAULT_STROBE_INTERVAL);

//Sin wave generator definitions
const int numberOfSinTableEntries = 1000;
const int sinWaveAmplitude = 4096;
int baseSinFrq = 1; //Hz

bool demoMode = false;

static const int sinWaveTable[numberOfSinTableEntries] = 
{
	2048,2060,2073,2086,2099,2112,2125,2138,2150,2163,2176,2189,2202,2215,2227,2240,2253,2266,2279,2291,2304,2317,2330,2342,2355,2368,2380,
    2393,2406,2419,2431,2444,2456,2469,2482,2494,2507,2519,2532,2544,2557,2569,2582,2594,2606,2619,2631,2643,2656,2668,2680,2692,2705,2717,
    2729,2741,2753,2765,2777,2789,2801,2813,2825,2837,2849,2861,2872,2884,2896,2908,2919,2931,2942,2954,2966,2977,2988,3000,3011,3023,3034,
    3045,3056,3068,3079,3090,3101,3112,3123,3134,3145,3155,3166,3177,3188,3198,3209,3220,3230,3241,3251,3261,3272,3282,3292,3302,3313,3323,
    3333,3343,3353,3363,3372,3382,3392,3402,3411,3421,3430,3440,3449,3458,3468,3477,3486,3495,3504,3513,3522,3531,3540,3549,3558,3566,3575,
    3583,3592,3600,3609,3617,3625,3633,3641,3649,3657,3665,3673,3681,3689,3696,3704,3711,3719,3726,3734,3741,3748,3755,3762,3769,3776,3783,
    3790,3797,3803,3810,3816,3823,3829,3836,3842,3848,3854,3860,3866,3872,3878,3883,3889,3895,3900,3906,3911,3916,3921,3927,3932,3937,3942,
    3946,3951,3956,3961,3965,3970,3974,3978,3983,3987,3991,3995,3999,4003,4006,4010,4014,4017,4021,4024,4027,4031,4034,4037,4040,4043,4046,
    4048,4051,4054,4056,4059,4061,4063,4066,4068,4070,4072,4074,4075,4077,4079,4080,4082,4083,4085,4086,4087,4088,4089,4090,4091,4092,4092,
    4093,4094,4094,4094,4095,4095,4095,4095,4095,4095,4095,4094,4094,4094,4093,4092,4092,4091,4090,4089,4088,4087,4086,4085,4083,4082,4080,
    4079,4077,4075,4074,4072,4070,4068,4066,4063,4061,4059,4056,4054,4051,4048,4046,4043,4040,4037,4034,4031,4027,4024,4021,4017,4014,4010,
    4006,4003,3999,3995,3991,3987,3983,3978,3974,3970,3965,3961,3956,3951,3946,3942,3937,3932,3927,3921,3916,3911,3906,3900,3895,3889,3883,
    3878,3872,3866,3860,3854,3848,3842,3836,3829,3823,3816,3810,3803,3797,3790,3783,3776,3769,3762,3755,3748,3741,3734,3726,3719,3711,3704,
    3696,3689,3681,3673,3665,3657,3649,3641,3633,3625,3617,3609,3600,3592,3583,3575,3566,3558,3549,3540,3531,3522,3513,3504,3495,3486,3477,
    3468,3458,3449,3440,3430,3421,3411,3402,3392,3382,3372,3363,3353,3343,3333,3323,3313,3302,3292,3282,3272,3261,3251,3241,3230,3220,3209,
    3198,3188,3177,3166,3155,3145,3134,3123,3112,3101,3090,3079,3068,3056,3045,3034,3023,3011,3000,2988,2977,2966,2954,2942,2931,2919,2908,
    2896,2884,2872,2861,2849,2837,2825,2813,2801,2789,2777,2765,2753,2741,2729,2717,2705,2692,2680,2668,2656,2643,2631,2619,2606,2594,2582,
    2569,2557,2544,2532,2519,2507,2494,2482,2469,2456,2444,2431,2419,2406,2393,2380,2368,2355,2342,2330,2317,2304,2291,2279,2266,2253,2240,
    2227,2215,2202,2189,2176,2163,2150,2138,2125,2112,2099,2086,2073,2060,2048,2035,2022,2009,1996,1983,1970,1957,1945,1932,1919,1906,1893,
    1880,1868,1855,1842,1829,1816,1804,1791,1778,1765,1753,1740,1727,1715,1702,1689,1676,1664,1651,1639,1626,1613,1601,1588,1576,1563,1551,
    1538,1526,1513,1501,1489,1476,1464,1452,1439,1427,1415,1403,1390,1378,1366,1354,1342,1330,1318,1306,1294,1282,1270,1258,1246,1234,1223,
    1211,1199,1187,1176,1164,1153,1141,1129,1118,1107,1095,1084,1072,1061,1050,1039,1027,1016,1005,994,983,972,961,950,940,929,918,907,897,
    886,875,865,854,844,834,823,813,803,793,782,772,762,752,742,732,723,713,703,693,684,674,665,655,646,637,627,618,609,600,591,582,573,564,
    555,546,537,529,520,512,503,495,486,478,470,462,454,446,438,430,422,414,406,399,391,384,376,369,361,354,347,340,333,326,319,312,305,298,
    292,285,279,272,266,259,253,247,241,235,229,223,217,212,206,200,195,189,184,179,174,168,163,158,153,149,144,139,134,130,125,121,117,112,
    108,104,100,96,92,89,85,81,78,74,71,68,64,61,58,55,52,49,47,44,41,39,36,34,32,29,27,25,23,21,20,18,16,15,13,12,10,9,8,7,6,5,4,3,3,2,1,1,
    1,0,0,0,0,0,0,0,1,1,1,2,3,3,4,5,6,7,8,9,10,12,13,15,16,18,20,21,23,25,27,29,32,34,36,39,41,44,47,49,52,55,58,61,64,68,71,74,78,81,85,89,
    92,96,100,104,108,112,117,121,125,130,134,139,144,149,153,158,163,168,174,179,184,189,195,200,206,212,217,223,229,235,241,247,253,259,
    266,272,279,285,292,298,305,312,319,326,333,340,347,354,361,369,376,384,391,399,406,414,422,430,438,446,454,462,470,478,486,495,503,512,
    520,529,537,546,555,564,573,582,591,600,609,618,627,637,646,655,665,674,684,693,703,713,723,732,742,752,762,772,782,793,803,813,823,834,
    844,854,865,875,886,897,907,918,929,940,950,961,972,983,994,1005,1016,1027,1039,1050,1061,1072,1084,1095,1107,1118,1129,1141,1153,1164,
    1176,1187,1199,1211,1223,1234,1246,1258,1270,1282,1294,1306,1318,1330,1342,1354,1366,1378,1390,1403,1415,1427,1439,1452,1464,1476,1489,
    1501,1513,1526,1538,1551,1563,1576,1588,1601,1613,1626,1639,1651,1664,1676,1689,1702,1715,1727,1740,1753,1765,1778,1791,1804,1816,1829,
    1842,1855,1868,1880,1893,1906,1919,1932,1945,1957,1970,1983,1996,2009,2022,2035
};

int sinWaveTime = 0;
float sinWave0 = 0;
float sinWave120 = 0;
float sinWave180 = 0;
float sinWave240 = 0;

IntervalTimer sinWaveTimer;

void stepSinWave()
{
    sinWave0 = sinWaveTable[sinWaveTime]; //0 degree phase shift
    sinWave120 = sinWaveTable[(sinWaveTime+333)%numberOfSinTableEntries]; //120 degree phase shift
    sinWave180 = sinWaveTable[(sinWaveTime+500)%numberOfSinTableEntries]; //180 degree phase shift
    sinWave240 = sinWaveTable[(sinWaveTime+667)%numberOfSinTableEntries]; //240 degree phase shift
    ++sinWaveTime %= numberOfSinTableEntries;
}

void indicatorRoutine() //Add this function to loop() to allow for indication of torpedo and battery status
{
    if((pressureState == DROPPING) && (boardConfig[PARAM_INDEX_ENABLE_PRESSURE_ALERT].paramValue))
    {
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 210);
        pwmDriver.setRGB(1, 1, 1, 0, 0.1*sinWave0/sinWaveAmplitude);
        return;
    }
    if((torpedoState0 == FIRING) || (torpedoState1 == FIRING))
    {
        disableExternalLEDControl();
        pwmDriver.setRGB(1, 0, 0, 0, 0.5);
        return;
    }
    if((torpedoState0 == ARMED) && (torpedoState1 == ARMED))
    {
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 1000);
        pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0, 0.2);
        return;
    }
    if(torpedoState0 == ARMED)
    {
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 1000);
        pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0.1*sinWave180/sinWaveAmplitude, 0, 0, 0.2);
        return;
    }
    if(torpedoState1 == ARMED)
    {
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 1000);
        pwmDriver.setRGB(sinWave0/sinWaveAmplitude, 0, 0.1*sinWave180/sinWaveAmplitude, 0, 0.2);
        return;
    }
    if(checkVoltages(BATTERY_VOLTAGE_POOR_VALUE, BATTERY_VOLTAGE_DANGER_VALUE) == DANGER && (int)boardConfig[PARAM_INDEX_ENABLE_LOW_VOLT_INDICATOR].paramValue)
    {
        //Serial.println("POOR");
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 2000);
        pwmDriver.setRGB(1, 0, 0, 0, 0.25*sinWave0/sinWaveAmplitude);
        return;
    }
    if(checkVoltages(BATTERY_VOLTAGE_POOR_VALUE, BATTERY_VOLTAGE_DANGER_VALUE) == POOR  && (int)boardConfig[PARAM_INDEX_ENABLE_LOW_VOLT_INDICATOR].paramValue)
    {
        //Serial.println("DANGER");
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 210);
        pwmDriver.setRGB(1, 0, 0, 0, 0.25*sinWave0/sinWaveAmplitude);
        return;
    }
    if((int)boardConfig[PARAM_INDEX_DEMO_MODE].paramValue)
    {
        disableExternalLEDControl();
        sinWaveTimer.begin(stepSinWave, 2000);
        pwmDriver.setRGB(sinWave0/sinWaveAmplitude, sinWave120/sinWaveAmplitude/4, sinWave240/sinWaveAmplitude/6, 0, 0.125);
        return;
    }
    else
    {
        if(enableExternalLEDControl()) //Only turn off LED if external LED control is disabled
        {
            pwmDriver.setRGB(0, 0, 0, 0, 0);
            sinWaveTimer.end();   
        }
        if(strobeActivated && strobeLightTimer.check())
        {
            pwmDriver.setRGB(0, 0, 0, 0, 0);
            strobeActivated = false;
        }
    }
}

// this runs once to setup everything
void setup() 
{
    KickDog();
    Serial.begin(9600);
    Serial.println("Setup start");
    
    cmdInit(&Serial);
    serialCMDInitCommands();

    pwmDriver.begin();

    initTorpedos();
    initLEDControl(&pwmDriver, &strobeLightTimer);

    //--UAVCAN init--//
    // init LEDs
    initLeds();

    // Create a node
    systemClock = &getSystemClock();
    canDriver = &getCanDriver();
    node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
    initNode(node, nodeID, nodeName, swVersion, hwVersion);

    // init subscriber
    initSubscriber(node);

    // start up node
    node->setModeOperational();

    // start parameter server
    initParameter(node);
    initServoControl(&pwmDriver);
    Serial.println("Setup Complete");
}

//Runs continuously
void loop() 
{
    KickDog();
    indicatorRoutine();
    torpedoRoutine();

    cmdPoll();

    //--UAVCAN cycles--//
    // wait in cycle
    cycleWait(framerate);

    // do some CAN stuff
    cycleNode(node);

    // toggle heartbeat
    toggleHeartBeat();
}