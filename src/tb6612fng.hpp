#pragma once
#include "CONFIG.hpp"

class tb6612fng
{
  private:
  int8_t in1;                         //Motor1 tb66fng12 H bridge
  int8_t in2;                         //Motor1 tb66fng12 H bridge
  int8_t in3;                         //Motor2 tb66fng12 H bridge
  int8_t in4;                         //Motor2 tb66fng12 H bridge
  int8_t pwmPin1;                     //pulse with moudulation enable1 tb66fng12 H bridge
  int8_t pwmPin2;                     //pulse with moudulation enable1 tb66fng12 H bridge
  int8_t standbyPin;                  //making this low will put the motor in low power state
  uint8_t resolution;
  uint32_t frequency;
  uint8_t id;
  bool debugStatus;

  const uint8_t DEFAULTSPEED = 0;

  public:

  enum tb6612fngStates : uint8_t
  {
    GRIPPEROPEN = 'O',
    GRIPPERCLOSE = 'C',
    GRIPPERUP = 'U',
    GRIPPERDOWN = 'D',
    ENABLE = 'E',
    DISABLE = 'X',
    STOP = 'S'
  };
  tb6612fngStates tb6612fngStatus = tb6612fngStates::STOP;                        //State variable set to stop initially

  uint8_t speed;
  
  inline tb6612fng(int8_t=-1, int8_t=-1, int8_t=-1, int8_t=-1, int8_t=-1, int8_t=-1, int8_t=-1, uint8_t=8, uint32_t=5000, int8_t=-1, bool=true) __attribute__((always_inline));
  inline void begin() __attribute__((always_inline));    //internal functions starts with begin
  inline void stop() __attribute__((always_inline));
  inline void gripperOpen() __attribute__((always_inline));
  inline void gripperClose() __attribute__((always_inline));
  inline void gripperUp() __attribute__((always_inline));
  inline void gripperDown() __attribute__((always_inline));
  inline void enable() __attribute__((always_inline));
  inline void disable() __attribute__((always_inline));
  inline void loop() __attribute__((always_inline));
};

tb6612fng::tb6612fng(int8_t in1, int8_t in2, int8_t in3, int8_t in4, int8_t pwmPin1, int8_t pwmPin2, int8_t standbyPin, uint8_t resolution, uint32_t frequency, int8_t id, bool debugStatus) :
in1(in1),
in2(in2),
in3(in3),
in4(in4),
pwmPin1(pwmPin1),
pwmPin2(pwmPin2),
standbyPin(standbyPin),
resolution(resolution),
frequency(frequency),
id(id),
debugStatus(debugStatus)
{
  speed=0;
}

void tb6612fng::begin(){
  //Motor1 functions
  if(in1 > 0) pinMode(in1,OUTPUT);
  if(in2 > 0) pinMode(in2,OUTPUT);

  //Motor2 functions
  if(in3 > 0) pinMode(in3,OUTPUT);
  if(in4 > 0) pinMode(in4,OUTPUT);

  //PWM pins set as output
  if(pwmPin1 > 0) ledcAttach(this->pwmPin1, this->frequency, this->resolution);
  if(pwmPin2 > 0) ledcAttach(this->pwmPin2, this->frequency, this->resolution);

  //Standby pins if set to low will shutdown the tb6612fng motor driver
  if(standbyPin > 0)  pinMode(standbyPin,OUTPUT);

  this->speed=0;
}

void tb6612fng::stop(){
  //OFF the motor driver
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  ledcWrite(pwmPin1, DEFAULTSPEED);
  ledcWrite(pwmPin2, DEFAULTSPEED);
  digitalWrite(standbyPin, LOW);

  if(debugStatus) Serial.println((String)id+" stop: "+this->speed);
}

void tb6612fng::gripperOpen(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  ledcWrite(pwmPin1,speed);

  if(debugStatus) Serial.println((String)id+" OPEN: "+this->speed);
}

void tb6612fng::gripperClose(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  ledcWrite(pwmPin1,speed);

  if(debugStatus) Serial.println((String)id+" CLOSE: "+this->speed);
}

void tb6612fng::gripperUp(){
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  ledcWrite(pwmPin2,speed);

  if(debugStatus) Serial.println((String)id+" UP: "+this->speed);
}

void tb6612fng::gripperDown(){
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  ledcWrite(pwmPin2,speed);

  if(debugStatus) Serial.println((String)id+" DOWN: "+this->speed);
}

void tb6612fng::enable(){
  digitalWrite(standbyPin, HIGH);

  if(this->debugStatus) Serial.println((String)this->id+" motor driver enabled");
}

void tb6612fng::disable(){
  digitalWrite(standbyPin, LOW);

  if(this->debugStatus) Serial.println((String)this->id+" motor driver disabled");
}

void tb6612fng::loop(){
  switch(tb6612fngStatus){
    case tb6612fngStates::GRIPPEROPEN:
    gripperOpen();
    break;

    case tb6612fngStates::GRIPPERCLOSE:
    gripperClose();
    break;

    case tb6612fngStates::GRIPPERUP:
    gripperUp();
    break;

    case tb6612fngStates::GRIPPERDOWN:
    gripperDown();
    break;


    case tb6612fngStates::ENABLE:
    enable();
    break;

    case tb6612fngStates::DISABLE:
    disable();
    break;

    case tb6612fngStates::STOP:
    stop();
    break;
  }
}