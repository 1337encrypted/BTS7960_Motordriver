/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/
#ifndef buzzer_h
#define buzzer_h

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/*
  NOTE: All the pins can be used with the tone library including the non PWM pins
*/
class buzzer
{
  private: 
  String buzzId;
  uint8_t buzzPin;
  bool debugStatus;

  public:
  //Function prototype
  inline void begin() __attribute__((always_inline));
  inline buzzer(const uint8_t=-1, const uint8_t=-1, bool=false) __attribute__((always_inline));
  inline ~buzzer() __attribute__((always_inline));
  inline void initBuzzer() __attribute__((always_inline));
  inline void deinitBuzzer() __attribute__((always_inline));
  inline void alarm() __attribute__((always_inline));
  inline void nonBlockToneOn() __attribute__((always_inline));
  inline void nonBlockToneInit() __attribute__((always_inline));
  inline void on() __attribute__((always_inline));
  inline void off() __attribute__((always_inline));
  inline void printInfo() __attribute__((always_inline));
  
};

//Parametrized constructor
buzzer::buzzer(const uint8_t buzzPin, const uint8_t buzzId, bool debugStatus)
{
  //Initilize the buzzer
  this->buzzPin = buzzPin;
  this->debugStatus = debugStatus;
  this->buzzId = buzzId;

  //Begin and enable happens after object construction
  begin();
}

//Destructor
buzzer::~buzzer()
{
  Serial.println("buzzer object destroyed"); 
}

void buzzer::begin()
{
  //buzzer pin as output
  pinMode(buzzPin, OUTPUT);
}

void buzzer::initBuzzer()
{
  //InitBuzzer is for active buzzer
  tone(buzzPin, 2000, 100);
  delay(100);
  tone(buzzPin, 1000, 100);
  delay(200);
  noTone();
}

void buzzer::deinitBuzzer()
{
  //InitBuzzer is for active buzzer
  tone(buzzPin, 2000, 100);
  delay(150);
  tone(buzzPin, 1000, 100);
  delay(150);
  tone(buzzPin, 500, 100);
  delay(150);  
  noTone(buzzPin);
}

void buzzer::alarm()
{
  tone(buzzPin, 1000, 100);
}

void buzzer::off()
{
  noTone(buzzPin);
}

void buzzer::nonBlockToneOn()
{
  static unsigned long buzzMillis = millis();                   //Assigns the current snapshot of time only the first
  if(millis() - buzzMillis > 1000)                               //time this code executes
  {
    tone(buzzPin, 1000, 100);
    buzzMillis = millis();  
    off();
  }
}

void buzzer::nonBlockToneInit()
{
  static unsigned long currentTime = millis();
  static uint8_t currentTone = 1;

  // Check if it's time to play the next tone
  if (millis() - currentTime > 1000) {

    // Play the appropriate tone based on the currentTone value
    switch (currentTone) {
      case 1:
        tone(this->buzzPin, 2000, 150);
        break;
      case 2:
        tone(this->buzzPin, 1500, 150);
        break;
      case 3:
        tone(this->buzzPin, 1000, 150);
        break;
    }
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
  Serial.println(this->buzzId+" object initilized");
  delay(1000);
}


#endif  //END BUZZER_H


