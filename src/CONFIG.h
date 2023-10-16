/* 
 * Debug on and off enables code optimization
 * DEBUG 0 will ignore all Serial.print functions
 * DEBUG 1 will will add back Serial.print functions back to the code
 */

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define DEBUG 1

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
  #define MOTORDEBUG true
  #define LEDDEBUG true
  #define BUZZERDEBUG true
  #define SYSTEMDEBUG true
#else
  #define debug(x)
  #define debugln(x) 
  #define MOTORDEBUG false
  #define LEDDEBUG false
  #define BUZZERDEBUG false
  #define SYSTEMDEBUG false
//  #define Serial.begin(x)
#endif
