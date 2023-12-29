/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/
#ifndef led_h
#define led_h

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class led
{
  private:
  //ledPin pins for function feedback
  uint8_t ledPin;
  bool debugStatus;
  String ledId;

  public:

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++MOTOR STATES++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  enum ledStates : uint8_t
  {
    ON,
    OFF,
    TOGGLE,
    BLINKTWICE
  };
  ledStates ledStatus = ledStates::OFF;

  //Function prototypes
  inline led(const uint8_t=-1, const String="", bool=false) __attribute__((always_inline));
  inline ~led() __attribute__((always_inline));
  inline void begin() __attribute__((always_inline));
  inline void on() __attribute__((always_inline));
  inline void off() __attribute__((always_inline));
  inline void toggle() __attribute__((always_inline));
  inline void blinkTwice() __attribute__((always_inline));
  inline void printInfo() __attribute__((always_inline));
};


void led::begin()
{
  if(this->ledPin > 0)  pinMode(ledPin, OUTPUT);
}

//parametrized constructor
led::led(const uint8_t ledPin, const String ledId, bool debugStatus)
{
  //Initilize the ledPin pins
  this->ledPin = ledPin;
  this->ledId = ledId;
  this->debugStatus = debugStatus;
}

//Destructor
led::~led()
{
  if(this->debugStatus) Serial.println(this->ledId+" object destroyed");
}

void led::on()
{
  digitalWrite(ledPin, HIGH);
  if(this->debugStatus) Serial.println(this->ledId+" ON");
}

void led::off()
{
  digitalWrite(ledPin, LOW);
  if(this->debugStatus) Serial.println(this->ledId+" OFF");
}

void led::toggle()
{
  static unsigned long ledMillis = millis(); /*Assigns the current snapshot of time only the first time this code executes*/
  if(millis() - ledMillis > 700)
  {
    digitalWrite(this->ledPin, !digitalRead(this->ledPin));
    ledMillis = millis();  
  }
}

void led:: blinkTwice()
{
  on();
  delay(50);
  off();
  delay(50);
  on();
  delay(50);
  off();
  delay(50);
}

void led::printInfo()
{
	if(this->debugStatus) Serial.println(this->ledId+" object initilized");
	delay(1000);
  return;
}

#endif  //END led_h
