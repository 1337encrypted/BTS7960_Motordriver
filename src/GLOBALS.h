#include "BUZZER.h"
#include "BTS7960_Motordriver.h"
#include "LED.h"
#include "CONFIG.h"

// Miscellaneous variables
bool initOnce = true;

//BTS7960 motor driver 2 pin definitions
const String leftMotorsId = "Left Motors";
constexpr uint8_t R_EN1 = 7;  
constexpr uint8_t L_EN1 = 8;
constexpr uint8_t RPWM1 = 6;           //PWM 490hz
constexpr uint8_t LPWM1 = 5;           //PWM 980hz
constexpr uint8_t R_IS1 = -1;          //Alarm pin
constexpr uint8_t L_IS1 = -1;          //Alarm pin

//BTS7960 motor driver 2 pin definitions
const String rightMotorsId = "Right Motors";
constexpr uint8_t R_EN2 = A0;
constexpr uint8_t L_EN2 = A1;
constexpr uint8_t RPWM2 = 9;            //PWM 980hz
constexpr uint8_t LPWM2 = 10;           //PWM 490hz
constexpr uint8_t R_IS2 = -1;           //Alarm pin
constexpr uint8_t L_IS2 = -1;           //Alarm pin

//Led definition section
//use between 150 ohms to 330 ohms resistor

//LED1
const String redLedId = "Red Led";
constexpr uint8_t redLedPin = A4;

//LED2
const String blueLedId = "Blue Led";
constexpr uint8_t blueLedPin = 3;

//Buzzer definition section
const String buzzId = "Buzzer";
#define buzzpin 2 //Active buzzer use 100 ohms resistor

/*=====================================================  Object declaration=============================================================*/
BTS7960 motor1(L_EN1, R_EN1, LPWM1, RPWM1, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG);       //Create an object of class motor1
BTS7960 motor2(L_EN2, R_EN2, RPWM2, LPWM2, L_IS1, R_IS2, rightMotorsId, MOTORDEBUG);      //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin, redLedId, LEDDEBUG);                                                //Create object for red led
led blueLed(blueLedPin, blueLedId, LEDDEBUG);                                             //Create object for blue led
buzzer buzz(buzzpin, buzzId, BUZZERDEBUG);                                               //Create object for buzzer
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++MOTOR STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

enum class motorStates : uint8_t
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
   STOPALL = 'D',
   FRONTLIGHTSON = 'W',
   FRONTLIGHTSOFF = 'w',
   BACKLIGHTSON = 'U',
   BACKLIGHTSOFF = 'u',
   HORNON = 'V',
   HORNOFF = 'v',
   EXTRAON = 'X',
   EXTRAOFF = 'x',
   SPEED0 = '0',
   SPEED1 = '1',
   SPEED2 = '2',
   SPEED3 = '3',
   SPEED4 = '4',
   SPEED5 = '5',
   SPEED6 = '6',
   SPEED7 = '7',
   SPEED8 = '8',
   SPEED9 = '9',
   MAXSPEED = 'q',
   
};
motorStates motorStatus = motorStates::STOPALL;                        //State variable set to STOP initially
//motorStates motorPrevStatus;                                         //Previous motor state

/*==================================================Function prototyping section========================================================*/
inline void initSystem() __attribute__((always_inline));
inline void standbySystem() __attribute__((always_inline));
/*======================================================================================================================================*/

void initSystem()
{
  //Initlization sequence
  debugln("System initlized, waiting for Transmitter...");
  motor1.enable();  motor2.enable();                                  //Makes all enable pins go high
  blueLed.on();                                                       //Turns the blue led on
  redLed.on();                                                        //Turns the red led on
  buzz.initBuzzer();                                                  //puts the buzzer on

  if(initOnce && SYSTEMDEBUG)
  {
    BTS7960::printInfo();
    debugln();

    buzz.alarm();
    motor1.printDriverStatus();
    debugln();

    buzz.alarm();
    motor2.printDriverStatus();
    debugln();
    
    buzz.alarm();
    redLed.printInfo();
    debugln();
    
    buzz.alarm();
    blueLed.printInfo();
    debugln();
    
    buzz.alarm();
    buzz.printInfo();
    debugln();
    
    buzz.initBuzzer();
    debugln("System initlized, waiting for Transmitter...");
    delay(3000);
    buzz.off();
    initOnce = false;
  }

  //Ready state
  blueLed.off();                                                      //Turns the blue led on
  redLed.on();                                                        //Turns the red led on

  //Initial statemachine setting
  motorStatus = motorStates::STOP;                                    //State variable set to STOP initially
}

void standbySystem()
{
  debugln("Receiver disconnected...");
  motor1.disable(); motor2.disable();
  blueLed.off();  redLed.off();
  buzz.deinitBuzzer();  
}

//namespaces here
