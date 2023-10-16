/*
 * BTS7960_ESP32.h - Library for controlling a brushed DC motor using the BTS7960_ESP32 43amp motor driver.
 * Created by Yash Herekar, August 31, 2022.
 * Released into the public domain.
 * Copyright [2022] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

/*Cannot create a .cpp file as  prototypes need the function to be present in the same file*/

#include "CONFIG.h"

#ifndef BTS7960_ESP32_h
#define BTS7960_ESP32_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class BTS7960_ESP32
{
    private:
    uint8_t R_EN, L_EN, R_PWM, L_PWM, R_IS, L_IS, ID, resolution, PWMChannel1, PWMChannel2;
    uint32_t freq;
    static String version;
    const int DEFAULTSPEED = 0;
    
    public:

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
      UNFLIP = 'U',
      SPEED
    };
    motorStates motorStatus = motorStates::STOPALL;                        //State variable set to STOP initially
    motorStates ENABLEStatus = motorStates::ENABLE;                        //Motor Enable state
    motorStates FLIPStatus = motorStates::UNFLIP;                          //Motor FLIP state

    //pwm variable to control the speed of motor
    uint8_t speed;
    
    /*=============================================Function prototyping section=====================================================*/
    inline BTS7960_ESP32();
    inline BTS7960_ESP32(uint8_t, uint8_t, uint8_t, uint8_t, uint32_t=-1, uint8_t=12, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1); // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, ID
    inline void begin() __attribute__((always_inline));
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
BTS7960_ESP32::BTS7960_ESP32(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM, uint32_t freq, uint8_t resolution, uint8_t PWMChannel1, uint8_t PWMChannel2, uint8_t L_IS, uint8_t R_IS, uint8_t ID)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;            //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;            //pin 6 has PWM frequency of 980Hz
    this->freq = freq;              //frequency set to 20 khz
    this->resolution = resolution;  //12 bit resolution
    this->PWMChannel1 = PWMChannel1;  //channel for PWM
    this->PWMChannel2 = PWMChannel2;  //channel for PWM
    this->L_IS = L_IS;              //Alarm pin
    this->R_IS = R_IS;              //Alarm pin
    this->ID = ID;                  //for seial monitor display
    this->speed = 155;
    this->version = "1.0.0";
    
    begin();
}

void BTS7960_ESP32::begin()
{
    if(this->L_EN > 0)  pinMode(this->R_EN, OUTPUT);    //Motor driver enable pins set as output and high
    if(this->R_EN > 0)  pinMode(this->L_EN, OUTPUT);
    if(this->L_PWM > 0) pinMode(this->L_PWM, OUTPUT);   //PWM is for direction and pwm
    if(this->R_PWM > 0) pinMode(this->R_PWM, OUTPUT);
    if(this->R_IS > 0 ) pinMode(this->R_IS, INPUT);     //R_IS and L_IS alarm pins
    if(this->L_IS > 0)  pinMode(this->L_IS, INPUT);

    // Configure PWM channels
    ledcSetup(this->PWMChannel1, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/
    ledcSetup(this->PWMChannel2, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/

    // Attach PWM channels to pins
    ledcAttachPin(this->L_PWM, this->PWMChannel1);
    ledcAttachPin(this->R_PWM, this->PWMChannel2);
    
    enable();
}

void BTS7960_ESP32::enable()
{
    //Setting the BTS7960_ESP32 enable pins high
    digitalWrite(this->R_EN, HIGH);
    digitalWrite(this->L_EN, HIGH);
    debugln((String)this->ID+" initilized and enabled");
}

void BTS7960_ESP32::disable()
{
    //Setting the BTS7960_ESP32 enable pins high
    digitalWrite(this->R_EN, LOW);
    digitalWrite(this->L_EN, LOW);
    debugln("Motor driver disabled");
}

void BTS7960_ESP32::stop()
{
    ledcWrite(this->PWMChannel1,this->DEFAULTSPEED);
    ledcWrite(this->PWMChannel2,this->DEFAULTSPEED);
    debugln("Motor "+(String)ID+" STOP: "+this->DEFAULTSPEED);
}

void BTS7960_ESP32::front()
{
    ledcWrite(this->PWMChannel1,this->DEFAULTSPEED);
    ledcWrite(this->PWMChannel2,this->speed);
    debugln("Motor "+(String)ID+" FRONT: "+this->speed);
}

void BTS7960_ESP32::back()
{
    ledcWrite(this->PWMChannel1,this->speed);
    ledcWrite(this->PWMChannel2,this->DEFAULTSPEED);
    debugln("Motor "+(String)ID+" BACK: "+this->speed);
}

void BTS7960_ESP32::alarm()
{
    if(digitalRead(L_IS) || digitalRead(R_IS))
    {
        disable();
        debugln("High current alarm");
    }  
}

void BTS7960_ESP32::printInfo()
{
  debugln();
  debugln("BTS7960_ESP32 Motordriver library");
  debugln("Library version:");
  debugln(version);
  debugln("Yash Herekar 2023");
  
  delay(1000);
}

//Destructor
BTS7960_ESP32::~BTS7960_ESP32()
{
  debugln("motor object destroyed");
}

#endif  //END BTS7960_ESP32_H
