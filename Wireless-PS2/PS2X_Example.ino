#include <PS2X_lib.h>  //for v1.6
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

PS2X ps2x; // create PS2 Controller Class
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0; 
byte type = 0;
byte vibrate = 0;
const int motorleft1 = 8;
const int motorleft2 = 9;
const int motorright1 = 7;
const int motorright2 = 6;

//right motor to left side of h bridge motorleft = h-bridge right
//h bridge pin 2 to arduino pin 2
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);

void setup(){
 Serial.begin(57600);
 AFMS.begin();
  myMotor->setSpeed(100);
  myMotor->run(FORWARD);
  myMotor->run(RELEASE);
  myMotor2->setSpeed(100);
  myMotor2->run(FORWARD);
  myMotor2->run(RELEASE);
  myMotor3->setSpeed(100);
  myMotor3->run(FORWARD);
  myMotor3->run(RELEASE);
  myMotor4->setSpeed(100);
  myMotor4->run(FORWARD);
  myMotor4->run(RELEASE);
  pinMode(motorleft1, OUTPUT);
  pinMode(motorleft2, OUTPUT);
  pinMode(motorright1, OUTPUT);
  pinMode(motorright2, OUTPUT);

 //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  
 error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

 Serial.println(error);
 if(error == 0){
  Serial.println("Found Controller, configured successful");
  Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
   
 
  
}

void loop(){
   unsigned long startTime = millis();
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
   
   
   
 if(error == 1) //skip loop if no controller found
  return; 
  
 if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller 
   if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
  
 }

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    digitalWrite(motorleft1, LOW);
    digitalWrite(motorleft2, LOW);
    digitalWrite(motorright1, LOW);
    digitalWrite(motorright2, LOW);
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
        Serial.println("Select is being held");
          
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
       digitalWrite(motorleft1, HIGH);
        digitalWrite(motorleft2, LOW);
       digitalWrite(motorright1, HIGH);
       digitalWrite(motorright2, LOW);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
       digitalWrite(motorleft1, HIGH);
       digitalWrite(motorleft2, LOW);
       digitalWrite(motorright1, LOW);
       digitalWrite(motorright2, HIGH);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
       digitalWrite(motorleft1, LOW);
       digitalWrite(motorleft2, HIGH);
       digitalWrite(motorright1, HIGH);
       digitalWrite(motorright2, LOW);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      digitalWrite(motorleft1, LOW);
      digitalWrite(motorleft2, HIGH);
      digitalWrite(motorright1, LOW);
      digitalWrite(motorright2, HIGH);
      }   
  
    
             
       if(ps2x.ButtonPressed(PSB_L3))
         Serial.println("L3 pressed");
       if(ps2x.ButtonPressed(PSB_R3))
         Serial.println("R3 pressed");
        
       if(ps2x.ButtonPressed(PSB_L2)){
          Serial.println("L2 pressed");
          myMotor4->run(BACKWARD);
        }
       if(ps2x.ButtonReleased(PSB_L2)){
         myMotor4->run(RELEASE);
        }
        
       if(ps2x.ButtonPressed(PSB_R2)){
         Serial.println("R2 pressed");
         myMotor4->run(FORWARD);
        }
       if(ps2x.ButtonReleased(PSB_R2)){
         myMotor4->run(RELEASE);
        
        }
       if(ps2x.ButtonPressed(PSB_L1)){
          Serial.println("L1 pressed");
          myMotor3->run(BACKWARD);
        }
       if(ps2x.ButtonReleased(PSB_L1)){
         myMotor3->run(RELEASE);
        }
        
       if(ps2x.ButtonPressed(PSB_R1)){
         Serial.println("R1 pressed");
         myMotor3->run(FORWARD);
        }
       if(ps2x.ButtonReleased(PSB_R1)){
         myMotor3->run(RELEASE);
        
        }
       if(ps2x.ButtonPressed(PSB_GREEN)){
          Serial.println("Triangle pressed");
          myMotor2->run(FORWARD);
        }
       if(ps2x.ButtonReleased(PSB_GREEN)){
         myMotor2->run(RELEASE);
        }
        
       if(ps2x.ButtonPressed(PSB_PINK)){
         Serial.println("Square pressed");
         myMotor2->run(BACKWARD);
        }
       if(ps2x.ButtonReleased(PSB_PINK)){
         myMotor2->run(RELEASE);
        
        }
         
       

    //CLAW CODE
    static byte clawState = 1;
    if(ps2x.ButtonPressed(PSB_RED)){             //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
      clawState++;
    }
    else if(ps2x.ButtonReleased(PSB_RED)) {
      clawState--;
    }
    if(ps2x.ButtonPressed(PSB_BLUE)){            //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
      clawState--;
    }
    else if(ps2x.ButtonReleased(PSB_BLUE)) {
      clawState++;
    }
            

    if(clawState == 2) {
      myMotor->run(FORWARD);
      delay(30);
      myMotor->run(RELEASE);
    }
    else if(clawState == 0) {
      myMotor->run(BACKWARD);
      delay(30);
      myMotor->run(RELEASE);
    }
    //END CLAW CODE

 unsigned long endTime = millis();
 unsigned long difference = endTime - startTime;
 if(difference >= 50) difference = 0;
 delay(50 - difference);
 }
}
