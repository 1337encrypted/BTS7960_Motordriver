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

#ifndef L298N_h
#define L298N_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class L298N
{
  private:

  uint8_t IN1;
  uint8_t IN2;
  uint8_t ENA;
  String id;
  bool debug;
 
  public:
  uint8_t pwm;

  inline L298N(uint8_t=-1, uint8_t=-1, uint8_t=-1, String="", bool=false) __attribute__((always_inline));
  inline void begin() __attribute__((always_inline));
  inline void enable() __attribute__((always_inline));
  inline void disable() __attribute__((always_inline));
  inline void front() __attribute__((always_inline));
  inline void back() __attribute__((always_inline));
  inline void stop() __attribute__((always_inline));
  inline ~L298N() __attribute__((always_inline));
};

L298N::L298N(uint8_t IN1, uint8_t IN2, uint8_t ENA, String id, bool debug)
{
    //Motor driver 1 pin definitions
    this->IN1 = IN1;
    this->IN2 = IN2;
    this->ENA = ENA;        //pin 5 has PWM frequency of 980Hz
    this->id = id;
    this->debug = debug;   //for seial monitor display
    
    //Set the global pwm variable to 255
    this->pwm = 255;        //pin 5 has PWM frequency of 980Hz
    
    begin();
    enable();
}
  
void L298N::begin()
{
  //Motor driver enable pins set as output and high
  if(this->IN1 > 0)   pinMode(this->IN1, OUTPUT);
  if(this->IN2 > 0)   pinMode(this->IN2, OUTPUT);
  if(this->ENA > 0)   pinMode(this->ENA, OUTPUT);
}

void L298N::enable()
{
    //Setting the L298N enable pins high
    digitalWrite(this->ENA, HIGH);
    if(this->debug)   Serial.println("Motor driver enabled");
}

void L298N::disable()
{
    //Setting the L298N enable pins high
    digitalWrite(this->ENA, LOW);
    if(this->debug)   Serial.println("Motor driver disabled");
}


void L298N::stop()
{
  digitalWrite(this->IN1,LOW);
  digitalWrite(this->IN2,LOW);
  if(this->ENA != -1) analogWrite(this->ENA,0);
}

void L298N::front()
{
  digitalWrite(this->IN1,HIGH);
  digitalWrite(this->IN2,LOW);
  if(this->ENA != -1) analogWrite(this->ENA,this->pwm);
}

void L298N::back()
{
  digitalWrite(this->IN1,LOW);
  digitalWrite(this->IN2,HIGH);
  if(this->ENA != -1) analogWrite(this->ENA,this->pwm);
}

L298N::~L298N()
{
  if(this->debug)   Serial.println("Motor driver destroyed");
}

#endif  //L298N
