/* batt_levelling firmware info */
#define SW_VERSION 1
#define HW_VERSION 1

#define NUM_OF_BATTERIES 4

/* Min voltage on ee batt to start switching */
#define EE_MIN_VOLTAGE 15

/* Difference in voltage needed to request battery swap */
#define V_DELTA 0.1

/* Pin definitions */
/* 10 should != 6 this will cause a short */
#define EE_BATT_CONT 10
#define MOTOR_BATT_CONT 6