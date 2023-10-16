#include "GLOBALS.h"

void setup()
{
  Serial.begin(9600);
  initSystem();
}

void loop()
{     
  
/*==========================================================ROBOT STATE MACHINE========================================================*/  
  switch (motorStatus)
  {
    case motorStates::FRONT:
    debug("Front: ");
    motor1.front(); 
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::BACK:
    debug("Back: ");
    motor1.back();  
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::LEFT:
    debug("Left: ");
    motor1.back();
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::RIGHT:
    debug("Right: ");
    motor1.front();
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
         
    case motorStates::SHARPRIGHTFRONT:
    debug("Sharp Right Front: ");
    motor1.front();
    debug(" : ");
    motor2.stop();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::SHARPLEFTFRONT:
    debug("Sharp Left Front: ");
    motor1.stop();
    debug(" : ");
    motor2.front();
    blueLed.on();
    redLed.off(); 
    break;
   
    case motorStates::SHARPRIGHTBACK:
    debug("Sharp Right Back: ");
    motor1.stop();
    debug(" : ");
    motor2.back();
    blueLed.on();
    redLed.off();
    break;
    
    case motorStates::SHARPLEFTBACK:
    debug("Sharp Left Back: ");
    motor1.back();
    debug(" : ");
    motor2.stop();
    blueLed.on();
    redLed.off();
    break;

    case motorStates::STOP:
    debug("Stop: ");
    motor1.stop();
    debug(" : ");
    motor2.stop();
    blueLed.off();
    redLed.on();
    break;
    
    case motorStates::SPEED0:
    motor1.pwm = motor2.pwm = 0;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.on();
    break;
    
    case motorStates::SPEED1:
    motor1.pwm = motor2.pwm = 25;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED2:
    motor1.pwm = motor2.pwm = 51;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED3:
    motor1.pwm = motor2.pwm = 76;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED4:
    motor1.pwm = motor2.pwm = 102;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED5:
    motor1.pwm = motor2.pwm = 127;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED6:
    motor1.pwm = motor2.pwm = 153;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED7:
    motor1.pwm = motor2.pwm = 178;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED8:
    motor1.pwm = motor2.pwm = 204;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::SPEED9:
    motor1.pwm = motor2.pwm = 229;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;
    
    case motorStates::MAXSPEED:
    motor1.pwm = motor2.pwm = 255;
    debug("Speed: "); debug(motor1.pwm);  debug(" : "); debugln(motor2.pwm);
    buzz.off();
    break;

    case motorStates::STOPALL:
    standbySystem();
    while(true)
    {
      if(Serial.available())
      {
        initSystem();
        break;
      }
      else
        redLed.toggle();
    }
    break;

    case motorStates::EXTRAON:                            //Same as EXTRAOFF
    standbySystem();
    while(true)
    {
      motorStatus = (motorStates)Serial.read();
      if(motorStates::EXTRAOFF == motorStatus)
      {
        initSystem();
        break;
      }
      else if(motorStates::STOPALL == motorStatus)
      {
        break;
      }
      else
        redLed.toggle();
    }
    break;
    
    case motorStates::EXTRAOFF:                           //Same as EXTRAON
    standbySystem();
    while(true)
    {
      motorStatus = (motorStates)Serial.read();
      if(motorStates::EXTRAOFF == motorStatus)
      {
        initSystem();
        break;
      }
      else if(motorStates::STOPALL == motorStatus)
      {
        break;
      }
      else
        redLed.toggle();
    }
    break;

    case motorStates::FRONTLIGHTSON:
    debugln("Front lights on");
    //Do nothing for now
    break;
    case motorStates::FRONTLIGHTSOFF:
    debugln("Front lights off");
    //Do nothing for now
    break;
    case motorStates::BACKLIGHTSON:
    debugln("Back lights on");
    //Do nothing for now
    break;
    case motorStates::BACKLIGHTSOFF:
    debugln("Back lights off");
    //Do nothing for now
    break;
    case motorStates::HORNON:
    debugln("Horn on");
    //Do nothing for now
    break;
    case motorStates::HORNOFF:
    debugln("Horn off");
    //Do nothing for now
    break;

    default: debugln("Invalid input");
  }

  debugln();
  
  if (Serial.available() > 0) 
    motorStatus = (motorStates)Serial.read();        //Read the state
}
