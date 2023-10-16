/*
 * BTS7960.h - Library for controlling a brushed DC motor using the BTS7960 43amp motor driver.
 * Created by Yash Herekar, August 31, 2022.
 * Released into the public domain.
 * Copyright [2022] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

/*Cannot create a .cpp file as  prototypes need the function to be present in the same file*/

#ifndef BTS7960_h
#define BTS7960_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class BTS7960
{
    private:
    uint8_t R_EN;
    uint8_t L_EN;
    uint8_t R_PWM;
    uint8_t L_PWM;
    uint8_t R_IS;
    uint8_t L_IS;
    bool debug;
    String id;
    static String version;
    public:

    //pwm variable to control the speed of motor
    volatile uint8_t pwm;
    
    /*=============================================Function prototyping section=====================================================*/
    inline BTS7960(uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, String id = "", bool=false); // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, _debug
    inline void begin() __attribute__((always_inline));
    inline void enable() __attribute__((always_inline));
    inline void disable() __attribute__((always_inline));
    inline void front() __attribute__((always_inline));
    inline void back() __attribute__((always_inline));
    inline void stop() __attribute__((always_inline));
    inline void alarm() __attribute__((always_inline));
    inline void printPWM() __attribute__((always_inline));
    inline void printDriverStatus() __attribute__((always_inline));
    inline static void printInfo() __attribute__((always_inline));
    
    inline ~BTS7960() __attribute__((always_inline));
    /*===============================================================================================================================*/
};

//Static variables initilisation

    String BTS7960::version="";

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM, uint8_t L_IS, uint8_t R_IS, String id, bool debug)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz
    this->L_IS = L_IS;      //Alarm pin
    this->R_IS = R_IS;      //Alarm pin
    this->debug = debug;   //for seial monitor display
    
    //Set the global pwm variable to 255
    this->pwm = 255;

    this->id = id;
    this->version = "1.0.0";
    
    begin();
    enable();
}

void BTS7960::begin()
{
    //Motor driver enable pins set as output and high
    if(this->L_EN != -1 && this->R_EN != -1)
    {
      pinMode(this->R_EN, OUTPUT);
      pinMode(this->L_EN, OUTPUT);
    }

    //PWM is for direction and pwm
    if(this->L_PWM != -1 && this->R_PWM != -1)
    {
        pinMode(this->L_PWM, OUTPUT);
        pinMode(this->R_PWM, OUTPUT);
    }
    
    //R_IS and L_IS alarm pins
    if(this->R_IS != -1 && this->L_IS != -1)
    {
        pinMode(this->R_IS, INPUT);
        pinMode(this->L_IS, INPUT);
    }
}

void BTS7960::enable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(this->R_EN, HIGH);
    digitalWrite(this->L_EN, HIGH);
}

void BTS7960::disable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(this->R_EN, LOW);
    digitalWrite(this->L_EN, LOW);
    
    if(this->debug)
    {
        Serial.println("Motor driver disabled");
    }
}

void BTS7960::stop()
{
    analogWrite(this->L_PWM,0);
    analogWrite(this->R_PWM,0);
    
    printPWM();
}

void BTS7960::front()
{
    analogWrite(this->L_PWM,0);
//    delayMicroseconds(100);
    analogWrite(this->R_PWM,pwm);
//    delayMicroseconds(100);
    printPWM();
}

void BTS7960::back()
{
    analogWrite(this->L_PWM,pwm);
//    delayMicroseconds(100);
    analogWrite(this->R_PWM,0);
//    delayMicroseconds(100);

    printPWM();
}

void BTS7960::alarm()
{
    if(digitalRead(L_IS) || digitalRead(R_IS))
    {
        disable();
        
        if(debug)
        {
            Serial.print("High current alarm");
        }
    }
}

//PrintPWM
void BTS7960::printPWM()
{
  if(this->debug)
    {
        Serial.print(pwm);
    }  
}

void BTS7960::printInfo()
{
  Serial.println();
  Serial.println("BTS7960 Motordriver library");
  Serial.print("Library version:");
  Serial.println(version);
  Serial.println("Yash Herekar 2022");
  
  delay(1000);
}

void BTS7960::printDriverStatus()
{
  if(this->debug)
  {
    Serial.println(id+" initilized and enabled");
    delay(1000);
  }
}

//Destructor
BTS7960::~BTS7960()
{
    if(this->debug)
    {
        Serial.println("motor object destroyed");
    }
}

#endif  //END BTS7960_H
