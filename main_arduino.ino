#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "utility/Adafruit_PWMServoDriver.h"
#define Center 80
#define PanCenter 87

Servo myservo; 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);

#include <Servo.h> 
 
Servo servo;
Servo servo_pan;
Servo servo_tilt;

const int NUMBER_OF_FIELDS = 2; // how many comma separated fields we expect
int fieldIndex = 0;            // the current field being received
double values[NUMBER_OF_FIELDS];   // array holding values for all the fields
double theta;
int FLAG;
int sign = 1;

void setup()
{
  Serial.begin(115200); // Initialize serial port to send and receive at 115200 baud
  
  AFMS.begin();
  M1->setSpeed(25);
  M2->setSpeed(25);
  M1->run(FORWARD);
  M2->run(FORWARD);
  
  myservo.attach(9); 
  servo_pan.attach(10);
  //servo_tilt.attach(11);
  servo_pan.write(PanCenter);
  //servo_tilt.write(90);
  myservo.write(Center);
  //M1->run(RELEASE);
  //M2->run(RELEASE);
}

void loop()
{     //servo_pan.write(90); 
    if(abs(theta)<40 && FLAG == 1)
    {
      
      M1->run(FORWARD);
      M2->run(FORWARD);
      M1->setSpeed(30);
      M2->setSpeed(30);
    }
    else
    {   M1->run(RELEASE);
        M2->run(RELEASE);
      }
      //M1->setSpeed(25);
      //M2->setSpeed(25);                             s                                                                                  
  if( Serial.available())
  {
    char ch = Serial.read();
    if(ch >= '0' && ch <= '9') // is this an ascii digit between 0 and 9?
    {
                          // yes, accumulate the value if the fieldIndex is within range
                          // additional fields are not stored
                          if(fieldIndex < NUMBER_OF_FIELDS) 
                          {
                            values[fieldIndex] = (values[fieldIndex] * 10) + (ch - '0'); 
                          }
                        }
                        else if (ch == ',')  // comma is our separator, so move on to the next field
                        {
                            values[fieldIndex] = values[fieldIndex] * sign; 
                            fieldIndex++;   // increment field index 
                            sign = 1;
                        }
                        else if (ch== '-')
                        {
                            sign = -1;    
                        }
                        else
                        {
                          // any character not a digit or comma ends the acquisition of fields
                          // in this example it's the newline character sent by the Serial Monitor
                          values[fieldIndex] = values[fieldIndex] * sign; //last number
                          // print each of the stored fields
                          theta = values[0]; //get degree error
                          FLAG = values[1]; //get FLAG
                      
                          for(int i=0; i < min(NUMBER_OF_FIELDS, fieldIndex+1); i++)
                          {
                            //Serial.println(values[i]);
                            values[i] = 0; // set the values to zero, ready for the next message
                          }
                          fieldIndex = 0;  // ready to start over
                          sign = 1;
                          //robot(Rtarget,Ltarget);
                        }
                        
   
    
  }
    int pan=0;
    int panOffsetLimit=20;
    int wheelServo=0;
    
    
    int pan_increment=int(theta)*0.2;
    
    if(abs(pan_increment)<2)
      { pan_increment=0;
      }
    
    
    
    pan=87-pan_increment;
    
    
    if(abs(theta)<60)
    
    {wheelServo=Center-0.1*int(theta);
    }
     else
     {wheelServo=Center-0.6*int(theta);
     }
    
    
    wheelServo=Center-0.7*int(theta);
    
    if(pan>(PanCenter+panOffsetLimit))
   {
       pan=PanCenter+panOffsetLimit;
   } 
    
    if(pan<(PanCenter-panOffsetLimit))
    {
   pan=PanCenter-panOffsetLimit;
  }
    
     
    if(wheelServo>110)
   {
   wheelServo=110;
   } 
    
    if(wheelServo<50)
    {
    wheelServo=50;
  }
    
    
    
    if(abs(theta)<10)
    {
      myservo.write(Center);
      delay(15);
    }
    else{
     myservo.write(wheelServo);
     delay(15);
    }
   
    //if(abs(theta)<35)
    //{servo_tilt.write(87);
  //}
    //else
   // {
      servo_pan.write(pan);
    //}
}
  



