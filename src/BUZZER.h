/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/
#include "CONFIG.h"

#ifndef BUZZER_h
#define BUZZER_h

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class buzzer {
private:
    uint8_t buzzId;
    uint8_t buzzPin;
    uint8_t resolution;
    uint8_t PWMChannel;
    bool debugStatus;
    uint32_t frequency;

public:
    inline buzzer(uint8_t = -1, uint8_t = -1, uint8_t = -1, uint8_t = 8, uint32_t = 5000, bool = false); __attribute__((always_inline));
    inline void begin() __attribute__((always_inline));
    inline ~buzzer() __attribute__((always_inline));
    inline void initBuzzer() __attribute__((always_inline));
    inline void deinitBuzzer() __attribute__((always_inline));
    inline void alarm() __attribute__((always_inline));
    inline void nonBlockToneOn() __attribute__((always_inline));
    inline void nonBlockToneInit() __attribute__((always_inline));
    inline void printInfo() __attribute__((always_inline));
};

buzzer::buzzer(uint8_t buzzId, uint8_t buzzPin, uint8_t PWMChannel, uint8_t resolution, uint32_t frequency, bool debugStatus) 
{
  this->buzzId = buzzId;
  this->buzzPin = buzzPin;
  this->PWMChannel = PWMChannel;
  this->resolution = resolution;
  this->frequency = frequency;
  this->debugStatus = debugStatus;
}

void buzzer::begin() 
{
  pinMode(this->buzzPin, OUTPUT);
  ledcSetup(this->PWMChannel, this->frequency, this->resolution);  // Set LEDC channel 0 with a frequency of 5000 Hz and resolution of 8 bits
  ledcAttachPin(this->buzzPin, this->PWMChannel);
  initBuzzer();
}

void buzzer::initBuzzer()
{
  //InitBuzzer is for active buzzer
  ledcWriteTone(this->buzzPin, 2000);
  delay(200);
  ledcWriteTone(this->buzzPin, 1000);
  delay(300);
  ledcWriteTone(this->buzzPin, 0);
}

void buzzer::deinitBuzzer()
{
  ledcWriteTone(this->buzzPin, 2000);
  delay(250);
  ledcWriteTone(this->buzzPin, 1000);
  delay(250);
  ledcWriteTone(this->buzzPin, 500);
  delay(250);  
  ledcWriteTone(this->buzzPin, 0);
}

void buzzer::alarm()
{
  ledcWriteTone(this->buzzPin, 1000);
  delay(10);
  ledcWriteTone(this->buzzPin,0);
}

void buzzer::nonBlockToneOn()
{
  static unsigned long buzzMillis = millis();                   //Assigns the current snapshot of time only the first
  if(millis() - buzzMillis > 1000)                               //time this code executes
  {
    ledcWriteTone(this->buzzPin, 1000);
    delay(100);
    buzzMillis = millis();  
    ledcWriteTone(this->buzzPin, 0);
  }
}

void buzzer::nonBlockToneInit()
{
  static unsigned long currentTime = millis();
  static uint8_t currentTone = 1;

  // Check if it's time to play the next tone
  if (millis() - currentTime > 1000) {

    // Play the appropriate tone based on the currentTone value
    switch (currentTone) 
    {
      case 1:
        ledcWriteTone(this->buzzPin, 2000);
        break;
      case 2:
        ledcWriteTone(this->buzzPin, 1500);
        break;
      case 3:
        ledcWriteTone(this->buzzPin, 1000);
        break;
    }
    delay(150);
    // Update the previous time
    currentTime = millis();
    // Increment the currentTone value
    currentTone++;

    // Reset currentTone if it exceeds the total number of tones
    if (currentTone > 3)
      currentTone = 1;
  }
}

void buzzer::printInfo()
{
  debugln(this->buzzId+" object initilized");
  delay(1000);
}

//Destructor
buzzer::~buzzer()
{
  ledcDetachPin(this->buzzPin);
  // {
  //   debugln("buzzer object destroyed"); 
  // }
}

#endif  //END BUZZER_H