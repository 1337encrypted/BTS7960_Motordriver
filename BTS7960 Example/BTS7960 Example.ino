#include <BTS7960_Motordriver.h>

//Motor PWM properties
const uint32_t freq = 10000;
const uint8_t resolution = 8;

//BTS7960 motor driver 2 pin definitions
constexpr uint8_t leftMotorsId = 1;
constexpr uint8_t RPWM1 = 26;
constexpr uint8_t LPWM1 = 27;                   
constexpr uint8_t R_EN1 = 25;  
constexpr uint8_t L_EN1 = 14;
constexpr uint8_t R_IS1 = -1;        //Alarm pin
constexpr uint8_t L_IS1 = -1;        //Alarm pin

constexpr MOTORDEBUG=true;

BTS7960_ESP32 motor1(L_EN1, R_EN1, LPWM1, RPWM1, freq, resolution, L_IS1, R_IS1, leftMotorsId, MOTORDEBUG);       //Create an object of class motor1

void setup() {
  Serial.begin(9600);                       //begin the serial monitor for output
  motor1.begin();                           //This method will set the motor driver pins as output
  motor1.enable();                          //This method will set the L_EN and R_EN to HIGH or digitalWrite them to +5v/3v depending on your mcu
}

void loop() {
  
  for(int i=0; i<=255; i=i+10)
  {
    motor1.speed = i;                         //Set the speed, by default the speed is set to 255 you can change it 
    motor1.front();                         //front functions should turn the motor in clockwise direction
    delay(500);
  }
  motor1.stop();                            //stop function should halt the motor by applying 0 pwm to both R_PWM and L_PWM
  delay(500);
  for(int i=0; i<=255; i=i+10)
  { 
    motor1.speed = i;                         //Set the speed
    motor1.back();                          //back functions should turn the motor in anti-clockwise direction
    delay(500);
  }
  delay(500);
}
