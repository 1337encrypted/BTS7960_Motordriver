/*
 * BTS7960_ESP32.h - Library for controlling a brushed DC motor using the BTS7960_ESP32 43amp motor driver.
 * Created by Yash Herekar, August 31, 2022.
 * Released into the public domain.
 * Copyright [2022] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#pragma once

class BTS7960_ESP32
{
  private:
  gpio_num_t R_EN;
  gpio_num_t L_EN;
  gpio_num_t R_PWM;
  gpio_num_t L_PWM;
  gpio_num_t R_IS;
  gpio_num_t L_IS;
  gpio_num_t ID;
  gpio_num_t resolution;

  uint32_t frequency;
  static String version;
  bool debugStatus;
  const gpio_num_t DEFAULTSPEED = 0;
  
  public:

  //pwm variable to control the speed of motor
  gpio_num_t speed;
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

  
  /*=============================================Function prototyping section=====================================================*/
  inline BTS7960_ESP32();
  inline BTS7960_ESP32(const gpio_num_t& = -1, const gpio_num_t& = -1, const gpio_num_t& = -1, const gpio_num_t& = -1, const uint32_t& = 10000, const gpio_num_t& = 8, const gpio_num_t& = -1, const gpio_num_t& = -1, const gpio_num_t& = -1, const bool& = false); // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, ID
  inline void begin();
  inline void enable();
  inline void disable();
  inline void front();
  inline void back();
  inline void stop();
  inline void alarm();
  inline static void printInfo();
  inline ~BTS7960_ESP32();
  /*===============================================================================================================================*/
};

#include "BTS7960_Motordriver.h"

//Static variables initilisation
String BTS7960_ESP32::version="";

//Default constructor
BTS7960_ESP32::BTS7960_ESP32(){}

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960_ESP32::BTS7960_ESP32(const gpio_num_t& L_EN, const gpio_num_t& R_EN, const gpio_num_t& L_PWM, const gpio_num_t& R_PWM, const uint32_t& frequency, const gpio_num_t& resolution, const gpio_num_t& L_IS, const gpio_num_t& R_IS, const gpio_num_t& ID, const bool& debugStatus) :
L_EN(L_EN),
R_EN(R_EN),
L_PWM(L_PWM),              
R_PWM(R_PWM),              
frequency(frequency),                //frequencyuency set to 10 khz
resolution(resolution),    //8 bit resolution
L_IS(L_IS),                //Alarm pin
R_IS(R_IS),                //Alarm pin
ID(ID),                    //for seial monitor display
debugStatus(debugStatus)
{
  version = "1.0.0";
  printInfo();
}

void BTS7960_ESP32::begin()
{
  if (this->L_EN > 0) {
    gpio_set_direction(this->L_EN, GPIO_MODE_OUTPUT);
} // Motor driver enable pins set as output and high

if (this->R_EN > 0) {
    gpio_set_direction(this->R_EN, GPIO_MODE_OUTPUT);
}

if (this->R_IS > 0) {
    gpio_set_direction(this->R_IS, GPIO_MODE_INPUT);
}

if (this->L_IS > 0) {
    gpio_set_direction(this->L_IS, GPIO_MODE_INPUT);
}


 
  // Attach PWM channels to pins
  ledcAttach(this->L_PWM, this->frequency, this->resolution);
  ledcAttach(this->R_PWM, this->frequency, this->resolution);
}

void BTS7960_ESP32::enable()
{
  //Setting the BTS7960_ESP32 enable pins high
  gpio_set_level(this->R_EN, 1);
  gpio_set_level(this->L_EN, 1);

  ESP_LOGD("Motor "+(String)ID, "initilized and enabled");
}

void BTS7960_ESP32::disable()
{
  //Setting the BTS7960_ESP32 enable pins low
  gpio_set_level(this->R_EN, 0);
  gpio_set_level(this->L_EN, 0);

  ESP_LOGD((String)this->ID, " motor driver disabled");
}

void BTS7960_ESP32::stop()
{
  ledcWrite(this->L_PWM,this->DEFAULTSPEED);
  ledcWrite(this->R_PWM,this->DEFAULTSPEED);
  
  ESP_LOGD("Motor "+(String)ID+" STOP: ", this->DEFAULTSPEED);
}

void BTS7960_ESP32::front()
{
  ledcWrite(this->L_PWM,this->DEFAULTSPEED);
  ledcWrite(this->R_PWM,this->speed);
  ets_delay_us(100);
  
  ESP_LOGD("Motor "+(String)ID+" FRONT: ", this->speed);
}

void BTS7960_ESP32::back()
{
  ledcWrite(this->L_PWM,this->speed);
  ledcWrite(this->R_PWM,this->DEFAULTSPEED);
  ets_delay_us(100);
  
  ESP_LOGD("Motor "+(String)ID+" BACK: ", this->speed);
}

void BTS7960_ESP32::alarm()
{
  if(digitalRead(L_IS) || digitalRead(R_IS))
  {
      disable();
      ESP_LOGD("Motor "+(String)ID, "High current alarm");
  }  
}

void BTS7960_ESP32::printInfo()
{
  ESP_LOGI("Motor "+(String)ID, "BTS7960_ESP32 Motordriver library");
  ESP_LOGI("Motor "+(String)ID, "Library version:");
  ESP_LOGI("Motor "+(String)ID, version);
  ESP_LOGI("Motor "+(String)ID, "Yash Herekar 2023");
  
  vTaskDelay(1000 / portTICK_PERIOD_MS) ;
}

//Destructor
BTS7960_ESP32::~BTS7960_ESP32()
{
  ledcDetach(this->L_PWM);
  ledcDetach(this->R_PWM);
  ESP_LOGI("Motor "+(String)ID, "motor object destroyed");
}

