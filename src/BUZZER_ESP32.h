/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/
#include "CONFIG.h"

#pragma once

class buzzer {
private:
    uint8_t buzzId;
    uint8_t buzzPin;
    uint8_t resolution;
    // uint8_t PWMChannel;
    bool debugStatus;
    uint32_t frequency;
    uint8_t ledTimer;

public:
    inline buzzer(const uint8_t& = -1, const uint8_t& = -1, const uint8_t& = 8, const uint32_t& = 5000, const bool& = false) __attribute__((always_inline));
    inline void begin() __attribute__((always_inline));
    inline ~buzzer() __attribute__((always_inline));
    inline void initBuzzer() __attribute__((always_inline));
    inline void deinitBuzzer() __attribute__((always_inline));
    inline void alarm() __attribute__((always_inline));
    inline void nonBlockToneOn() __attribute__((always_inline));
    inline void nonBlockToneInit() __attribute__((always_inline));
};

buzzer::buzzer(const uint8_t& buzzId, const uint8_t& buzzPin, const uint8_t& resolution, const uint32_t& frequency, const bool& debugStatus) :
buzzId(buzzId),
buzzPin(buzzPin),
resolution(resolution),
frequency(frequency),
debugStatus(debugStatus)
{
  ledTimer = 0;
}

void buzzer::begin() 
{
  // ledcSetup(this->PWMChannel, this->frequency, this->resolution);  // Set LEDC channel 0 with a frequency of 5000 Hz and resolution of 8 bits
  ledcAttach(this->buzzPin, this->frequency, this->resolution);
  deinitBuzzer();

  if(debugStatus) debugln(String(this->buzzId)+" object initilized");
}

void buzzer::initBuzzer()
{
  //InitBuzzer is for active buzzer
  ledcWriteNote(this->buzzPin, NOTE_E, 4);
  delay(200);
  ledcWriteNote(this->buzzPin, NOTE_C, 5);
  delay(200);
  ledcWriteNote(this->buzzPin, NOTE_G, 4);
  delay(200);
  ledcWriteTone(this->buzzPin, 0);

  if(debugStatus) debugln(String(this->buzzId)+": Init Buzzer");
}

void buzzer::deinitBuzzer()
{
  //InitBuzzer is for active buzzer
  ledcWriteNote(this->buzzPin, NOTE_G, 3);
  delay(200);
  ledcWriteNote(this->buzzPin, NOTE_E, 3);
  delay(200);
  ledcWriteNote(this->buzzPin, NOTE_C, 3);
  delay(200);
  ledcWriteTone(this->buzzPin, 0);

  if(debugStatus) debugln(String(this->buzzId)+": deInit Buzzer");
}

void buzzer::alarm()
{
  // ledcWriteTone(this->buzzPin, 1000);
  ledcWriteNote(this->buzzPin, NOTE_G, 4);
  delay(20);
  ledcWriteTone(this->buzzPin, 0);

  if(debugStatus) debugln(String(this->buzzId)+": Alarm");
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

//Destructor
buzzer::~buzzer()
{
  ledcDetach(this->buzzPin);
  debugln("buzzer object destroyed"); 
}