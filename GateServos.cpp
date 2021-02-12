#include "Arduino.h"
#include <Servo.h>
#include "Debug.h"
#include "Configuration.h"
#include "GateServos.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

  // called this way, it uses the default address 0x40
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);



  // Constructor.. usually called with -1 to indicate no gates are open
  //
  GateServos::GateServos(int initcuropengate)
  { 
    curopengate = initcuropengate;
  }

  // Open the given gate number
  //
  void GateServos::opengate(int gatenum) 
  {
      DPRINT("OPENING GATE #"); 
      DPRINT(gatenum);
      DPRINT(" SERVO PIN:");
      DPRINT(servopin[gatenum]);
      DPRINT(" VALUE:");
      DPRINT(servoOpenPos[gatenum]); 
      DPRINT(" DELAY:");
      DPRINT(opendelay);
      DPRINTLN("");
      curopengate = gatenum;
      
      digitalWrite(ledpin[gatenum], HIGH);
      int pulselength = map(0, 0, 180, servoOpenPos[gatenum], servoClosedPos[gatenum]);
      pwm.setPWM(servopin[gatenum], 0, pulselength);
      
//      myservo.attach(servopin[gatenum]);  // attaches the servo
//      myservo.write(servoOpenPos[gatenum]); //open gate
//      delay(opendelay); // wait for gate to close
//      myservo.detach();
      
     
      DPRINTLN("OPENED GATE");
      //if (servopin[gatenum] ==12) testServo(12);
  }


  // Close the given gate number
  void GateServos::closegate(int gatenum)
  {
    DPRINT("CLOSING GATE #");
    DPRINT(gatenum);
    DPRINTLN("");

    digitalWrite(ledpin[gatenum], LOW);
    int pulselength = map(180, 0, 180, servoOpenPos[gatenum], servoClosedPos[gatenum]);
    pwm.setPWM(servopin[gatenum], 0, pulselength);
    
//    myservo.attach(servopin[gatenum]);  // attaches the servo
//    myservo.write(servoClosedPos[gatenum]); //close gate
//    delay(closedelay); // wait for gate to close
//    myservo.detach();
    
    DPRINTLN("CLOSED GATE");
  }

//#####################################################################
  // Debug function to test servo by opening and closing it given a pin number
  void GateServos::testServo(int servopin)
  { 
    
    DPRINT("TESTING SERVO #");
    DPRINTLN(servopin);


 //Drive servo forward then back.
  for (int pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servopin, 0, pulselen);
  }
  delay(1500);
  
  for (int pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servopin, 0, pulselen);
  }
  delay(1000);

  }
//#####################################################################
  // Initialize gates and close them all
  //
  void GateServos::initializeGates()
  {

    pwm.begin();
    
    //pwm.setPWMFreq(1000);
    
    pwm.setPWMFreq(60);  
    
    // close all gates one by one
    for (int thisgate = 0; thisgate < num_gates; thisgate++)
    {
     pinMode(ledpin[thisgate], OUTPUT);
     digitalWrite(ledpin[thisgate], HIGH);
     pwm.setPWM(servopin[thisgate], 0, servoClosedPos[thisgate]);


     digitalWrite(ledpin[thisgate], LOW);
    }
  }

  // Turn LED on for given gate number
  void GateServos::ledon(int gatenum)
  {
    digitalWrite(ledpin[gatenum], HIGH);
  }

  // Turn LED off for given gate number
  void GateServos::ledoff(int gatenum)
  {
    digitalWrite(ledpin[gatenum], LOW);
  }

  // User has pushed button to manually open given gate
  void GateServos::ManuallyOpenGate(int curselectedgate)
  {
      if (curselectedgate == num_gates)
      {
        closegate(curopengate);
        curopengate=-1;
        curselectedgate=-1;
      }
      else
      {
        if (curopengate > -1) 
        {
          DPRINT("Button Closing Gate= ");  
          DPRINTLN(curopengate);
          closegate(curopengate);
        }      
        curopengate = curselectedgate;   
        DPRINT("Button Opening Gate= ");  
        DPRINTLN(curopengate);
        opengate(curopengate);      
      }
  }
  
  // return index of first open gate or -1 for none
  int GateServos::firstgateopen()
  {
    for (int curgate = 0; curgate < num_gates; curgate++)
      if (gateopen[curgate]) return curgate;
  
    return -1;
  }

  void GateServos::powerOn()
  {
      if (powerservo_pin_on > -1) {
          DPRINT("POWERING ON");
          DPRINT(" SERVO PIN:");
          DPRINT(powerservo_pin_on);
          DPRINT(" VALUE:");
          DPRINT(powerservo_min_on);
          DPRINTLN("");
          bPowerOn = true;

          int pulselength = map(0, 0, 180, powerservo_max_on, powerservo_min_on);
          pwm.setPWM(powerservo_pin_on, 0, pulselength);

          delay(500);

          pulselength = map(35, 0, 180, powerservo_max_on, powerservo_min_on);
          pwm.setPWM(powerservo_pin_on, 0, pulselength);
          DPRINTLN("POWER ON.");
      }
      else{
          DPRINTLN("(No Power On servo pin specified.)");
      }
  }


  // Close the given gate number
  void GateServos::powerOff()
  {
    if(powerservo_pin_off > -1){
      DPRINT("POWERING OFF");
      DPRINT(" SERVO PIN:");
      DPRINT(powerservo_pin_off);
      DPRINT(" VALUE:");
      DPRINT(powerservo_min_off);
      DPRINTLN("");
      bPowerOn = false;

      int pulselength = map(35, 0, 180, powerservo_max_off, powerservo_min_off);
      pwm.setPWM(powerservo_pin_off, 0, pulselength);

      delay(500);

      pulselength = map(0, 0, 180, powerservo_max_off, powerservo_min_off);
      pwm.setPWM(powerservo_pin_off, 0, pulselength);


      DPRINTLN("POWER OFF.");
    }
    else {
        DPRINTLN("(No Power On servo pin specified.)");
    }
  }
  void GateServos::powerToggle()
  {
    if(bPowerOn){
      powerOff();
    }
    else{
      powerOn();
    }
    
  }
  
