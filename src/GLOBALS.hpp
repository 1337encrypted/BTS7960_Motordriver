#include "FlySkyIBus.h"
#include "BUZZER.hpp"
#include "BTS7960_Motordriver.hpp"
#include "tb6612fng.hpp"
#include "LED.hpp"
#include "CONFIG.hpp"

// Miscellaneous variables
bool initOnce = true;
const unsigned int DISCONNECT_TIMEOUT = 2000;  // Timeout duration in milliseconds
unsigned long currentTime = 0;
unsigned long lastReceiveTime = 0;
bool connOnce = true;


//Transmitter channels (previously uint16_t was long for all transmitter variables)
//uint16_t CH0 = 0;
//uint16_t CH1 = 0;
uint16_t CH2 = 0;
uint16_t CH3 = 0;
uint16_t CH4 = 0;
uint16_t CH5 = 0;
//bool CH4 = false;                     //Switch mode
//bool CH6 = false;                     //on off switch

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

//Led definition section
//use between 150 ohms to 330 ohms resistor

//tb6612fng motor driver 1 pin definitions
constexpr uint8_t gripperId = 1;
constexpr int8_t IN1 = -1;         //PWM 980hz
constexpr int8_t IN2 = -1;         //PWM 490hz
constexpr int8_t IN3 = -1;
constexpr int8_t IN4 = -1;        
constexpr int8_t pwmPin1 = -1; 
constexpr int8_t pwmPin2 = -1; 
constexpr int8_t standbyPin = -1; 

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

/*=====================================================  Object declaration=============================================================*/
FlySkyIBus ibus;                                                      // Create iBus Object
/*==================================================Object declaration===================================================*/  
// Create iBus Object
BTS7960_ESP32 motor1(L_EN1, R_EN1, LPWM1, RPWM1, frequency, resolution, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG); //Create an object of class motor1
BTS7960_ESP32 motor2(L_EN2, R_EN2, RPWM2, LPWM2, frequency, resolution, L_IS2, R_IS2, rightMotorsId, MOTORDEBUG);//Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin, frequency, resolution, redLedId, LEDDEBUG);                                                //Create object for red led
led blueLed(blueLedPin, frequency, resolution, blueLedId, LEDDEBUG);                                             //Create object for blue led
buzzer buzz(buzzId, buzzPin, resolutionBuzz, frequencyBuzz, BUZZERDEBUG);                                        //Create object for buzzer
tb6612fng gripper(IN1, IN2, IN3, IN4, pwmPin1, pwmPin2, standbyPin, frequency, resolution, gripperId);
