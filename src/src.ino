#include "GLOBALS.h"
#include "CONFIG.h"

void notify()
{
  //---------- Digital select/start/ps button events ---------
  if( Ps3.event.button_down.start )
  {
    if(++stateCount == 3)
      stateCount = 1;

    if(stateCount == 1)
    {
      Ps3.setPlayer(1);
      driveStatus = driveStates::mode1;
    }
    else if(stateCount == 2)
    {
      Ps3.setPlayer(2);
      driveStatus = driveStates::mode2;
    }
  }

  switch(driveStatus)
  {
    case driveStates::mode1:
    driveMode1();
    break;

    case driveStates::mode2:
    driveMode2();
    break;

    case driveStates::none:
    Ps3.setPlayer(15);
    break;
  }

  setSpeed();

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Motor State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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

  begin();

  debugln("Ready");
}

void loop()
{
  if(!Ps3.isConnected())  
    return;
}
