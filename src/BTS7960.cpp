#include "BTS7960.h"
//Default constructor
BTS7960::BTS7960()
{
    //Motor driver 1 pin definitions
    this->L_EN = 2;
    this->R_EN = 4;
    this->L_PWM = 5;        //pin 5 has PWM frequency of 980Hz
    this->R_PWM = 6;        //pin 6 has PWM frequency of 980Hz
//    this->L_IS = L_IS;      //Alarm pin
//    this->R_IS = R_IS;      //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Destructor
BTS7960::~BTS7960()
{
  Serial.println("motor object destroyed");
}

//Parametrised constructor with 6 parameters
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM, uint8_t L_IS, uint8_t R_IS)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz
//    this->L_IS = L_IS;      //Alarm pin
//    this->R_IS = R_IS;      //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor with 4 parameters
BTS7960::BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->R_EN = R_EN;
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor with 2 parameters
BTS7960::BTS7960(uint8_t L_PWM, uint8_t R_PWM)
{
    //Motor driver 1 pin definitions
    this->L_PWM = L_PWM;    //pin 5 has PWM frequency of 980Hz
    this->R_PWM = R_PWM;    //pin 6 has PWM frequency of 980Hz

    //Set the global pwm variable to 255
    this->pwm = 255;
}

void BTS7960::begin()
{
    //Motor driver enable pins set as output and high
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);

    //PWM is for direction and pwm
    pinMode(R_PWM, OUTPUT);
    pinMode(L_PWM, OUTPUT);
}

void BTS7960::enable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
}

void BTS7960::disable()
{
    //Setting the BTS7960 enable pins high
    digitalWrite(R_EN, LOW);
    digitalWrite(L_EN, LOW);
}

void BTS7960::stop()
{
    analogWrite(R_PWM,0);
    analogWrite(L_PWM,0);
}

void BTS7960::front()
{
    analogWrite(L_PWM,0);
    //delayMicroseconds(100);
    analogWrite(R_PWM,pwm);
    //delayMicroseconds(100);
}

void BTS7960::back()
{
    analogWrite(L_PWM,pwm);
    //delayMicroseconds(100);
    analogWrite(R_PWM,0);
    //delayMicroseconds(100);
}
