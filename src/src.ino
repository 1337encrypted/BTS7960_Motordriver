#include "GLOBALS.h"

void setup()
{ 
  Serial.begin(115200);             // Start serial monitor for debugging
  ibus.begin(Serial);               // Attach iBus object to serial port
  initSystem();
}

void loop()
{
  ibus.loop();

  // Flysky Th9x
  //CH0 = ibus.readChannel(0);                                                  // Leftshift - Rightshift
  motor1.pwm = motor2.pwm = int((ibus.readChannel(1)-1000)*0.255);            // Speed (Acceleration)
  CH2 = ibus.readChannel(2);                                                  // Forward - Reverse
  //CH3 = ibus.readChannel(3);                                                  // Left - Right 
  //CH4 = ibus.readSwitch(4);                                                   // CH4 Switch mode
  //CH5 is same as CH0
  CH6 = ibus.readSwitch(6);                                                   // CH6 on off switch

//  // Radiomaster TXI6S
//  CH0 = ibus.readChannel(0);                                                // Leftshift - Rightshift
//  motor1.pwm = motor2.pwm = int((ibus.readChannel(2)-1000)*0.255);          // Speed (Acceleration)
//  CH2 = ibus.readChannel(1);                                                // Forward - Reverse
//  CH3 = ibus.readChannel(3);                                                // Left - Right 
//  CH4 = ibus.readSwitch(4);                                                 // CH4 Switch mode
//  CH5 = ibus.readSwitch(5);                                                 // CH6 on off switch (Check it once)

  if(CH2 > 1800)
  {
    motorStatus = motorStates::FRONT;
  }
  else if(CH2 < 1200 && CH2 > 0)
  {
    motorStatus = motorStates::BACK;
  }
  else 
  {
    motorStatus = motorStates::STOP;
  }

  if(CH6 == true)
  {
    motorStatus = motorStates::STOPALL;
  }

  // Check for disconnection

  // currentTime = millis();

  // if(connOnce)
  // {
  //   connOnce = false;
  //   lastReceiveTime = millis();
  // }

  // if (currentTime - lastReceiveTime >= DISCONNECT_TIMEOUT) 
  // if(ibus.readChannel(0) == 0)
  // {
  //   motorStatus = motorStates::DISCONNECT;
  //   // connOnce = true;
  // } 
  // else
  // {
  //   lastReceiveTime = currentTime;
  // }
  
/*==========================================================ROBOT STATE MACHINE=========================================================*/  
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

    case motorStates::STOP:
    debug("Stop: ");
    motor1.stop();
    debug(" : ");
    motor2.stop();
    blueLed.off();
    redLed.on();
    break;

    case motorStates::STOPALL:
    standbySystem();
    while(true)
    {
      ibus.loop();
      CH6 = ibus.readSwitch(6);                                                 // CH6 on off switch
      if(CH6 == false)
      {
        initSystem();
        break;
      }
      else
      {
        redLed.toggle();
        buzz.nonBlockToneOn();
      }
    }
    break;

    // case motorStates::DISCONNECT:
    // standbySystem();
    // while(true)
    // {
    //   ibus.loop();
    //   if(ibus.readChannel(0) > 0)
    //   {
    //     initSystem();
    //     return;
    //   }
    //   else
    //   {
    //     redLed.toggle();
    //     blueLed.toggle();
    //     buzz.nonBlockToneInit();
    //   }
    // }
    // break;

    default: debugln("Invalid input");
  }
  debugln();
}
