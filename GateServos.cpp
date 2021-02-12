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


//=============================
//    
//    pwm.setPWM(servopin, 0, SERVOMIN);
//    DPRINTLN("Set to MIN");
//
//    delay(2000);
//        
//    pwm.setPWM(servopin, 0, SERVOMAX);
//    DPRINTLN("Set to MAX");
//
//    delay(2000);
//        
//    pwm.setPWM(servopin, 0, SERVOMIN);
//    DPRINTLN("Set to MIN");
//=============================    
//    myservo.attach(servopin);  // attaches the servo
//    myservo.write(255); 
//    delay(2000);
//    DPRINTLN("Set to 255");
//    myservo.write(0);
//    delay(2000);
//    DPRINTLN("Set to 0");
//    myservo.detach();
  }
//#####################################################################
  // Initialize gates and close them all
  //
  void GateServos::initializeGates()
  {

    pwm.begin();
    
    //pwm.setPWMFreq(1000);
    
    pwm.setPWMFreq(60);  
    
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  //pwm.setOscillatorFrequency(27000000);
  //pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  //Wire.setClock(400000);


    //testServo(12);  
    // close all gates one by one
    for (int thisgate = 0; thisgate < num_gates; thisgate++)
    {
     pinMode(ledpin[thisgate], OUTPUT);
     digitalWrite(ledpin[thisgate], HIGH);
     pwm.setPWM(servopin[thisgate], 0, servoClosedPos[thisgate]);

//     myservo.attach(servopin[thisgate]);  // attaches the servo
//     myservo.write(servoClosedPos[thisgate]); //close gate
//     delay(closedelay); // wait for gate to close
//     myservo.detach();

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
