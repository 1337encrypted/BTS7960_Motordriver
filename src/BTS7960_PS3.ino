#include "GLOBALS.h"

void notify()
{
  /* Get Y values from the joystick */
  rightY = (Ps3.data.analog.stick.ry);
  leftY = (Ps3.data.analog.stick.ly);

  /* By default the motor status is set to stop */
  motor1.motorStatus = motor1.motorStates::STOP;
  motor2.motorStatus = motor2.motorStates::STOP;
  redLed.ledStatus = redLed.ledStates::OFF;
  buzz.buzzStatus = buzz.buzzStates::OFF;

  /*---------------- Setting the speed ----------------
  //--- Digital cross/square/triangle/circle button events ---*/
  
  if( Ps3.event.button_down.triangle ) //100%
  {
    motor1.speed = motor2.speed = 255;
  }

  if( Ps3.event.button_down.circle )
  {
    motor1.speed = motor2.speed = 205;
  }
      
  if( Ps3.event.button_down.square )
  {
    motor1.speed = motor2.speed = 180;
  }     

  if( Ps3.event.button_down.cross )
  {
    motor1.speed = motor2.speed = 155;
  }

    
  //---------------- Analog stick value events ---------------
  
  if(rightY < DeadBandNegVal)
  {
    motor1.speed = (uint8_t) ceil(rightY*uSpeed);
    motor1.motorStatus = motor1.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }
  else if(rightY > DeadBandPosVal)
  {
    motor1.speed = (uint8_t) ceil(rightY*dSpeed);
    motor1.motorStatus = motor1.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if(leftY < DeadBandNegVal)
  {
    motor2.speed =  (uint8_t) ceil(leftY*uSpeed);
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }
  else if(leftY > DeadBandPosVal)
  {
    motor2.speed = (uint8_t) ceil(leftY*dSpeed);
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  /*-------------- Digital trigger button events -------------*/
  if( Ps3.event.button_down.l2 )
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.r2 )
  {
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  /*------------- Digital shoulder button events -------------*/
  if( Ps3.event.button_down.l1 )
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.r1 )
  {
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  /*--------------- Digital D-pad button events --------------*/

  if( Ps3.event.button_down.up )
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.left )
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.right )
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.down )
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  /*---------- Digital select/start/ps button events ---------*/
    if( Ps3.event.button_down.select )
    {
      if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
      {
        motor1.motorStatus = motor1.motorStates::FLIP;
        motor2.motorStatus = motor2.motorStates::FLIP;
        motor1.FLIPStatus = motor1.motorStatus;
      }
      else
      {
        motor1.motorStatus = motor1.motorStates::UNFLIP;
        motor2.motorStatus = motor2.motorStates::UNFLIP;
        motor1.FLIPStatus = motor1.motorStatus;
      }
      
      delay(50);
    }

    if( Ps3.event.button_down.start )
    {
      if(motor1.ENABLEStatus == motor1.motorStates::DISABLE)
      {
        motor1.motorStatus = motor1.motorStates::ENABLE;
        motor2.motorStatus = motor2.motorStates::ENABLE;
        motor1.ENABLEStatus = motor1.motorStatus;
        buzz.buzzStatus = buzz.buzzStates::INITBUZZER;
        redLed.ledStatus = redLed.ledStates::BLINKTWICE;
      }
      else
      {
        motor1.motorStatus = motor1.motorStates::DISABLE;
        motor2.motorStatus = motor2.motorStates::DISABLE;
        motor1.ENABLEStatus = motor1.motorStatus;
        buzz.buzzStatus = buzz.buzzStates::DEINITBUZZER;
        redLed.ledStatus = redLed.ledStates::TOGGLE;
      }
      
      delay(50);
    }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Motor State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  if(motor1.ENABLEStatus == motor1.motorStates::ENABLE)
  {
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
    {
      switch(motor1.motorStatus)
      {
        case motor1.motorStates::FRONT:
        motor1.front();
        break;

        case motor1.motorStates::BACK:
        motor1.back();
        break;

        case motor1.motorStates::ENABLE:
        motor1.enable();
        break;

        case motor1.motorStates::DISABLE:
        motor1.disable();
        break;

        case motor1.motorStates::STOP:
        motor1.stop();
        break;
      }

      switch(motor2.motorStatus)
      {
        case motor2.motorStates::FRONT:
        motor2.front();
        break;

        case motor2.motorStates::BACK:
        motor2.back();
        break;

        case motor1.motorStates::ENABLE:
        motor2.enable();
        break;

        case motor1.motorStates::DISABLE:
        motor2.disable();
        break;

        case motor2.motorStates::STOP:
        motor2.stop();
        break;
      }
    }
    else if(motor1.FLIPStatus == motor1.motorStates::FLIP)
    {
      switch(motor1.motorStatus)
      {
        case motor1.motorStates::FRONT:
        motor2.front();
        break;

        case motor1.motorStates::BACK:
        motor2.back();
        break;

        case motor1.motorStates::ENABLE:
        motor2.enable();
        break;

        case motor1.motorStates::DISABLE:
        motor2.disable();
        break;

        case motor1.motorStates::STOP:
        motor2.stop();
        break;
      }

      switch(motor2.motorStatus)
      {
        case motor2.motorStates::FRONT:
        motor1.front();
        break;

        case motor2.motorStates::BACK:
        motor1.back();
        break;

        case motor1.motorStates::ENABLE:
        motor1.enable();
        break;

        case motor1.motorStates::DISABLE:
        motor1.disable();
        break;

        case motor2.motorStates::STOP:
        motor1.stop();
        break;
      }
    }
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LED State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  switch(redLed.ledStatus)
  {
    if(motor1.ENABLEStatus == motor1.motorStates::DISABLE)
    {
      redLed.ledStatus = redLed.ledStates::TOGGLE;
    }

    case redLed.ledStates::ON:
    redLed.on();
    break;

    case redLed.ledStates::OFF:
    redLed.off();
    break;

    case redLed.ledStates::TOGGLE:
    redLed.toggle();
    break;

    case redLed.ledStates::BLINKTWICE:
    redLed.blinkTwice();
    break;
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BUZZER State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  switch(buzz.buzzStatus)
  {
    // INITBUZZER
    case buzz.buzzStates::INITBUZZER:
    buzz.initBuzzer();
    break;

    //DEINITBUZZER
    case buzz.buzzStates::DEINITBUZZER:
    buzz.deinitBuzzer();
    break;

    //ALARM
    case buzz.buzzStates::ALARM:
    buzz.alarm();
    break;

    //ON
    case buzz.buzzStates::ON:
    buzz.on();
    break;

    //OFF
    case buzz.buzzStates::OFF:
    buzz.off();
    break;

    //TONE
    // case buzz.buzzStates::TONE:
    // buzz.tone(uint16_t freq);
    // break;
  }

  /* ---------------------- Battery events --------------------- */
  if( battery != Ps3.data.status.battery )
  {
      battery = Ps3.data.status.battery;
      Serial.print("The controller battery is ");
      if( battery == ps3_status_battery_charging )      Serial.println("charging");
      else if( battery == ps3_status_battery_full )     Serial.println("FULL");
      else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
      else if( battery == ps3_status_battery_low)       Serial.println("LOW");
      else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
      else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
      else Serial.println("UNDEFINED");
  }

}

void setup()
{
    Serial.begin(115200);

    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("24:62:ab:dd:a1:d6");
    Ps3.setPlayer(player);

    debugln("Ready.");
}

void loop()
{

  if(!Ps3.isConnected())
    return;

}
