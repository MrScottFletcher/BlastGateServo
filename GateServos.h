/*
  GateServos.h - Library for operating blast gate servos
  Created by Greg Pringle 2019-01-20
  Released into the public domain.
*/
#ifndef GateServo_h
#define GateServo_h

#include "Arduino.h"
#include <Servo.h>
#include "Debug.h"
#include "Configuration.h"

  class GateServos {
    static const int num_servos = NUM_GATES;
    static const int num_gateButtons = NUM_GATE_BUTTONS;
    static const int num_leds = NUM_LEDS;
    static const int servo_pin_1 = SERVO_PIN_1;
    static const int servo_pin_2 = SERVO_PIN_2; 
    static const int servo_pin_3 = SERVO_PIN_3;
    static const int servo_pin_4 = SERVO_PIN_4;
    static const int servo_pin_5 = SERVO_PIN_5;
    static const int servo_pin_6 = SERVO_PIN_6;
    static const int servo_pin_7 = SERVO_PIN_7;
    static const int servo_pin_8 = SERVO_PIN_8;

    static const int servo_closed_1 = SERVO_CLOSED_1;
    static const int servo_closed_2 = SERVO_CLOSED_2;
    static const int servo_closed_3 = SERVO_CLOSED_3;
    static const int servo_closed_4 = SERVO_CLOSED_4;
    static const int servo_closed_5 = SERVO_CLOSED_5;
    static const int servo_closed_6 = SERVO_CLOSED_6;
    static const int servo_closed_7 = SERVO_CLOSED_7;
    static const int servo_closed_8 = SERVO_CLOSED_8;

    static const int servo_open_1 = SERVO_OPEN_1;
    static const int servo_open_2 = SERVO_OPEN_2;
    static const int servo_open_3 = SERVO_OPEN_3;
    static const int servo_open_4 = SERVO_OPEN_4;
    static const int servo_open_5 = SERVO_OPEN_5;
    static const int servo_open_6 = SERVO_OPEN_6;
    static const int servo_open_7 = SERVO_OPEN_7;
    static const int servo_open_8 = SERVO_OPEN_8;

    static const int led_pin_1 = LED_PIN_1;
    static const int led_pin_2 = LED_PIN_2;
    static const int led_pin_3 = LED_PIN_3;
    static const int led_pin_4 = LED_PIN_4;
    static const int led_pin_5 = LED_PIN_5;
    static const int led_pin_6 = LED_PIN_6;
    static const int led_pin_7 = LED_PIN_7;
    static const int led_pin_8 = LED_PIN_8;

    static const int gateButton_pin_1 = GATE_BUTTON_PIN_1;
    static const int gateButton_pin_2 = GATE_BUTTON_PIN_2;
    static const int gateButton_pin_3 = GATE_BUTTON_PIN_3;
    static const int gateButton_pin_4 = GATE_BUTTON_PIN_4;
    static const int gateButton_pin_5 = GATE_BUTTON_PIN_5;
    static const int gateButton_pin_6 = GATE_BUTTON_PIN_6;
    static const int gateButton_pin_7 = GATE_BUTTON_PIN_7;
    static const int gateButton_pin_8 = GATE_BUTTON_PIN_8;

    static const int powerservo_pin_off = POWER_SERVO_PIN_OFF;
    static const int powerservo_pin_on = POWER_SERVO_PIN_ON;

    static const int powerservo_max_off = POWER_SERVO_MAX_OFF;
    static const int powerservo_max_on = POWER_SERVO_MAX_ON;

    static const int powerservo_min_off = POWER_SERVO_MIN_OFF;
    static const int powerservo_min_on = POWER_SERVO_MIN_ON;


    static const int closedelay = CLOSE_DELAY;

    const int servopin[8] = {servo_pin_1,servo_pin_2,servo_pin_3,servo_pin_4,servo_pin_5,servo_pin_6,servo_pin_7,servo_pin_8}; 
    const int servoClosedPos[8] = {servo_closed_1,servo_closed_2,servo_closed_3,servo_closed_4,servo_closed_5,servo_closed_6,servo_closed_7,servo_closed_8};
    const int servoOpenPos[8] = {servo_open_1,servo_open_2,servo_open_3,servo_open_4,servo_open_5,servo_open_6,servo_open_7,servo_open_8};
    const int ledpin[8] = {led_pin_1,led_pin_2,led_pin_3,led_pin_4,led_pin_5,led_pin_6,led_pin_7,led_pin_8}; // LED pins    
    const int gatebuttonpin[8] = {gateButton_pin_1,gateButton_pin_2,gateButton_pin_3,gateButton_pin_4,gateButton_pin_5,gateButton_pin_6,gateButton_pin_7,gateButton_pin_8}; // LED pins    

    
    //Servo myservo;  // create servo object to control a servo
             // a maximum of eight servo objects can be created
    
    public:    
      GateServos(int curopengate);  // initialize indicating currenly open gate (usually -1 for none)
      void opengate(int gatenum);   // open the given gate number
      void closegate(int gatenum);  // close the given gate number
      void initializeGates();       // initialize gates and close them all
      void ledoff(int gatenum);     // turn off given LED
      void ledon(int gatenum);      // turn on given LED
      void flashLedAndDelay(int gatenum, unsigned long msTotalDelay, unsigned long flashDelay);      // flash an LED and delay
      void ManuallyOpenGate(int gatenum);   // User manually opening given gate using the button
      int firstgateopen();                  // Returns ID of first gate that is open
      void testServo(int servopin);         // Test given servo pin (debug function)
      const int num_gates = NUM_GATES;      // 
      int curopengate = -1;                 // cuurrently open gate selected manually with button
      const int opendelay = OPEN_DELAY;     // ms delay to allow servo to completely open gate
      bool gateopen[8] = {false, false, false, false,false, false, false, false};   // array indicating which gates are open

      void powerOn();   // open the given gate number
      void powerOff();  // close the given gate number
      void powerToggle();
      void initializePower();       // turn off power
      bool bPowerOn = false;

  };
  

#endif
