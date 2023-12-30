#include "GLOBALS.h"
#include "CONFIG.h"

void notify()
{
  /*---------------- Setting the speed ----------------
  /*---- Analog cross/square/triangle/circle button events ----*/

  if( abs(Ps3.event.analog_changed.button.triangle) > 100)
  {
    motor1.speed = 255;
    motor2.speed = 255;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.circle) > 100)
  {
    motor1.speed = 190;
    motor2.speed = 190;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.cross) > 100)
  {
    motor1.speed = 100;
    motor2.speed = 100;
    buzz.alarm();
  }

  if( abs(Ps3.event.analog_changed.button.square) > 100)
  {
    motor1.speed = 120;
    motor2.speed = 120;
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

  /*---------- Digital select/start/ps button events ---------*/
  if( Ps3.event.button_down.select )
  {
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
    {
      motor1.FLIPStatus = motor1.motorStates::FLIP;
      // //buzz.nonBlockToneOn();
      debugln("FLIPPED");
    }
    else
    {
      motor1.FLIPStatus = motor1.motorStates::UNFLIP;
      // //buzz.nonBlockToneOn();
      debugln("UNFLIPPED");
    }
    
    delay(50);
  }

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Motor State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  switch(motor1.motorStatus)
  {
    case motor1.motorStates::FRONT:
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
      motor1.front();
    else
      motor2.front();
    break;

    case motor1.motorStates::BACK:
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
      motor1.back();
    else
      motor2.back();
    break;

    case motor1.motorStates::ENABLE:
    motor1.enable();
    buzz.initBuzzer();
    break;

    case motor1.motorStates::DISABLE:
    motor1.disable();
    buzz.deinitBuzzer();
    break;

    case motor1.motorStates::STOP:
    motor1.stop();
    break;
  }

  switch(motor2.motorStatus)
  {
    case motor2.motorStates::FRONT:
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
      motor2.front();
    else
      motor1.front();
    break;

    case motor2.motorStates::BACK:
    if(motor1.FLIPStatus == motor1.motorStates::UNFLIP)
      motor2.back();
    else
      motor1.back();
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

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ REDLED State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  switch(redLed.ledStatus)
  {
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

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BLUELED State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  switch(blueLed.ledStatus)
  {
    case blueLed.ledStates::ON:
    blueLed.on();
    break;

    case blueLed.ledStates::OFF:
    blueLed.off();
    break;

    case blueLed.ledStates::TOGGLE:
    blueLed.toggle();
    break;

    case blueLed.ledStates::BLINKTWICE:
    blueLed.blinkTwice();
    break;
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

  motor1.begin();
  motor1.enable();

  motor2.begin();
  motor2.enable();

  redLed.begin();

  buzz.begin();

  blueLed.begin();
  redLed.begin();

  debugln("Ready");
}

void loop()
{
  if(!Ps3.isConnected())  
    return;
  
  if(initOnce)
  {
    initOnce = false;
    buzz.initBuzzer();
    redLed.blinkTwice();
    blueLed.blinkTwice();
    // player = 2;
    debug("Setting LEDs to player "); 
    // Serial.println(player, DEC);
    Ps3.setPlayer(player);
  }
}
