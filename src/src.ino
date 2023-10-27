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
  clenchPwm = motor1.pwm = motor2.pwm = int((ibus.readChannel(2)-1000)*0.255);            // Speed (Acceleration)

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
    motorStatus = motorStates::FRONT;
  }
  else if(CH2 < 1200)
  {
    motorStatus = motorStates::BACK;
  }
  else if(CH3 > 1800)
  {
    motorStatus = motorStates::RIGHT;
  }
  else if(CH3 < 1200)
  {
    motorStatus = motorStates::LEFT;
  }
  else 
  {
    motorStatus = motorStates::STOP;
  }

  //L298N gripper

  if(CH4 > 1800)
  {
    L298NStatus = L298NStates::GRIPPEROPEN;
  }
  else if(CH4 < 1200)
  {
    L298NStatus = L298NStates::GRIPPERCLOSE;
  }
  else if(CH5 > 1800)
  {
    L298NStatus = L298NStates::GRIPPERUP;
  }
  else if(CH5 < 1200)
  {
    L298NStatus = L298NStates::GRIPPERDOWN;
  }
  else 
  {
    L298NStatus = L298NStates::STOP;
  }

  
/*================================================ ROBOT STATEMACHINE ==============================================*/  
  switch (motorStatus)
  {

    case motorStates::STOP:
    debug("Stop: ");
    motor1.stop();
    debug(" : ");
    motor2.stop();
    blueLed.off();
    redLed.on();
    break;

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

    default: debugln("Invalid input");
  }
  debugln();

  switch(L298NStatus)
  {
    case L298NStates::GRIPPEROPEN:
    // gripper.front();
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    // debugln("OPEN");
    break;

    case L298NStates::GRIPPERCLOSE:
    // gripper.back();
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    // debugln("CLOSE");
    break;

    case L298NStates::GRIPPERUP:
    // clench.front();
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    // debugln("UP");
    break;

    case L298NStates::GRIPPERDOWN:
    // clench.back();
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    // debugln("DOWN");
    break;

    case L298NStates::STOP:
    // gripper.stop();
    // clench.stop();
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    // debugln("STOP");
    break;

    default: debugln("Invalid input");
  }

  analogWrite(ENB, clenchPwm);
}
