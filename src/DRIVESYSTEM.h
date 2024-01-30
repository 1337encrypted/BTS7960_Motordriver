# pragma once 

#include <Ps3Controller.h>
#include "BTS7960_ESP32.h"
#include "LED.h"
#include "BUZZER_ESP32.h"
#include "CONFIG.h"


class driveMode
{
  private:
  uint8_t player;

  public:

  // Create iBus Object
  BTS7960_ESP32 motor1;      //Create an object of class motor1
  BTS7960_ESP32 motor2;      //Create an object of class motor2 should have been LPWM2, RPWM2
  led redLed;                //Create object for red led
  led blueLed;               //Create object for blue led
  buzzer buzz;               //Create object for buzzer
  
  enum driveStates : uint8_t
  {
    mode1,
    mode2,
    mode3,
    mode4,
    none
  };
  driveStates driveStatus = driveStates::none;


  driveMode(const BTS7960_ESP32 &, const BTS7960_ESP32 &, const led &, const led &, const buzzer &, const uint8_t &);
  void begin();
  void loop();
  void driveMode1();
  void driveMode2();
  void driveMode3();
  void driveMode4();
  void disconnectedBlink();
  // virtual ~driveMode(){}
};

driveMode::driveMode(const BTS7960_ESP32& motor1, const BTS7960_ESP32& motor2, const led& redLed, const led& blueLed, const buzzer& buzz, const uint8_t& player) : 
motor1(motor1),
motor2(motor2),
redLed(redLed),
blueLed(blueLed),
buzz(buzz),
player(player)
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

  Ps3.setPlayer(player);
}

void driveMode::loop()
{
  switch(this->driveStatus)
  {
    case driveStates::mode1:
    driveMode1();             //Normal mode
    break;

    case driveStates::mode2:
    driveMode2();             //Speed adaptive mode
    break;

    case driveStates::mode3:
    driveMode3();             //Performance mode
    break;

    case driveStates::mode4:
    driveMode4();             //Performance mode
    break;

    case driveStates::none:
    disconnectedBlink();
    break;
  }
}


void driveMode::driveMode4() 
{
  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed= 70;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed = 100;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = 150;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = 205;
    buzz.alarm();
  }
  
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
  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed= 70;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed = 100;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = 150;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = 205;
    buzz.alarm();
  }

  //---------------- Analog stick value events ---------------
  if(Ps3.data.analog.stick.lx > 2 ) //move Right, upper limit = 127
  {
    motor1.deviation = static_cast<int8_t>(motor1.changeSpeedInc / 127.0 * Ps3.data.analog.stick.lx);
    motor2.deviation = static_cast<int8_t>(motor2.changeSpeedDec / 127.0 * Ps3.data.analog.stick.lx);

    motor1.speed = motor1.oldMotorSpeed + motor1.deviation;
    motor2.speed = motor2.oldMotorSpeed - motor2.deviation;
  }

  if(Ps3.data.analog.stick.lx < -2 ) //move left, upper limit = -128
  {
    motor2.deviation = static_cast<int8_t>(-(motor2.changeSpeedInc) / 128.0 * Ps3.data.analog.stick.lx);
    motor1.deviation = static_cast<int8_t>(-(motor1.changeSpeedDec) / 128.0 * Ps3.data.analog.stick.lx);

    motor2.speed = motor2.oldMotorSpeed + motor2.deviation;
    motor1.speed = motor1.oldMotorSpeed - motor1.deviation;
  }

  if( Ps3.event.button_down.l2 )      /*------------- Digital left shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_up.l2 )      /*------------- Digital left shoulder button event -------------*/
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

  if( Ps3.data.analog.button.right > 5 )
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

  if( Ps3.data.analog.button.left > 5 )
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

void driveMode::driveMode3()
{
  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed = 130;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed = 150;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed = 170;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = motor1.oldMotorSpeed = motor2.oldMotorSpeed= 195;
    buzz.alarm();
  }
  //---------------- Analog stick value events ---------------
  if(Ps3.data.analog.stick.lx > 2 ) //move Right, upper limit = 127
  {
    motor1.deviation = static_cast<int8_t>(motor1.changeSpeedInc / 127.0 * Ps3.data.analog.stick.lx);
    motor2.deviation = static_cast<int8_t>(motor2.changeSpeedDec / 127.0 * Ps3.data.analog.stick.lx);

    motor1.speed = motor1.oldMotorSpeed + motor1.deviation;
    motor2.speed = motor2.oldMotorSpeed - motor2.deviation;
  }

  if((motor1.deviation = motor2.deviation = Ps3.data.analog.stick.lx) < -2 ) //move left, upper limit = -128
  {
    motor2.deviation = static_cast<int8_t>(-(motor2.changeSpeedInc) / 128.0 * Ps3.data.analog.stick.lx);
    motor1.deviation = static_cast<int8_t>(-(motor1.changeSpeedDec) / 128.0 * Ps3.data.analog.stick.lx);

    motor2.speed = motor2.oldMotorSpeed + motor2.deviation;
    motor1.speed = motor1.oldMotorSpeed - motor1.deviation;
  }

  if( Ps3.event.button_down.l2 )      /*------------- Digital left shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_up.l2 )      /*------------- Digital left shoulder button event -------------*/
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

  if( Ps3.data.analog.button.right > 5 )
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

  if( Ps3.data.analog.button.left > 5 )
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

void driveMode::driveMode1()
{    

  if( Ps3.event.button_up.r1 )
  {
    motor1.speed = motor2.speed = 0;
    motor1.motorStatus = motor1.motorStates::STOP;
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }

  if( Ps3.data.analog.button.l1 > 5 )
  {
    motor1.speed = motor2.speed = Ps3.data.analog.button.l1;
    motor1.motorStatus = motor1.motorStates::FRONT;
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  if( Ps3.event.button_up.l1 )
  {
    motor1.speed = motor2.speed = 0;
    motor1.motorStatus = motor1.motorStates::STOP;
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }   
  if( Ps3.data.analog.button.r1 > 5)
  {
    motor1.speed = motor2.speed = Ps3.data.analog.button.r1;
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }

  //   //---------------- Analog stick value events ---------------
  // if(Ps3.data.analog.stick.ry > 10 ) //move Right, upper limit = 127
  // {
  //   motor2.speed = motor2.speed = static_cast<int8_t>((255 / 127.0) * Ps3.data.analog.stick.ry); 
  //   motor1.motorStatus = motor1.motorStates::BACK;
  //   motor2.motorStatus = motor2.motorStates::BACK;
  //   redLed.ledStatus = redLed.ledStates::ON;
  //   blueLed.ledStatus = blueLed.ledStates::OFF;

  // }

  // else if(Ps3.data.analog.stick.ry < -10 ) //move left, upper limit = -128
  // {
  //   motor2.speed = motor2.speed = static_cast<int8_t>((-255 / 128.0) * Ps3.data.analog.stick.ry);

  //   motor1.motorStatus = motor1.motorStates::FRONT;
  //   motor2.motorStatus = motor2.motorStates::FRONT;
  //   redLed.ledStatus = redLed.ledStates::OFF;
  //   blueLed.ledStatus = blueLed.ledStates::ON;
  // }

  // else
  // {
  //   // motor2.speed = motor2.speed = static_cast<int8_t>((-255 / 128.0) * Ps3.data.analog.stick.ry);

  //   motor1.motorStatus = motor1.motorStates::STOP;
  //   motor2.motorStatus = motor2.motorStates::STOP;
  //   redLed.ledStatus = redLed.ledStates::OFF;
  //   blueLed.ledStatus = blueLed.ledStates::ON;
  // }

  //---------------- Analog stick value events ---------------
  if(Ps3.data.analog.stick.lx > 5 ) //move Right, upper limit = 127
  {
    motor2.deviation = static_cast<int8_t>(motor2.speed / 127.0 * Ps3.data.analog.stick.lx);
    motor2.speed = motor2.speed - motor2.deviation;
  }

  if(Ps3.data.analog.stick.lx < -5 ) //move left, upper limit = -128
  {
    motor1.deviation = static_cast<int8_t>(-(motor1.speed) / 128.0 * Ps3.data.analog.stick.lx);
    motor1.speed = motor1.speed - motor1.deviation;
  }

      /* Analog D pad button events */

//   if( Ps3.data.analog.button.right > 5 )
//   {
//     motor1.speed = 127;
//     motor1.motorStatus = motor1.motorStates::FRONT;
//     motor2.motorStatus = motor2.motorStates::BACK;
//     redLed.ledStatus = redLed.ledStates::OFF;
//     blueLed.ledStatus = blueLed.ledStates::ON;
//   }
//   if( Ps3.event.button_up.right )
//   {
//     motor1.motorStatus = motor1.motorStates::STOP;
//     motor2.motorStatus = motor2.motorStates::STOP;
//     redLed.ledStatus = redLed.ledStates::ON;
//     blueLed.ledStatus = blueLed.ledStates::OFF;
//   }       

//   if( Ps3.data.analog.button.left > 5 )
//   {
//     motor1.speed = 127;
//     motor2.motorStatus = motor2.motorStates::FRONT;
//     motor1.motorStatus = motor1.motorStates::BACK;
//     redLed.ledStatus = redLed.ledStates::OFF;
//     blueLed.ledStatus = blueLed.ledStates::ON;
//   }

//   if( Ps3.event.button_up.left )
//   {
//     motor1.motorStatus = motor1.motorStates::STOP;
//     motor2.motorStatus = motor2.motorStates::STOP;
//     redLed.ledStatus = redLed.ledStates::ON;
//     blueLed.ledStatus = blueLed.ledStates::OFF;
//   } 
}

void driveMode::disconnectedBlink()
{
  static unsigned long currentTime = millis();
  static uint8_t currentLed = 0;

  // Check if it's time to blink the next led
  if (millis() - currentTime > 150) 
  {
    // Increment the currentLed value
    Ps3.setPlayer(currentLed++);

    // Update the previous time
    currentTime = millis();

    // Reset currentLed if it exceeds the total number of Blink
    if (currentLed > 4)
      currentLed = 0;
  }
}