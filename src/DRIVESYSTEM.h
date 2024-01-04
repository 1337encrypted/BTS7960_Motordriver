#include <Ps3Controller.h>
#include "BTS7960_ESP32.h"
#include "LED.h"
#include "BUZZER_ESP32.h"
#include "CONFIG.h"

# pragma once 

class driveMode
{
  private: //PS3 variables
  uint8_t deviation;
  uint8_t player = 2;
  uint8_t battery = 0;

  // Create iBus Object
  BTS7960_ESP32 motor1;      //Create an object of class motor1
  BTS7960_ESP32 motor2;      //Create an object of class motor2 should have been LPWM2, RPWM2
  led redLed;                //Create object for red led
  led blueLed;               //Create object for blue led
  buzzer buzz;               //Create object for buzzer
  
  public:

  enum driveStates : uint8_t
  {
    mode1,
    mode2,
    mode3,
    mode4,
    none
  };
  driveStates driveStatus = driveStates::none;


  driveMode(BTS7960_ESP32 &, BTS7960_ESP32 &, led &, led &, buzzer &, uint8_t &, uint8_t &);
  void setSpeed();
  void begin();
  void driveMode1();
  void driveMode2();
  // virtual ~driveMode(){}
};

driveMode::driveMode(BTS7960_ESP32 &motor1, BTS7960_ESP32 &motor2, led &redLed, led &blueLed, buzzer &buzz, uint8_t &player, uint8_t &battery) : 
motor1(motor1),
motor2(motor2),
redLed(redLed),
blueLed(blueLed),
buzz(buzz),
player(player),
battery(battery)
{}

void driveMode::begin()
{

  motor1.begin();
  motor1.enable();

  motor2.begin();
  motor2.enable();

  blueLed.begin();
  redLed.begin();
  
  buzz.begin();

  Ps3.setPlayer(this->player);

  debug("Setting LEDs to player "); 
  // debugln(this->player, DEC);
}

void driveMode::setSpeed()
{
 if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = 205;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = 150;
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


void driveMode::driveMode1() 
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

  
void driveMode::driveMode2() 
{
  //---------------- Analog stick value events ---------------
  if((motor1.deviation = Ps3.data.analog.stick.lx) > 2 ) //move Right, upper limit = 127
  {
    motor1.deviation = static_cast<int>(motor1.changeSpeed / 127.0 * motor1.deviation);

    // Serial.println(motor1.deviation);



    motor1.speed = motor1.oldMotorSpeed + motor1.deviation;
    motor2.speed = motor1.oldMotorSpeed - motor1.deviation;
  }

  if((motor1.deviation = Ps3.data.analog.stick.lx) < -2 ) //move left, upper limit = -128
  {
    motor1.deviation = static_cast<int>(-(motor1.changeSpeed) / 128.0 * motor1.deviation);

    // Serial.println(motor1.deviation);



    motor1.speed = motor1.oldMotorSpeed - motor1.deviation;
    motor2.speed = motor1.oldMotorSpeed + motor1.deviation;
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