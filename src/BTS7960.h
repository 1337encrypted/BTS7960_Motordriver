/*
 * BTS7960.h - Library for controlling a brushed DC motor using the BTS7960 motor driver.
 * Created by Yash Herekar, August 24, 2022.
 * Released into the public domain.
 */

/*Cannot create a .cpp file as inline prototypes need the function to be present in the same file*/

#ifndef BTS7960_h
#define BTS7960_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class BTS7960
{
    private:

    //L_IS1,R_IS1 are not included
    byte L_EN;
    byte R_EN;
    byte L_PWM;
    byte R_PWM;
      
    public:
    //pwm variable to control the speed of motor
    volatile uint8_t pwm;
/*================================================Functin prototyping section========================================================*/
    inline BTS7960() __attribute__((always_inline));
    inline BTS7960(byte, byte, byte, byte) __attribute__((always_inline));
    inline void begin() __attribute__((always_inline));
    inline void enable() __attribute__((always_inline));
    inline void disable() __attribute__((always_inline));
    inline void stop() __attribute__((always_inline));
    inline void front() __attribute__((always_inline));
    inline void back() __attribute__((always_inline));
/*===================================================================================================================================*/
};

//Default constructor
BTS7960::BTS7960()
{
    //Motor driver 1 pin definitions
    this->L_EN = 2;
    this->L_EN = 4;
    this->L_PWM = 5;        //pin 5 has PWM frequency of 980Hz
    this->R_PWM = 6;        //pin 5 has PWM frequency of 980Hz
    //this->L_IS1 = L_IS1      //Alarm pin
    //this->R_IS1 = R_IS1      //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
}

//Parametrised constructor
BTS7960::BTS7960(byte L_EN, byte R_EN, byte L_PWM, byte R_PWM)
{
    //Motor driver 1 pin definitions
    this->L_EN = L_EN;
    this->L_EN = R_EN;
    this->L_PWM = L_PWM;
    this->R_PWM = R_PWM;
    //this->L_IS1 = L_IS1      //Alarm pin
    //this->R_IS1 = R_IS1      //Alarm pin

    //Set the global pwm variable to 255
    this->pwm = 255;
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

void BTS7960::begin()
{
    //Motor driver enable pins set as output and high
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);

    //PWM is for direction and pwm
    pinMode(R_PWM, OUTPUT);
    pinMode(L_PWM, OUTPUT);
}

void BTS7960::stop()
{
    analogWrite(R_PWM,0);
    analogWrite(L_PWM,0);
}

void BTS7960::front()
{
    analogWrite(R_PWM,pwm);
    delayMicroseconds(100);
    analogWrite(L_PWM,0);
    delayMicroseconds(100);
}

void BTS7960::back()
{
    analogWrite(R_PWM,0);
    delayMicroseconds(100);
    analogWrite(L_PWM,pwm);
    delayMicroseconds(100);
}
#endif  //END BTS7960_H
