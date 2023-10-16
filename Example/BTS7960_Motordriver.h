/*
 * BTS7960.h - Library for controlling a brushed DC motor using the BTS7960 43amp motor driver.
 * Created by Yash Herekar, August 31, 2022.
 * Released into the public domain.
 * Copyright [2022] [Yash Herekar]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 */

/*Cannot create a .cpp file as  prototypes need the function to be present in the same file*/

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
    uint8_t R_EN;
    uint8_t L_EN;
    uint8_t R_PWM;
    uint8_t L_PWM;
    uint8_t R_IS;
    uint8_t L_IS;
    // static string version;
    
    public:
    
    //pwm variable to control the speed of motor
    uint8_t pwm;
    bool _debug;
    
    /*================================================Function prototyping section========================================================*/
    BTS7960(uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, uint8_t=-1, bool=true);  // L_EN, R_EN, L_PWM, R_PWM, L_IS, R_IS, debug
    void begin();
    void enable();
    void disable();
    void stop();
    void front();
    void back();
    void alarm();
    ~BTS7960();
    /*====================================================================================================================================*/
};
#endif  //END BTS7960_H
