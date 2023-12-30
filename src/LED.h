/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file as they are defined*/
#ifndef led_h
#define led_h

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

constexpr uint8_t DUTYCYCLE100=255;
constexpr uint8_t DUTYCYCLE0=0;

class led
{
  private:
  //ledPin pins for function feedback
  uint8_t ledPin, resolution;
  uint32_t frequency;
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
  inline led(const uint8_t=-1, uint32_t=-1000, uint8_t=8, const String="LED", bool=false) __attribute__((always_inline));
  inline ~led() __attribute__((always_inline));
  inline void begin() __attribute__((always_inline));
  inline void on() __attribute__((always_inline));
  inline void off() __attribute__((always_inline));
  inline void toggle() __attribute__((always_inline));
  inline void blinkTwice() __attribute__((always_inline));
  inline void printInfo() __attribute__((always_inline));
};

//parametrized constructor
led::led(const uint8_t ledPin, const uint32_t frequency, const uint8_t resolution, const String ledId, bool debugStatus)
{
  //Initilize the ledPin pins
  this->ledPin = ledPin;
  this->ledId = ledId;
  this->resolution = resolution;
  this->frequency = frequency;
  this->debugStatus = debugStatus;
}

void led::begin()
{
  ledcAttach(this->ledPin, this->frequency, this->resolution);
  if(this->debugStatus) Serial.println(this->ledId+" object initilized");
}

void led::on()
{
  ledcWrite(ledPin, DUTYCYCLE100);
  if(this->debugStatus) Serial.println(this->ledId+": ON");
}

void led::off()
{
  ledcWrite(ledPin, DUTYCYCLE0);
  if(this->debugStatus) Serial.println(this->ledId+": OFF");
}

void led::toggle()
{
  static unsigned long ledMillis = millis(); /*Assigns the current snapshot of time only the first time this code executes*/
  if(millis() - ledMillis > 700)
  {
    if(ledcRead(this->ledPin) == DUTYCYCLE0)
      ledcWrite(this->ledPin, DUTYCYCLE100);
    else
      ledcWrite(this->ledPin, DUTYCYCLE0);
    ledMillis = millis();  

    if(this->debugStatus) Serial.println(this->ledId+": Toggle");
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
  if(this->debugStatus) Serial.println(this->ledId+": Blink Twice");
}

void led::printInfo()
{
	if(this->debugStatus) Serial.println(this->ledId+" object initilized");
	delay(1000);
  return;
}

//Destructor
led::~led()
{
  ledcDetach(this->ledPin);
  if(this->debugStatus) Serial.println(this->ledId+" object destroyed");
}

#endif  //END led_h
