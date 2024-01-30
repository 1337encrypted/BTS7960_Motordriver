#pragma once 

/*
Pins used
blueLedPin = 2;
L_EN2 = 4;
LPWM2 = 5; 
L_EN1 = 14;
RPWM2 = 15; 
R_EN2 = 16;
redLedPin = 17;
buzzPin = 18;
R_EN1 = 25;  
RPWM1 = 26;
LPWM1 = 27;

Pins Available

*/

#include <Ps3Controller.h>
#include <Wire.h>
// #include <U8g2lib.h>
#include "MENU.h"
#include "BTS7960_ESP32.h"
#include "LED.h"
#include "BUZZER_ESP32.h"
#include "DRIVESYSTEM.h"
#include "CONFIG.h"

//Motor PWM properties
const uint32_t frequency = 10000;
const uint8_t resolution = 8;

//BTS7960 motor driver 2 pin definitions
constexpr int8_t leftMotorsId = 1;
constexpr int8_t RPWM1 = 26;
constexpr int8_t LPWM1 = 27;                   
constexpr int8_t R_EN1 = 25;  
constexpr int8_t L_EN1 = 14;
constexpr int8_t R_IS1 = -1;        //Alarm pin
constexpr int8_t L_IS1 = -1;        //Alarm pin

//BTS7960 motor driver 2 pin definitions
constexpr int8_t rightMotorsId = 2;
constexpr int8_t RPWM2 = 15;         
constexpr int8_t LPWM2 = 5;          
constexpr int8_t R_EN2 = 16;
constexpr int8_t L_EN2 = 4;
constexpr int8_t R_IS2 = -1;        //Alarm pin
constexpr int8_t L_IS2 = -1;        //Alarm pin 

//PS3 variables
uint8_t player = 0;
uint8_t battery = 0;
uint8_t stateCount = 0;

//Led definition section
//use between 150 ohms to 330 ohms resistor

//LED1
const String redLedId = "Red Led";
constexpr int8_t redLedPin = 17;

//LED2
const String blueLedId = "Blue Led";
constexpr int8_t blueLedPin = 2;

//Buzzer definition section
constexpr int8_t buzzId = 1;
constexpr int8_t buzzPin = 18; //Active buzzer use 100 ohms resistor
constexpr uint8_t resolutionBuzz = 8;
constexpr uint32_t frequencyBuzz = 5000;

/*==================================================Object declaration===================================================*/  
// Create iBus Object
BTS7960_ESP32 motor1(L_EN1, R_EN1, LPWM1, RPWM1, frequency, resolution, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG);     //Create an object of class motor1
BTS7960_ESP32 motor2(L_EN2, R_EN2, RPWM2, LPWM2, frequency, resolution, L_IS2, R_IS2, rightMotorsId, MOTORDEBUG);    //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin, frequency, resolution, redLedId, LEDDEBUG);                                                    //Create object for red led
led blueLed(blueLedPin, frequency, resolution, blueLedId, LEDDEBUG);                                                 //Create object for blue led
buzzer buzz(buzzId, buzzPin, resolutionBuzz, frequencyBuzz, BUZZERDEBUG);                                            //Create object for buzzer
driveMode mode(motor1, motor2, redLed, blueLed, buzz, player);                                                       // Drive System object
/*==================================================Function section========================================================*/