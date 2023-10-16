/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/

#include "CONFIG.h"
#ifndef BUZZER_h
#define BUZZER_h

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class buzzer
{
  private: 
  uint8_t buzzPin, PWMChannel, resolution, buzzId;
  uint32_t freq;

  public:

  enum buzzStates : uint8_t
  {
    INITBUZZER,
    DEINITBUZZER,
    ALARM,
    ON,
    OFF,
    TONE
  };
  buzzStates buzzStatus = buzzStates::OFF;

  //Function prototype
  inline void begin() __attribute__((always_inline));
  inline buzzer(uint8_t=-1, uint8_t=-1, uint8_t=-1, uint32_t=5000, uint8_t=-1) __attribute__((always_inline));
  inline ~buzzer() __attribute__((always_inline));
  inline void initBuzzer() __attribute__((always_inline));
  inline void deinitBuzzer() __attribute__((always_inline));
  inline void alarm() __attribute__((always_inline));
  inline void on() __attribute__((always_inline));
  inline void off() __attribute__((always_inline));
  inline void tone(uint16_t, uint32_t = 0) __attribute__((always_inline));
  inline void noTone() __attribute__((always_inline));
	
};



//Parametrized constructor
buzzer::buzzer(uint8_t buzzPin, uint8_t PWMChannel, uint8_t resolution, uint32_t freq, uint8_t buzzId)
{
  //Initilize the buzzer
  this->buzzPin = buzzPin;
  this->PWMChannel = PWMChannel;
  this->resolution = resolution;
  this->freq = freq;
  this->buzzId = buzzId;

  //Begin and enable happens after object construction
  begin();
}

void buzzer::begin()
{
  //buzzer pin as output
  if(buzzPin > 0) pinMode(buzzPin, OUTPUT);

  ledcSetup(this->PWMChannel, this->freq, this->resolution); // 5 kHz frequency, 8-bit resolution
  ledcAttachPin(this->buzzPin, this->PWMChannel);
  ledcWrite(this->PWMChannel, 0);

  debugln(this->buzzId+" object initilized");
}

//Destructor
buzzer::~buzzer()
{
  debugln("buzzer object destroyed"); 
}

void buzzer::initBuzzer()
{
  //InitBuzzer is for active buzzer
  tone(2000, 100);
  delay(100);
  tone(1000, 100);
  delay(200);
  noTone();
}

void buzzer::deinitBuzzer()
{
  //InitBuzzer is for active buzzer
  tone(2000, 100);
  delay(150);
  tone(1000, 100);
  delay(150);
  tone(500, 100);
  delay(150);  
  noTone();
}

void buzzer::alarm()
{
  tone(1000, 100);
}

void buzzer::off()
{
  noTone();
}

void buzzer::on()
{
  static unsigned long buzzMillis = millis();                   //Assigns the current snapshot of time only the first
                                                                //time this code executes
  if(millis() - buzzMillis > 10)
  {
    tone(1000, 100);
    buzzMillis = millis();  
  }
}

void buzzer::tone(uint16_t freq, uint32_t duration) 
{
  ledcWriteTone(this->PWMChannel, freq);
  if (duration > 0) 
  {
    delay(duration);
    noTone();
  }
}

void buzzer::noTone() 
{
  ledcWrite(this->PWMChannel, 0);
}

#endif  //END BUZZER_H