// #include "DRIVESYSTEM.h"
#include <Ps3Controller.h>
#include "BTS7960_ESP32.h"
#include "LED.h"
#include "BUZZER_ESP32.h"
#include "CONFIG.h"


// Drive State machine 
enum class driveStates : uint8_t
{
  mode1,
  mode2,
  mode3,
  mode4,
  none
};
driveStates driveStatus = driveStates::none;

// Miscellaneous variables
bool initOnce = true;

//Motor PWM properties
const uint32_t freq = 1000;
const uint8_t resolution = 8;

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t leftMotorsId = 1;
constexpr uint8_t RPWM1 = 26;
constexpr uint8_t LPWM1 = 27;                   
constexpr uint8_t R_EN1 = 25;  
constexpr uint8_t L_EN1 = 14;
constexpr uint8_t R_IS1 = -1;        //Alarm pin
constexpr uint8_t L_IS1 = -1;        //Alarm pin

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t rightMotorsId = 2;
constexpr uint8_t RPWM2 = 15;         
constexpr uint8_t LPWM2 = 5;          
constexpr uint8_t R_EN2 = 16;
constexpr uint8_t L_EN2 = 4;
constexpr uint8_t R_IS2 = -1;        //Alarm pin
constexpr uint8_t L_IS2 = -1;        //Alarm pin 

//PS3 variables
uint8_t player = 2;
uint8_t battery = 0;
uint8_t stateCount = 0;

//Led definition section
//use between 150 ohms to 330 ohms resistor

//LED1
const String redLedId = "Red Led";
constexpr uint8_t redLedPin = 17;

// //LED2
const String blueLedId = "Blue Led";
constexpr uint8_t blueLedPin = 2;

//Buzzer definition section
constexpr uint8_t buzzId = 1;
constexpr uint8_t buzzPin = 21; //Active buzzer use 100 ohms resistor
constexpr uint8_t resolutionBuzz = 8;
constexpr uint32_t freqBuzz = 5000;

/*==================================================Object declaration===================================================*/  
// Create iBus Object
BTS7960_ESP32 motor1(L_EN1, R_EN1, LPWM1, RPWM1, freq, resolution, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG);       //Create an object of class motor1
BTS7960_ESP32 motor2(L_EN2, R_EN2, RPWM2, LPWM2, freq, resolution, L_IS2, R_IS2, rightMotorsId, MOTORDEBUG);      //Create an object of class motor2 should have been LPWM2, RPWM2
led redLed(redLedPin, freq, resolution, redLedId, LEDDEBUG);                                                      //Create object for red led
led blueLed(blueLedPin, freq, resolution, blueLedId, LEDDEBUG);                                                   //Create object for blue led
buzzer buzz(buzzId, buzzPin, resolutionBuzz, freqBuzz, BUZZERDEBUG);                                              //Create object for buzzer
// Drive System object
// driveMode mode(motor1, motor2, redLed, blueLed, buzz, player, battery);
/*==================================================Function section========================================================*/

void onConnect()
{
  debugln("Connected.");
}

//namespaces here


void begin()
{
  motor1.begin();
  motor1.enable();

  motor2.begin();
  motor2.enable();

  blueLed.begin();
  redLed.begin();
  
  buzz.begin();

  Ps3.setPlayer(player);

  debug("Setting LEDs to player "); 
  // debugln(this->player, DEC);
}

void setSpeed()
{
  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = 205;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = 150;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = 70;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = 100;
    buzz.alarm();
  }
}


void driveMode1() 
{
  /* ============================================= Motor 1 events ============================================= */

  if( Ps3.event.button_down.l1 )        /*------------- Digital left shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_down.l2 )        /*-------------- Digital left trigger button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  if( Ps3.event.button_up.l1 )   /*-------------- Digital left trigger button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }
  if( Ps3.event.button_up.l2 )   /*-------------- Digital left trigger button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }
  
  /* ============================================= Motor 2 events =============================================*/

  if( Ps3.event.button_down.r1 )      /*------------- Digital right shoulder button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  if( Ps3.event.button_down.r2 )      /*-------------- Digital right trigger button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  if( Ps3.event.button_up.r1 )  /*-------------- Digital right trigger button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }

  if(Ps3.event.button_up.r2)
  {
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }
}
  
void driveMode2() 
{
  //---------------- Analog stick value events ---------------
  if((motor1.deviation = motor2.deviation = Ps3.data.analog.stick.lx) > 2 ) //move Right, upper limit = 127
  {
    motor1.deviation = static_cast<int>(motor1.changeSpeedInc / 127.0 * motor1.deviation);
    motor2.deviation = static_cast<int>(motor2.changeSpeedDec / 127.0 * motor2.deviation);

    motor1.speed = motor1.oldMotorSpeed + motor1.deviation;
    motor2.speed = motor2.oldMotorSpeed - motor2.deviation;
  }

  if((motor1.deviation = motor2.deviation = Ps3.data.analog.stick.lx) < -2 ) //move left, upper limit = -128
  {
    motor2.deviation = static_cast<int>(-(motor2.changeSpeedInc) / 128.0 * motor2.deviation);
    motor1.deviation = static_cast<int>(-(motor1.changeSpeedDec) / 128.0 * motor1.deviation);

    motor2.speed = motor2.oldMotorSpeed + motor2.deviation;
    motor1.speed = motor1.oldMotorSpeed - motor1.deviation;
  }

  if( Ps3.event.button_down.r2 )      /*------------- Digital right shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_up.r2 )      /*------------- Digital right shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }

  if( Ps3.event.button_down.r1 )      /*------------- Digital right shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_up.r1 )
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  } 

    //--------------- Digital D-pad button events --------------
    // if( Ps3.event.button_down.up )
    //     Serial.println("Started pressing the up button");
    // if( Ps3.event.button_up.up )
    //     Serial.println("Released the up button");

    // if( Ps3.event.button_down.down )
    //     Serial.println("Started pressing the down button");
    // if( Ps3.event.button_up.down )
    //     Serial.println("Released the down button");

    if( Ps3.event.button_down.right )
    {
      // Serial.println("Started pressing the right button");
      motor1.motorStatus = motor1.motorStates::FRONT;
      motor2.motorStatus = motor2.motorStates::BACK;
    }
    if( Ps3.event.button_up.right )
    {
      // Serial.println("Released the left button");
      motor1.motorStatus = motor1.motorStates::STOP;
      motor2.motorStatus = motor2.motorStates::STOP;
    }       

    if( Ps3.event.button_down.left )
    {
      // Serial.println("Started pressing the left button");
      motor2.motorStatus = motor2.motorStates::FRONT;
      motor1.motorStatus = motor1.motorStates::BACK;
    }

    if( Ps3.event.button_up.left )
    {
      // Serial.println("Released the right button");
      motor1.motorStatus = motor1.motorStates::STOP;
      motor2.motorStatus = motor2.motorStates::STOP;
    } 
}
