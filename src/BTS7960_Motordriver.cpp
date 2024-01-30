//Static variables initilisation
String BTS7960_ESP32::version="";

//Default constructor
BTS7960_ESP32::BTS7960_ESP32(){}

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960_ESP32::BTS7960_ESP32(const uint8_t& L_EN, const uint8_t& R_EN, const uint8_t& L_PWM, const uint8_t& R_PWM, const uint32_t& freq, const uint8_t& resolution, const uint8_t& L_IS, const uint8_t& R_IS, const uint8_t& ID, const bool& debugStatus) :
L_EN(L_EN),
R_EN(R_EN),
L_PWM(L_PWM),              //pin 5 has PWM frequency of 980Hz
R_PWM(R_PWM),              //pin 6 has PWM frequency of 980Hz
freq(freq),                //frequency set to 20 khz
resolution(resolution),    //12 bit resolution
L_IS(L_IS),                //Alarm pin
R_IS(R_IS),                //Alarm pin
ID(ID),                    //for seial monitor display
debugStatus(debugStatus)
{
  speed = 70;
  version = "1.0.0";
}

void BTS7960_ESP32::begin()
{
  if(this->L_EN > 0)  pinMode(this->R_EN, OUTPUT);    //Motor driver enable pins set as output and high
  if(this->R_EN > 0)  pinMode(this->L_EN, OUTPUT);
  // if(this->L_PWM > 0) pinMode(this->L_PWM, OUTPUT);   //PWM is for direction and pwm
  // if(this->R_PWM > 0) pinMode(this->R_PWM, OUTPUT);
  if(this->R_IS > 0) pinMode(this->R_IS, INPUT);     //R_IS and L_IS alarm pins
  if(this->L_IS > 0)  pinMode(this->L_IS, INPUT);

  // Configure PWM channels
  // ledcSetup(this->PWMChannel1, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/
  // ledcSetup(this->PWMChannel2, this->freq, this->resolution);  /*PWMChannel, 20 kHz frequency, 12-bit resolution*/

  // Attach PWM channels to pins
  ledcAttach(this->L_PWM, this->freq, this->resolution);
  ledcAttach(this->R_PWM, this->freq, this->resolution);
}

void BTS7960_ESP32::enable()
{
  //Setting the BTS7960_ESP32 enable pins high
  digitalWrite(this->R_EN, HIGH);
  digitalWrite(this->L_EN, HIGH);

  if(this->debugStatus) Serial.println((String)this->ID+" initilized and enabled");
  return;
}

void BTS7960_ESP32::disable()
{
  //Setting the BTS7960_ESP32 enable pins low
  digitalWrite(this->R_EN, LOW);
  digitalWrite(this->L_EN, LOW);

  if(this->debugStatus) Serial.println((String)this->ID+" motor driver disabled");
  return;
}

void BTS7960_ESP32::stop()
{
  ledcWrite(this->L_PWM,this->DEFAULTSPEED);
  ledcWrite(this->R_PWM,this->DEFAULTSPEED);
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" STOP: "+this->DEFAULTSPEED);
}

void BTS7960_ESP32::front()
{
  if( this->prevMotorStatus != this->motorStatus )
  {
    ledcWrite(this->L_PWM,this->DEFAULTSPEED);
    delayMicroseconds(50);
    ledcWrite(this->R_PWM,this->speed);

    this->prevMotorStatus = this->motorStatus;
  }
  else
  {
    ledcWrite(this->L_PWM,this->DEFAULTSPEED);
    ledcWrite(this->R_PWM,this->speed);
  }
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" FRONT: "+this->speed);
}

void BTS7960_ESP32::back()
{
  if( this->prevMotorStatus != this->motorStatus )
  {
    ledcWrite(this->R_PWM,this->DEFAULTSPEED);
    delayMicroseconds(50);
    ledcWrite(this->L_PWM,this->speed);

    this->prevMotorStatus = this->motorStatus;
  }
  else
  {
    ledcWrite(this->R_PWM,this->DEFAULTSPEED);
    ledcWrite(this->L_PWM,this->speed);
  }
  
  if(this->debugStatus) Serial.println("Motor "+(String)ID+" BACK: "+this->speed);
}

void BTS7960_ESP32::alarm()
{
  if(digitalRead(L_IS) || digitalRead(R_IS))
  {
      disable();
      if(this->debugStatus) Serial.println("High current alarm");
  }  
}

void BTS7960_ESP32::printInfo()
{
  Serial.println();
  Serial.println("BTS7960_ESP32 Motordriver library");
  Serial.println("Library version:");
  Serial.println(version);
  Serial.println("Yash Herekar 2023");
  
  delay(1000);
  return;
}

//Destructor
BTS7960_ESP32::~BTS7960_ESP32()
{
  ledcDetach(this->L_PWM);
  ledcDetach(this->R_PWM);
  if(this->debugStatus) Serial.println("motor object destroyed");
  return;
}
