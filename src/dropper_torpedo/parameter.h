#if !defined(PARAMETER_H)
#define PARAMETER_H

/* macros */
#define LEN(a) (sizeof(a) / sizeof(*a))

/* PWM Channel definitions for PCA9685 chip on torpedo board */
enum pwm_chan {
	PWM_CHAN_0,
	PWM_CHAN_1,
	PWM_CHAN_2,
	PWM_CHAN_3,
	PWM_CHAN_4,
	PWM_CHAN_5,
	PWM_CHAN_6,
	PWM_CHAN_7,
	PWM_CHAN_8,
	PWM_CHAN_9,
	PWM_CHAN_10,
	PWM_CHAN_11
};

enum param_index {
	TORPEDO_PULSE,
	MIN_SERVO_PULSE,
	MAX_SERVO_PULSE,
	ACT_ID_TORP_0,
	ACT_ID_TORP_1,
	ACT_ID_SERVO_0,
	ACT_ID_SERVO_1,
	ACT_ID_SERVO_2,
	ACT_ID_SERVO_3,
	ACT_ID_SERVO_4,
	ACT_ID_SERVO_5,
	ACT_ID_SERVO_6,
	ACT_ID_SERVO_7,
	ACT_ID_SERVO_8,
	ACT_ID_SERVO_9,
	ACT_ID_SERVO_10,
	ACT_ID_SERVO_11,
	LIGHT_ID_STRIP_0,
	LIGHT_ID_STRIP_0_STROBE,
	STROBE_INTERVAL,
	DEMO_MODE,
	ENABLE_LOW_VOLT_INDICATOR,
	LOW_PRESSURE_THRESHOLD,
	ENABLE_PRESSURE_ALERT,
	ENABLE_TORPEDO_INDICATOR,
	SERVO_START_ANGLE
};

struct Param {
	char name[24];
	double value;
};

/* based on sizeof(Param) (32 bytes) and 2048 byte EEPROM capacity in Teensy 3.2 */
#define MAX_NUMBER_OF_PARAMETERS 64

extern struct Param boardConfig[MAX_NUMBER_OF_PARAMETERS];

#endif