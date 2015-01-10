#include <Wire.h>
#include "RTClib.h"
#include "suntime.h"
#include "motor.h"

// Clock
#define DS1307_I2C_ADDRESS 0x68
RTC_DS1307 rtc;

// Motor
#define LED_GRN_PIN A2 // Motor Active Indicator LED
#define LED_RED_PIN A3
/* Left & Right enable drive (both must be 
 * active for motor to run) */
#define LR_EN  8
#define R_PWM  9 // Right PWM "go"
#define L_PWM 10 // Left PWM "go"
#define MOTOR_ACTION_TIME 60000
#define MANUAL_SWITCH_OPEN 6
#define MANUAL_SWITCH_CLOSE 7

Motor mtr; 
int redled = 0;

// Sun Time
float sunrise, sunset;
SunTime suntime;
float openhours, closehours;
boolean isBooted = false;

void setup(){
  pinMode( MANUAL_SWITCH_CLOSE, INPUT_PULLUP);
  pinMode( MANUAL_SWITCH_OPEN,  INPUT_PULLUP);
  
  Wire.begin();
  rtc.begin();
  
  mtr.setup( LR_EN, L_PWM, R_PWM, LED_GRN_PIN);
  pinMode( LED_RED_PIN, OUTPUT);
}

void loop(){
  DateTime rightnow = rtc.now();
  redled ^= 1;
  //digitalWrite( LED_RED_PIN, redled);
  digitalWrite( LED_RED_PIN, rightnow.second() & 1);
  
  float dayhours;
  dayhours = rightnow.hour() + float(rightnow.minute())/60.
    + float(rightnow.second())/3600.;
    
  if( dayhours < 0.1 || !isBooted){
    // Calculate door open time and door close time         
    suntime.riseset( rightnow.year(), rightnow.month(), 
                     rightnow.day(), true, &openhours);
                     
    suntime.riseset( rightnow.year(), rightnow.month(), 
                     rightnow.day(), false, &closehours);
    
    closehours += 0.85;
    
    isBooted = true;
  }
  
  // Calculate the target door position
  // Manual switch overrides
  if( !digitalRead( MANUAL_SWITCH_OPEN)){
   mtr.open();
  }
  else if( !digitalRead( MANUAL_SWITCH_CLOSE)){
   mtr.close();
  }
  else if( dayhours > closehours){
    mtr.close();
  }
  else if( dayhours > openhours){
    mtr.open();
  }
  delay( 1000);
}
