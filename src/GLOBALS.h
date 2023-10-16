#include <Ps3Controller.h>
#include "BTS7960_ESP32.h"
#include "LED.h"
#include "BUZZER_ESP32.h"
#include "CONFIG.h"

// Miscellaneous variables
bool initOnce = true;

//Motor PWM properties
const uint32_t freq = 1000;
const uint8_t resolution = 8;

const float MAXSPEED = 255;
const float STICKUPVAL = 128;
const float STICKDOWNVAL = 127;
const uint8_t DeadBandPosVal = 10;
const int8_t DeadBandNegVal = -10;

//Check this value everytime you make changes
float uSpeed = MAXSPEED/STICKUPVAL;
float dSpeed = MAXSPEED/STICKDOWNVAL;

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t leftMotorsId = 1;
constexpr uint8_t RPWM1 = 36;
constexpr uint8_t LPWM1 = 39;                   
constexpr uint8_t R_EN1 = 27;  
constexpr uint8_t L_EN1 = 34;
constexpr uint8_t R_IS1 = -1;        //Alarm pin
constexpr uint8_t L_IS1 = -1;        //Alarm pin
constexpr uint8_t PWMChannel1 = 1;
constexpr uint8_t PWMChannel2 = 2; 

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t rightMotorsId = 2;
constexpr uint8_t RPWM2 = 15;         
constexpr uint8_t LPWM2 = 2;          
constexpr uint8_t R_EN2 = 16;
constexpr uint8_t L_EN2 = 4;
constexpr uint8_t R_IS2 = -1;        //Alarm pin
constexpr uint8_t L_IS2 = -1;        //Alarm pin
constexpr uint8_t PWMChannel3 = 3; 
constexpr uint8_t PWMChannel4 = 4; 

//PS3 variables
int player = 0;
int battery = 0;

//Joystick helper values
int rightY=0,leftY=0;

//Led definition section
//use between 150 ohms to 330 ohms resistor

//LED1
const String redLedId = "Red Led";
constexpr uint8_t redLedPin = 17;

// //LED2
// const String blueLedId = "Blue Led";
// constexpr uint8_t blueLedPin = 3;

//Buzzer definition section
constexpr uint8_t buzzId = 1;
constexpr uint8_t buzzpin = 3; //Active buzzer use 100 ohms resistor

/*=====================================================  Object declaration=============================================================*/                                                   // Create iBus Object
BTS7960_ESP32 motor1(L_EN1, R_EN1, LPWM1, RPWM1, freq, resolution, PWMChannel1, PWMChannel2, L_IS1, R_IS1, leftMotorsId);       //Create an object of class motor1
BTS7960_ESP32 motor2(L_EN2, R_EN2, RPWM2, LPWM2, freq, resolution, PWMChannel3, PWMChannel4, L_IS2, R_IS2, rightMotorsId);      //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin, redLedId, LEDDEBUG);                                                                   //Create object for red led
// led blueLed(blueLedPin, blueLedId, LEDDEBUG);                                                                //Create object for blue led
buzzer buzz(buzzpin, buzzId, BUZZERDEBUG);                                                                   //Create object for buzzer

/*==================================================Function section========================================================*/
void onConnect()
{
    debugln("Connected.");
}

//namespaces here
