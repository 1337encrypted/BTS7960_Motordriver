/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/

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
  bool debugStatus;
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
  inline buzzer(uint8_t=-1, uint8_t=-1, uint8_t=-1, uint16_t=5000, uint8_t=-1, bool=false) __attribute__((always_inline));
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
buzzer::buzzer(uint8_t buzzPin, uint8_t PWMChannel, uint8_t resolution, uint16_t freq, uint8_t buzzId, bool debugStatus)
{
  //Initilize the buzzer
  this->buzzPin = buzzPin;
  this->PWMChannel = PWMChannel;
  this->resolution = resolution;
  this->freq = freq;
  this->buzzId = buzzId;
  this->debugStatus = debugStatus;

  //Begin happens after object construction
  begin();
}

void buzzer::begin()
{
  //buzzer pin as output
  if(this->buzzPin > 0) pinMode(buzzPin, OUTPUT);

  ledcSetup(this->PWMChannel, this->freq, this->resolution); // 5 kHz frequency, 8-bit resolution
  ledcAttachPin(this->buzzPin, this->PWMChannel);
  ledcWrite(this->PWMChannel, 0);

  if(this->debugStatus) Serial.println(this->buzzId+" object initilized");
}

//Destructor
buzzer::~buzzer()
{
  ledcDetachPin(this->buzzPin);
  if(this->debugStatus) Serial.println("buzzer object destroyed"); 
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
  delay(100);
  noTone();
}

void buzzer::off()
{
  noTone();
}

void buzzer::on()
{
  static unsigned long buzzMillis = millis(); /*Assigns the current snapshot of time only the first time this code executes*/
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
    delay(duration);
  noTone();
}

void buzzer::noTone() 
{
  ledcWrite(this->PWMChannel, 0);
}

#endif  //END BUZZER_H


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BUZZER State machine ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
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

  */
