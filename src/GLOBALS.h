#include "FlySkyIBus.h"
#include "BUZZER.h"
#include "BTS7960_Motordriver.h"
// #include "L298N_Motordriver.h"
#include "LED.h"
#include "CONFIG.h"

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

//BTS7960 motor driver 2 pin definitions
const String leftMotorsId = "Left Motor";
constexpr uint8_t RPWM1 = 5;         //PWM 980hz
constexpr uint8_t LPWM1 = 6;         //PWM 490hz
constexpr uint8_t R_EN1 = 7;  
constexpr uint8_t L_EN1 = 8;
constexpr uint8_t R_IS1 = -1;        //Alarm pin
constexpr uint8_t L_IS1 = -1;        //Alarm pin


//BTS7960 motor driver 2 pin definitions
const String rightMotorsId = "Right Motor";
constexpr uint8_t RPWM2 = 10;         //PWM 980hz
constexpr uint8_t LPWM2 = 9;        //PWM 490hz
constexpr uint8_t R_EN2 = A1;
constexpr uint8_t L_EN2 = A0;
constexpr uint8_t R_IS2 = -1;        //Alarm pin
constexpr uint8_t L_IS2 = -1;        //Alarm pin

//Led definition section
//use between 150 ohms to 330 ohms resistor

//L298N motor driver 1 pin definitions
const String gripperId = "gripper";
constexpr uint8_t IN3 = A5;         //PWM 980hz
constexpr uint8_t IN4 = 12;         //PWM 490hz
constexpr uint8_t ENA = -1;  

//L298N motor driver 2 pin definitions
const String clenchId = "clench";
constexpr uint8_t IN1 = 13;
constexpr uint8_t IN2 = A2;        //Alarm pin
constexpr uint8_t ENB = 11;        //Alarm pin

uint8_t clenchPwm = 0;

//LED1
const String redLedId = "Red Led";
constexpr uint8_t redLedPin = A4;

//LED2
const String blueLedId = "Blue Led";
constexpr uint8_t blueLedPin = 3;

//Buzzer definition section
const String buzzId = "Buzzer";
constexpr uint8_t buzzpin = 2; //Active buzzer use 100 ohms resistor

/*=====================================================  Object declaration=============================================================*/
FlySkyIBus ibus;                                                      // Create iBus Object
BTS7960 motor1(L_EN1, R_EN1, LPWM1, RPWM1, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG); //Create an object of class motor1
BTS7960 motor2(L_EN2, R_EN2, LPWM2, RPWM2, L_IS1, R_IS2, rightMotorsId, MOTORDEBUG);//Create an object of class motor2 should have been LPWM2, RPWM2
// L298N gripper(IN1, IN2, ENA, gripperId, MOTORDEBUG);                                //Create an object of L298N class
// L298N clench(IN3, IN4, ENB, clenchId, MOTORDEBUG);                                  //Create an object of L298N class
led redLed(redLedPin, redLedId, LEDDEBUG);                                          //Create object for red led
led blueLed(blueLedPin, blueLedId, LEDDEBUG);                                       //Create object for blue led
buzzer buzz(buzzpin, buzzId, BUZZERDEBUG);                                          //Create object for buzzer
/*++++++++++++++++++++++++++++++++++++++++++++++++MOTOR STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
  DISCONNECT = 'Q',
  FRONTLIGHTSON = 'W',
  FRONTLIGHTSOFF = 'w',
  BACKLIGHTSON = 'U',
  BACKLIGHTSOFF = 'u',
  HORNON = 'V',
  HORNOFF = 'v',
  EXTRAON = 'X',
  EXTRAOFF = 'x'
};
motorStates motorStatus = motorStates::STOP;                        //State variable set to STOP initially
//motorStates motorPrevStatus;                                         //Previous motor state

/*++++++++++++++++++++++++++++++++++++++++++++++++L298N STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

enum class L298NStates : uint8_t
{
  GRIPPEROPEN,
  GRIPPERCLOSE,
  GRIPPERUP,
  GRIPPERDOWN,
  STOP
};
L298NStates L298NStatus = L298NStates::STOP;                        //State variable set to STOP initially

/*=========================================Function prototyping section===============================================*/
inline void initSystem() __attribute__((always_inline));
inline void standbySystem() __attribute__((always_inline));
/*====================================================================================================================*/

void initSystem()
{
  //Initlization sequence
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
  motor1.disable(); motor2.disable();
  blueLed.off();  redLed.off();
  if(motorStatus == motorStates::STOPALL)
  {
    debugln("Receiver standby mode...");
    buzz.deinitBuzzer();  
  }
  else if(motorStatus == motorStates::DISCONNECT)
  {
    Serial.println("Transmitter disconnected");
  }
}

//namespaces here
