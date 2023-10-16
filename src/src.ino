#include "GLOBALS.h"
#include "CONFIG.h"

void notify()
{

  buzz.buzzStatus = buzz.buzzStates::OFF;

  /*---------------- Setting the speed ----------------
  /*---- Analog cross/square/triangle/circle button events ----*/

  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = motor2.speed = 255;
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = motor2.speed = 205;
  }

  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = motor2.speed = 155;
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = motor2.speed = 180;
  }

  /* ============================================= Motor 1 events ============================================= */

  if( Ps3.event.button_down.l1 )        /*------------- Digital left shoulder button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }

  if( Ps3.event.button_down.l2 )        /*-------------- Digital left trigger button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }
  if( Ps3.event.button_up.l1 || Ps3.event.button_up.l2)   /*-------------- Digital left trigger button event -------------*/
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::OFF;
  }

  /* ============================================= Motor 2 events =============================================*/

  if( Ps3.event.button_down.r1 )      /*------------- Digital right shoulder button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::ON;
  }
  if( Ps3.event.button_down.r2 )      /*-------------- Digital right trigger button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::ON;
  }
  if( Ps3.event.button_up.r1 || Ps3.event.button_up.r2)  /*-------------- Digital right trigger button event -------------*/
  {
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::OFF;
  }


  /*---------- Digital select/start/ps button events ---------*/
  if( Ps3.event.button_down.select )
  {
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
    {
      motor1.FLIPStatus = motor1.motorStates::FLIP;
    }
    else
    {
      motor1.FLIPStatus = motor1.motorStates::UNFLIP;
    }
    
    delay(50);
  }

  if( Ps3.event.button_down.start )
  {
    if(motor1.ENABLEStatus == motor1.motorStates::DISABLE)
    {
      motor1.motorStatus = motor1.motorStates::ENABLE;
      motor2.motorStatus = motor2.motorStates::ENABLE;
      
      motor1.ENABLEStatus = motor1.motorStates::ENABLE;
      buzz.buzzStatus = buzz.buzzStates::INITBUZZER;
      redLed.ledStatus = redLed.ledStates::BLINKTWICE;
    }
    else
    {
      motor1.motorStatus = motor1.motorStates::DISABLE;
      motor2.motorStatus = motor2.motorStates::DISABLE;

      motor1.ENABLEStatus = motor1.motorStates::DISABLE;
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
      debug("The controller battery is ");
      if( battery == ps3_status_battery_charging )      debugln("charging");
      else if( battery == ps3_status_battery_full )     debugln("FULL");
      else if( battery == ps3_status_battery_high )     debugln("HIGH");
      else if( battery == ps3_status_battery_low)       debugln("LOW");
      else if( battery == ps3_status_battery_dying )    debugln("DYING");
      else if( battery == ps3_status_battery_shutdown ) debugln("SHUTDOWN");
      else debugln("UNDEFINED");
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
