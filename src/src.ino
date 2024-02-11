#include "GLOBALS.hpp"

void setup()
{ 
  Serial.begin(115200);             // Start serial monitor for debugging
  ibus.begin(Serial);               // Attach iBus object to serial port

  motor1.begin();
  motor2.begin();
  gripper.begin();
  buzz.begin();
  redLed.begin();
  blueLed.begin();

  motor1.enable();
  motor2.enable();
  gripper.enable();
}

void loop()
{
  ibus.loop();

  // Flysky Th9x
  //CH0 = ibus.readChannel(0);                                                  // Leftshift - Rightshift
  // motor1.pwm = motor2.pwm = int((ibus.readChannel(1)-1000)*0.255);            // Speed (Acceleration)
  // CH2 = ibus.readChannel(2);                                                  // Forward - Reverse
  // CH3 = ibus.readChannel(3);                                                  // Left - Right 
  // CH4 = ibus.readSwitch(4);                                                   // CH4 Switch mode
  // CH5 is same as CH0
  // CH6 = ibus.readSwitch(6);                                                   // CH6 on off switch

  // Radiomaster FSI6 or Radiomaster TXI6S
  //CH0 = ibus.readChannel(0);                                                // Leftshift - Rightshift
  CH2 = ibus.readChannel(0);                                                  // Forward - Reverse
  CH3 = ibus.readChannel(3);                                                  // Left - Right 
  CH4 = ibus.readChannel(4);                                                  // CH4 Switch mode
  CH5 = ibus.readChannel(5);                                                  // CH6 on off switch (Check it once)
  // clenchPwm = int((CH5-1000)*0.255);                                         // Gripper Speed (Acceleration)
  //gripper.pwm = int((CH4-1000)*0.255);                                        // clench Speed (Acceleration)
  gripper.speed = motor1.speed = motor2.speed = int((ibus.readChannel(2)-1000)*0.255);            // Speed (Acceleration)

  // Serial.print(ibus.readChannel(2));
  // Serial.print(" | ");
  // Serial.print(CH2);
  // Serial.print(" | ");
  // Serial.print(CH3);    //Speed
  // Serial.print(" | ");
  // Serial.print(CH4);
  // Serial.print(" | ");
  // Serial.print(CH5);   // Switch 5
  // debugln(" | ");

  if(CH2 > 1800)
  {
    motor1.motorStatus = motor1.motorStates::FRONT;
    motor2.motorStatus = motor2.motorStates::FRONT;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else if(CH2 < 1200)
  {
    motor1.motorStatus = motor1.motorStates::BACK;
    motor2.motorStatus = motor2.motorStates::BACK;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else 
  {
    motor1.motorStatus = motor1.motorStates::STOP;
    motor2.motorStatus = motor2.motorStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }

  if(CH3 > 1600)
  {
    // motorStatus = motorStates::RIGHT;
    if(motor1.motorStatus == motor1.motorStates::STOP && motor2.motorStatus == motor2.motorStates::STOP)
    {
      motor1.front();
      motor2.back();
    }
    else
    {
      CH3 = CH3-1000;
      motor2.deviation = static_cast<int>(CH3/2-245);
      motor2.speed = motor2.speed - motor2.deviation;
    }
  }

  if(CH3 < 1400)
  {
    // motorStatus = motorStates::LEFT;
    if(motor1.motorStatus == motor1.motorStates::STOP && motor2.motorStatus == motor2.motorStates::STOP)
    {
      motor1.back();
      motor2.front();
    }
    else
    {
      CH3 = CH3-1000;
      motor1.deviation = static_cast<int>(CH3/2-255);
      motor1.speed = motor1.speed + motor1.deviation;
    }
  }


  //tb6612fng gripper

  if(CH4 > 1600)
  {
    gripper.tb6612fngStatus = gripper.tb6612fngStates::GRIPPEROPEN;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else if(CH4 < 1400)
  {
    gripper.tb6612fngStatus = gripper.tb6612fngStates::GRIPPERCLOSE;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else if(CH5 > 1600)
  {
    gripper.tb6612fngStatus = gripper.tb6612fngStates::GRIPPERUP;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else if(CH5 < 1400)
  {
    gripper.tb6612fngStatus = gripper.tb6612fngStates::GRIPPERDOWN;
    redLed.ledStatus = redLed.ledStates::OFF;
    blueLed.ledStatus = blueLed.ledStates::ON;
  }
  else 
  {
    gripper.tb6612fngStatus = gripper.tb6612fngStates::STOP;
    redLed.ledStatus = redLed.ledStates::ON;
    blueLed.ledStatus = blueLed.ledStates::OFF;
  }


  motor1.loop();
  motor2.loop();
  gripper.loop();
  redLed.loop();
  blueLed.loop();
}
