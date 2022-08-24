#include <BTS7960.h>

/* The default constructor has pins assigned as:
 * L_EN = 2 
 * R_EN = 4
 * L_PWM = 5
 * R_PWM = 6
 * so no need of passing the paramaters through the parameter list
 */
  
#define L_EN 2
#define R_EN 4
#define L_PWM 5                             //pin 5 supports 980hz pwm frequency
#define R_PWM 6                             //pin 6 supports 980hz pwm frequency
//#define L_IS 7
//#define R_IS 8

//BTS7960 default constructor
//BTS7960 motor1()

//Parametrized constructor
BTS7960 motor1(L_EN, R_EN, L_PWM, R_PWM);

void setup() {
  Serial.begin(9600);
  motor1.begin();
  motor1.enable(); 
}

void loop() {
  
  for(int i=0; i<=255; i=i+10)
  { 
    //clockwise rotation
    motor1.pwm = i;
    motor1.front();
    delay(500);
  }
  motor1.stop();
  delay(500);
  for(int i=0; i<=255; i=i+10)
  { 
    //anti-clockwise rotation
    motor1.pwm = i;
    motor1.back();
    delay(500);
  }
  delay(500);
  motor1.disable();
}
