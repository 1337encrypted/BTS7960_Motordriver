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

    //L_IS1,R_IS1 are not included
    uint8_t R_EN;
    uint8_t L_EN;
    uint8_t R_PWM;
    uint8_t L_PWM;
//    uint8_t R_IS;
//    uint8_t L_IS;
    
    public:
    //pwm variable to control the speed of motor
    volatile uint8_t pwm;
/*================================================Functin prototyping section========================================================*/
    BTS7960();
     ~BTS7960();
     BTS7960(uint8_t, uint8_t);
     BTS7960(uint8_t, uint8_t, uint8_t, uint8_t);
     BTS7960(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
     void begin();
     void enable();
     void disable();
     void stop();
     void front();
     void back();
/*===================================================================================================================================*/
};
#endif  //END BTS7960_H
