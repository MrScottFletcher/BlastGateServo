/*  Blast gate servo controller for Arduino
 *   
 *   Allows a single push button to switch between different blast gates, with only a single one open at a time.
 *   Currently open blast gate is indicated by an LED.
 *    
 *   Requires:  A servo for each blast gate, one button
 *   Optional: An LED for each blast gate
 *   
 *   In the future I plan on adding an option to automate this based on detected AC current from the tool
 *   (if my sensor ever shows up)
 *   
 *   Created 2019-01-02 - Greg Pringle
 *   Updated 2019-01-20 - Greg Pringle - Refactored, added support for AC current sensor
 */
#include <Servo.h>
#include "Debug.h"
#include "Configuration.h"
#include "GateServos.h"
#include "AcSensors.h"

int buttonState = 0;         // variable for reading the pushbutton status
int lastbuttonpushms = 0;
int curselectedgate = -1;
bool waitingforbuttonrelease = false;
bool metermode = false; 

volatile boolean ISR_updatePending;
volatile int ISR_gateIndexToActivate;


static const bool has_cyclebutton = HAS_BUTTON;
static const int cyclebuttonPin = BUTTON_PIN;
static const bool hascyclebutton = HAS_BUTTON;  
const int num_gates = NUM_GATES;      

GateServos gateservos(-1);  // object controlling blast gate servos
AcSensors acsensors;        // objest controlling AC current sensors

static const int gateButton_pin_1 = GATE_BUTTON_PIN_1;
static const int gateButton_pin_2 = GATE_BUTTON_PIN_2;
static const int gateButton_pin_3 = GATE_BUTTON_PIN_3;
static const int gateButton_pin_4 = GATE_BUTTON_PIN_4;
static const int gateButton_pin_5 = GATE_BUTTON_PIN_5;
static const int gateButton_pin_6 = GATE_BUTTON_PIN_6;
static const int gateButton_pin_7 = GATE_BUTTON_PIN_7;
static const int gateButton_pin_8 = GATE_BUTTON_PIN_8;

const int gatebuttonpin[8] = {gateButton_pin_1,gateButton_pin_2,gateButton_pin_3,gateButton_pin_4,gateButton_pin_5,gateButton_pin_6,gateButton_pin_7,gateButton_pin_8}; // LED pins    

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  Serial.println("16 channel PWM test!");

  gateservos.initializeGates();

//  #ifdef DEBUG
//  gateservos.testServo(0);
//  gateservos.testServo(1);
//  gateservos.testServo(2);
//  gateservos.testServo(3);
//  #endif

  // initialize the pushbutton pin as an input:
  if (has_cyclebutton) pinMode(cyclebuttonPin, INPUT);

  //for (int thisgate = 0; thisgate < 4; thisgate++)
  for (int thisgate = 0; thisgate < num_gates; thisgate++)
  {
     int pin = gatebuttonpin[thisgate];
     if(pin >= 0) pinMode(pin, INPUT_PULLUP);
  }
  
  //Cant send parms in/out of ISRs, so gonna do it the old-fashioned way...
  if(gatebuttonpin[0] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[0]), gateButtonPressISR_1, CHANGE);
  if(gatebuttonpin[1] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[1]), gateButtonPressISR_2, CHANGE);
  if(gatebuttonpin[2] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[2]), gateButtonPressISR_3, CHANGE);
  if(gatebuttonpin[3] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[3]), gateButtonPressISR_4, CHANGE);
  if(gatebuttonpin[4] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[4]), gateButtonPressISR_5, CHANGE);
  if(gatebuttonpin[5] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[5]), gateButtonPressISR_6, CHANGE);
  if(gatebuttonpin[6] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[6]), gateButtonPressISR_7, CHANGE);
  if(gatebuttonpin[7] >=0) attachInterrupt(digitalPinToInterrupt(gatebuttonpin[7]), gateButtonPressISR_8, CHANGE);

  if (has_cyclebutton && digitalRead(cyclebuttonPin)== HIGH) // user held down button on startup, go into meter mode
      metermode = true;
  else metermode = false;

  #ifdef DEBUGMETER
      metermode = true;
  #endif
  acsensors.InitializeSensors();
}


void loop() 
{
  bool toolon = false; // indicates if there is any current sensed

  acsensors.ReadSensors(); // read all the AC current sensors
  
  if (metermode)  acsensors.DisplayMeter();  // if user put device into meter mode, use LED lights to display sensor signal.
  else // not meter mode
  {
    //================================================
    // AC SENSOR LOOP
    //================================================
    for (int cursensor=0; cursensor < acsensors.num_ac_sensors; cursensor++)
    {
        // This sensor is triggered by power tool
        //
        if (acsensors.Triggered(cursensor))
        {
          // this tool is active, output info to debug
          //DPRINT(" TOOL ON #"); DPRINT(cursensor); DPRINT(" OFF READING:"); DPRINT(offReadings[cursensor]); DPRINT(" AVG SENSOR READING:"); DPRINTLN(avgSensorReading(cursensor));

          // ignore button if tool detected
          waitingforbuttonrelease = false;
          toolon = true;
          if (curselectedgate != cursensor) curselectedgate = cursensor;

          // Gate hasn't been opened yet, open it
          if (gateservos.gateopen[cursensor] != true)
          {
            gateservos.gateopen[cursensor] = true;
            gateservos.ledon(cursensor);
            gateservos.opengate(cursensor);
          }
        }
        else
        {
          // this tool is not active and gate hasn't been closed yet. Close it.
          if (gateservos.gateopen[cursensor])
          {
            gateservos.gateopen[cursensor] = false;
            gateservos.ledoff(cursensor);
            DPRINT(" TOOL OFF #"); DPRINTLN(cursensor);
            gateservos.closegate(cursensor);      
            curselectedgate = gateservos.firstgateopen();  // change currently active gate to first open one
          }
        }
    }
    //================================================
    // read the state of the pushbutton value.  
    // Allow the user to light up the appropriate gate and pause before opening/closing the gates.
    if (hascyclebutton) buttonState = digitalRead(cyclebuttonPin);
    if (hascyclebutton && !toolon && buttonState == HIGH)
    {
      if (! waitingforbuttonrelease)
      {
        lastbuttonpushms = 0;
        DPRINT("Button Pushed, curselectedgate= ");  DPRINTLN(curselectedgate);
        gateservos.ledoff(curselectedgate);
        curselectedgate++;
        if (curselectedgate == gateservos.num_gates) 
          curselectedgate = -1;
        else
          gateservos.ledon(curselectedgate);
        waitingforbuttonrelease= true;
      }
    }
    
    if (hascyclebutton && !toolon && buttonState == LOW)
    {
      if (waitingforbuttonrelease)
      {
         waitingforbuttonrelease=false;
         lastbuttonpushms = 0;
      }
    
      if (lastbuttonpushms > gateservos.opendelay)
      {    
          gateservos.ManuallyOpenGate(curselectedgate);
      }
    }
    //================================================
    //  ISR EVENT WATCHER
    //================================================
    if(ISR_updatePending){
        DPRINT("ISR detected! For Gate: ");
        DPRINTLN(ISR_gateIndexToActivate);
        lastDebounceTime = millis();
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:
        if(curselectedgate != ISR_gateIndexToActivate){
          //shut the gate
          DPRINT("Closing gate: ");
          DPRINTLN(curselectedgate);
          gateservos.gateopen[curselectedgate] = false;
          gateservos.ledoff(curselectedgate);
          gateservos.closegate(curselectedgate);
  
          curselectedgate = ISR_gateIndexToActivate;
        }
        if(gateservos.gateopen[curselectedgate] == false)
        {
          //turn it on
          DPRINTLN("ISR TURN ON");
          gateservos.gateopen[curselectedgate] = true;
          gateservos.ledon(curselectedgate);
          gateservos.opengate(curselectedgate);
          gateservos.powerOn();
          //Clear the flag to allow another update
          ISR_updatePending = false;
          
        }
        else
        {
          //turn it off, like  a light switch, now it's gone!
          DPRINTLN("ISR SAME BUTTON - TOGGLE POWER");
//          gateservos.gateopen[curselectedgate] = false;
//          gateservos.closegate(curselectedgate);

          //Clear the flag to allow another update
          gateservos.powerToggle();

          if(gateservos.bPowerOn){
            gateservos.ledon(curselectedgate);
          }
          else{
            gateservos.ledoff(curselectedgate);
          }
          
          ISR_updatePending = false;
        }
    }
    //================================================
  }
  
  if (metermode)
   delay(1);  // minimal delay while metering so we can collect as many samples as possible
  else 
   delay(50);
  if (!toolon)
  {    
     // User is picking which gate to open manually. this code 'debounces' the button so it only moves one at a time
     //
     if (curselectedgate == gateservos.curopengate) 
        lastbuttonpushms = 0;
     else
        lastbuttonpushms +=50;
  }
}

void gateButtonPressISR_1() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 1 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 0;
  }
}
void gateButtonPressISR_2() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 2 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 1;
  }
}
void gateButtonPressISR_3() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 3 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 2;
  }
}
void gateButtonPressISR_4() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 4 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 3;
  }
}
void gateButtonPressISR_5() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 5 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 4;
  }
}
void gateButtonPressISR_6() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 6 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 5;
  }
}
void gateButtonPressISR_7() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 7 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 6;
  }
}
void gateButtonPressISR_8() {
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    DPRINTLN("BUTTON 8 DETECTED");
    ISR_updatePending = true;
    ISR_gateIndexToActivate = 7;
  }
}
