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

#pragma once

class BTS7960_ESP32
{
  private:
  uint8_t R_EN;
  uint8_t L_EN;
  uint8_t R_PWM;
  uint8_t L_PWM;
  uint8_t R_IS;
  uint8_t L_IS;
  uint8_t ID;
  uint8_t resolution;

  uint32_t freq;
  static String version;
  bool debugStatus;
  const uint8_t DEFAULTSPEED = 0;
  
  public:

  //pwm variable to control the speed of motor
  uint8_t speed;
  int deviation;
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
  // motorStates ENABLEStatus = motorStates::ENABLE;                     //Motor Enable state
  // motorStates FLIPStatus = motorStates::UNFLIP;                       //Motor FLIP state

  
  /*=============================================Function prototyping section=====================================================*/
  inline BTS7960_ESP32();
  inline BTS7960_ESP32(const uint8_t& = -1, const uint8_t& = -1, const uint8_t& = -1, const uint8_t& = -1, const uint32_t& = 1000, const uint8_t& = 8, const uint8_t& = -1, const uint8_t& = -1, const uint8_t& = -1, const bool& = false); // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, ID
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
