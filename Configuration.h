
#ifndef CONFIGURATION_H
#define CONFIGURATION_H 
#define CONFIGURATION_H_VERSION 01

// All user defined settings required to configure operation are in this file

#define DEBUG   //Enable debug output
//#define DEBUGMETER // debug meter mode for placing sensors on cables


#define SERVOMIN  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  590 // this is the 'maximum' pulse length count (out of 4096)


// servo stuff 
#define CLOSE_DELAY 1000;// how long it takes a gate to close
#define OPEN_DELAY 1000; // how long after last button push to open gate

#define HAS_BUTTON false;  // true if button is attached
#define BUTTON_PIN 13;    // the number of the pushbutton pin

/// current Sensor stuff
#define NUM_AC_SENSORS 0;        // Number of AC sensors connected.. currently only handles 1 per gate
#define NUM_GATE_BUTTONS 4;
#define NUM_LEDS 4;              // Number of LEDs connected.. optional but should be 1 per gate
#define NUM_GATES 4;             // Number of blast gates with servos connected
#define NUM_OFF_SAMPLES 50;      // number of samples when checking avg sensor off values (unused)
#define NUM_OFF_MAX_SAMPLES 500; // Milliseconds to sample for max off value for each gate when starting up
#define AVG_READINGS 25;         // number of readings to average when triggering gates.. higher number is more accurate but more delay ( no more than 50)
#define AC_SENSOR_SENSITIVITY 2.0; // Triggers on twice the max readings of the off setting. The closer to one, the more sensitive

#ifdef DEBUG
#define MAX_BLINK_LEN 10; // blink faster when debugging to serial since slower
#else 
#define MAX_BLINK_LEN 1000 / NUM_AC_SENSORS; // slowest blink rate in ms
#endif

// PWM PCA9685 Servo Pins
#define SERVO_PIN_1 0; // Pin for first blast gate servo
#define SERVO_PIN_2 1; // ...etc..
#define SERVO_PIN_3 2;
#define SERVO_PIN_4 3;
#define SERVO_PIN_5 -1;//4
#define SERVO_PIN_6 -1;//5
#define SERVO_PIN_7 -1; //6
#define SERVO_PIN_8 -1;//---------

#define POWER_SERVO_PIN_ON 14; //Set to -1 if you don't have one
#define POWER_SERVO_PIN_OFF 15; //Set to -1 if you don't have one


// AC Sensor Pins
#define AC_SENSOR_PIN_1 -1;// A0; // Pin for first AC sensor
#define AC_SENSOR_PIN_2 -1;// A1; // ..etc...
#define AC_SENSOR_PIN_3 -1;// A2;
#define AC_SENSOR_PIN_4 -1;// A3;
#define AC_SENSOR_PIN_5 -1;
#define AC_SENSOR_PIN_6 -1;
#define AC_SENSOR_PIN_7 -1;
#define AC_SENSOR_PIN_8 -1;


// Servo Max (Closed) degrees positions.. maximum close position representing closed.
// It's ok to leave at default and for the servo to try to close too far but it will increase the power load
//
#define SERVO_CLOSED_1 SERVOMAX;
#define SERVO_CLOSED_2 SERVOMAX;
#define SERVO_CLOSED_3 SERVOMAX;
#define SERVO_CLOSED_4 SERVOMAX;
#define SERVO_CLOSED_5 -1;
#define SERVO_CLOSED_6 -1;
#define SERVO_CLOSED_7 -1;
#define SERVO_CLOSED_8 -1;

// Servo Min (Open) degrees positions
// It's ok to leave at default and for the servo to try to open too far but it will increase the power load
#define SERVO_OPEN_1 SERVOMIN;
#define SERVO_OPEN_2 SERVOMIN;
#define SERVO_OPEN_3 SERVOMIN;
#define SERVO_OPEN_4 SERVOMIN;
#define SERVO_OPEN_5 -1;
#define SERVO_OPEN_6 -1;
#define SERVO_OPEN_7 -1;
#define SERVO_OPEN_8 -1;

#define POWER_SERVO_MAX_OFF SERVOMAX;
#define POWER_SERVO_MAX_ON SERVOMAX;

#define POWER_SERVO_MIN_OFF SERVOMIN;
#define POWER_SERVO_MIN_ON SERVOMIN;

// LED pins 
#define LED_PIN_1 7;  // Pin for first LED indicator
#define LED_PIN_2 8;  // ...etc..
#define LED_PIN_3 9;
#define LED_PIN_4 10;
#define LED_PIN_5 -1;//11
#define LED_PIN_6 -1;//12
#define LED_PIN_7 -1;//13
#define LED_PIN_8 -1;

// GATE Button pins
// AT Mega boards can use these pins for ISR (Interrupts) 2, 3, 18, 19, 20, 21
//https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
#define GATE_BUTTON_PIN_1 2;  // Pin for first Gate Button (13 is reserved for the cycle button of the original)
#define GATE_BUTTON_PIN_2 3;  // ...etc..
#define GATE_BUTTON_PIN_3 18;
#define GATE_BUTTON_PIN_4 19;
#define GATE_BUTTON_PIN_5 -1; //20;
#define GATE_BUTTON_PIN_6 -1; //21;
#define GATE_BUTTON_PIN_7 -1;
#define GATE_BUTTON_PIN_8 -1;


#endif // CONFIGURATION_H
