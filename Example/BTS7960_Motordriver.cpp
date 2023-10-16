#include "BTS7960_Motordriver.h"

//Parametrised constructor with 6 parameters (still need to work on it, avoid it for right now)
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM, uint8_t L_IS, uint8_t R_IS, bool _debug)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz
    this->L_IS = L_IS;      //Alarm pin
    this->R_IS = R_IS;      //Alarm pin
    this->_debug = _debug;   //for seial monitor display
    //Set the global pwm variable to 255
    this->pwm = 255;
    
    if(this->_debug)
    {
        Serial.println("BTS7960 Motordriver library");
        Serial.print("Library version:");
        // Serial.println(this->version);
        Serial.println("Yash Herekar 2022");
    }
    
    begin();
    enable();
}

void BTS7960::begin()
{
    //Motor driver enable pins set as output and high
    if(this->L_EN > 0)    pinMode(this->R_EN, OUTPUT);
    if(this->R_EN > 0)    pinMode(this->L_EN, OUTPUT);
    if(this->L_PWM > 0)   pinMode(this->L_PWM, OUTPUT);
    if(this->R_PWM > 0)   pinMode(this->R_PWM, OUTPUT);
    if(this->R_IS > 0)    pinMode(this->R_IS, INPUT);
    if(this->L_IS > 0)    pinMode(this->L_IS, INPUT);
    
    if(this->_debug)    Serial.println("Motor driver initilized");
}

void BTS7960::enable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(this->R_EN, HIGH);
    digitalWrite(this->L_EN, HIGH);
    
    if(this->_debug)    Serial.println("Motor driver enabled");
}

void BTS7960::disable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(this->R_EN, LOW);
    digitalWrite(this->L_EN, LOW);
    
    if(this->_debug)    Serial.println("Motor driver disabled");
}

void BTS7960::stop()
{
    analogWrite(this->L_PWM,0);
    analogWrite(this->R_PWM,0);
    
    if(this->_debug)
    {
        Serial.print("Stop: ");
        Serial.println(L_PWM);
        Serial.print(" : ");
        Serial.print(R_PWM);
    }
}

void BTS7960::front()
{
    analogWrite(this->L_PWM,0);
    delayMicroseconds(100);
    analogWrite(this->R_PWM,pwm);
    delayMicroseconds(100);
    
    if(this->_debug)
    {
        Serial.print("Front: ");
        Serial.println(L_PWM);
        Serial.print(" : ");
        Serial.print(R_PWM);
    }
}

void BTS7960::back()
{
    analogWrite(this->L_PWM,pwm);
    delayMicroseconds(100);
    analogWrite(this->R_PWM,0);
    delayMicroseconds(100);
    
    if(this->_debug)
    {
        Serial.print("Back: ");
        Serial.println(L_PWM);
        Serial.print(" : ");
        Serial.print(R_PWM);
    }
}

void BTS7960::alarm()
{
    if(digitalRead(L_IS) || digitalRead(R_IS))
    {
        disable();
        if(_debug)  Serial.print("High current alarm");
    }
}

//Destructor
BTS7960::~BTS7960()
{
    if(this->_debug)    Serial.println("motor object destroyed");
}
