/*
 * BTS7960_ESP32.h - Library for controlling a brushed DC motor using the BTS7960_ESP32 43amp motor driver.
 * Created by Yash Herekar, Feburary 10, 2024.
 * Released into the public domain.
 * Copyright [2024] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

/*Cannot create a .cpp file as  prototypes need the function to be present in the same file*/

#pragma once

class BTS7960_ESP32
{
  private:
  int8_t R_EN;
  int8_t L_EN;
  int8_t R_PWM;
  int8_t L_PWM;
  int8_t R_IS;
  int8_t L_IS;
  int8_t ID;
  uint8_t resolution;

  uint32_t freq;
  static String version;
  bool debugStatus;
  const uint8_t DEFAULTSPEED = 0;
  
  public:

  //pwm variable to control the speed of motor
  uint8_t speed;
  int8_t deviation;
  uint8_t oldMotorSpeed;
  uint8_t changeSpeedInc, changeSpeedDec;
  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++MOTOR STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  enum motorStates : uint8_t
  {
    FRONT = 'F',
    BACK = 'B',
    LEFT = 'L',
    RIGHT = 'R',
    SHARPLEFTFRONT = 'G',
    SHARPRIGHTFRONT = 'I',
    SHARPLEFTBACK = 'H',
    SHARPRIGHTBACK = 'J',
    STOP = 'S',
    STOPALL = 'X',
    ENABLE = 'E',
    DISABLE = 'D',
    FLIP = 'W',
    UNFLIP = 'U'
  };
  motorStates motorStatus = motorStates::STOPALL;                        //State variable set to STOP initially
  motorStates prevMotorStatus = motorStatus;                             //State variable set to STOP initially
  // motorStates ENABLEStatus = motorStates::ENABLE;                     //Motor Enable state
  // motorStates FLIPStatus = motorStates::UNFLIP;                       //Motor FLIP state

  
  /*=============================================Function prototyping section=====================================================*/
  inline BTS7960_ESP32();
  inline BTS7960_ESP32(const int8_t& = -1, const int8_t& = -1, const int8_t& = -1, const int8_t& = -1, const uint32_t& = 1000, const uint8_t& = 8, const int8_t& = -1, const int8_t& = -1, const int8_t& = -1, const bool& = false); // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, ID
  inline void begin() __attribute__((always_inline));
  inline void loop() __attribute__((always_inline));
  inline void enable() __attribute__((always_inline));
  inline void disable() __attribute__((always_inline));
  inline void front() __attribute__((always_inline));
  inline void back() __attribute__((always_inline));
  inline void stop() __attribute__((always_inline));
  inline void alarm() __attribute__((always_inline));
  inline static void printInfo() __attribute__((always_inline));
  inline ~BTS7960_ESP32() __attribute__((always_inline));
  /*===============================================================================================================================*/
};

//Static variables initilisation
String BTS7960_ESP32::version="";

//Default constructor
BTS7960_ESP32::BTS7960_ESP32(){}

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960_ESP32::BTS7960_ESP32(const int8_t& L_EN, const int8_t& R_EN, const int8_t& L_PWM, const int8_t& R_PWM, const uint32_t& freq, const uint8_t& resolution, const int8_t& L_IS, const int8_t& R_IS, const int8_t& ID, const bool& debugStatus) :
L_EN(L_EN),
R_EN(R_EN),
L_PWM(L_PWM),              //pin 5 has PWM frequency of 980Hz
R_PWM(R_PWM),              //pin 6 has PWM frequency of 980Hz
freq(freq),                //frequency set to 20 khz
resolution(resolution),    //12 bit resolution
L_IS(L_IS),                //Alarm pin
R_IS(R_IS),                //Alarm pin
ID(ID),                    //for seial monitor display
debugStatus(debugStatus)
{
  speed = 70;
  version = "1.0.0";
  oldMotorSpeed = 70;
  changeSpeedInc = 60;
  changeSpeedDec = 50;
}

void BTS7960_ESP32::begin()
{
  if(this->L_EN > 0)  pinMode(this->R_EN, OUTPUT);    //Motor driver enable pins set as output and high
  if(this->R_EN > 0)  pinMode(this->L_EN, OUTPUT);
  // if(this->L_PWM > 0) pinMode(this->L_PWM, OUTPUT);   //PWM is for direction and pwm
  // if(this->R_PWM > 0) pinMode(this->R_PWM, OUTPUT);
  if(this->R_IS > 0) pinMode(this->R_IS, INPUT);     //R_IS and L_IS alarm pins
  if(this->L_IS > 0)  pinMode(this->L_IS, INPUT);

  // Configure PWM channels
  // ledcSetup(this->PWMChannel1, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/
  // ledcSetup(this->PWMChannel2, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/

  // Attach PWM channels to pins
  ledcAttach(this->L_PWM, this->freq, this->resolution);
  ledcAttach(this->R_PWM, this->freq, this->resolution);
}

void BTS7960_ESP32::loop()
{
  switch(this->motorStatus)
  {
    case motorStates::FRONT:
    front();
    break;

    case motorStates::BACK:
    back();
    break;

    case motorStates::ENABLE:
    enable();
    break;

    case motorStates::DISABLE:
    disable();
    break;

    case motorStates::STOP:
    stop();
    break;
  }
}

void BTS7960_ESP32::enable()
{
  //Setting the BTS7960_ESP32 enable pins high
  digitalWrite(this->R_EN, HIGH);
  digitalWrite(this->L_EN, HIGH);

  if(this->debugStatus) Serial.println((String)this->ID+" initilized and enabled");
  return;
}

void BTS7960_ESP32::disable()
{
  //Setting the BTS7960_ESP32 enable pins low
  digitalWrite(this->R_EN, LOW);
  digitalWrite(this->L_EN, LOW);

  if(this->debugStatus) Serial.println((String)this->ID+" motor driver disabled");
  return;
}

void BTS7960_ESP32::stop()
{
  ledcWrite(this->L_PWM,this->DEFAULTSPEED);
  ledcWrite(this->R_PWM,this->DEFAULTSPEED);
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" STOP: "+this->DEFAULTSPEED);
}

void BTS7960_ESP32::front()
{
  if( this->prevMotorStatus != this->motorStatus )
  {
    ledcWrite(this->L_PWM,this->DEFAULTSPEED);
    delayMicroseconds(50);
    ledcWrite(this->R_PWM,this->speed);

    this->prevMotorStatus = this->motorStatus;
  }
  else
  {
    ledcWrite(this->L_PWM,this->DEFAULTSPEED);
    ledcWrite(this->R_PWM,this->speed);
  }
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" FRONT: "+this->speed);
}

void BTS7960_ESP32::back()
{
  if( this->prevMotorStatus != this->motorStatus )
  {
    ledcWrite(this->R_PWM,this->DEFAULTSPEED);
    delayMicroseconds(50);
    ledcWrite(this->L_PWM,this->speed);

    this->prevMotorStatus = this->motorStatus;
  }
  else
  {
    ledcWrite(this->R_PWM,this->DEFAULTSPEED);
    ledcWrite(this->L_PWM,this->speed);
  }
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" BACK: "+this->speed);
}

void BTS7960_ESP32::alarm()
{
  if(digitalRead(L_IS) || digitalRead(R_IS))
  {
      disable();
      if(this->debugStatus) Serial.println("High current alarm");
  }  
}

void BTS7960_ESP32::printInfo()
{
  Serial.println();
  Serial.println("BTS7960_ESP32 Motordriver library");
  Serial.println("Library version:");
  Serial.println(version);
  Serial.println("Yash Herekar 2023");
  
  delay(1000);
  return;
}

//Destructor
BTS7960_ESP32::~BTS7960_ESP32()
{
  ledcDetach(this->L_PWM);
  ledcDetach(this->R_PWM);
  if(this->debugStatus) Serial.println("motor object destroyed");
  return;
}