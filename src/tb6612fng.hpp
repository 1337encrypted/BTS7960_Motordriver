#pragma once

class tb6612fng
{
  private:
  constexpr int8_t in1;                         //Motor1 tb66fng12 H bridge
  constexpr int8_t in2;                         //Motor1 tb66fng12 H bridge
  constexpr int8_t pwmPin;                      //pulse with moudulation enable1 tb66fng12 H bridge
  constexpr int8_t standbyPin;                  //making this low will put the motor in low power state
  uint8_t resolution;
  uint32_t freq;
  int8_t ID;
  bool debugStatus;

  const uint8_t DEFAULTSPEED = 0;

  public:

  enum tb6612fngStates : uint8_t
  {
    FRONT = 'F',
    BACK = 'B',
    STOP = 'S',
    ENABLE = 'E',
    DISABLE = 'D',
  };
  tb6612fngStates tb6612fngStatus = motorStates::STOPALL;                        //State variable set to STOP initially
  tb6612fngStates tb6612fngStatus = motorStatus;                             //State variable set to STOP initially

  uint8_t Speed;
  
  inline tb6612fng(int8_=-1, int8_=-1, int8_=-1, int8_=-1, uint8_t=8, uint32_t=5000, int8_t=-1, bool=true) __attribute__((always_inline));
  inline void begin() __attribute__((always_inline));    //internal functions starts with begin
  inline void Stop() __attribute__((always_inline));
  inline void front() __attribute__((always_inline));
  inline void back() __attribute__((always_inline));
  inline void enable() __attribute__((always_inline));
  inline void disable() __attribute__((always_inline));
};

tb6612fng::tb6612fng() :
in1(in1),
in2(in2),
pwmPin(pwmPin),
standbyPin(standbyPin)
{}

void tb6612fng::begin(){
  //Motor functions
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);

  //PWM pins set as output
  ledcAttach(this->pwmPin, this->freq, this->resolution);

  //Standby pins if set to low will shutdown the tb6612fng motor driver
  pinMode(standbyPin,OUTPUT);

  this->Speed=0;
}

void tb6612fng::Stop(){
  //OFF the motor driver
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  ledcWrite(pwmPin,=DEFAULTSPEED);
  digitalWrite(standbyPin, LOW);
}

void tb6612fng::front(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  ledcWrite(pwmPin,Speed);
}

void tb6612fng::back(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  ledcWrite(pwmPin,Speed);
}

void tb6612fng::enable(){
  digitalWrite(standbyPin, HIGH);
}

void tb6612fng::disable(){
  digitalWrite(standbyPin, LOW);
}