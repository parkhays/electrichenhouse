#include "motor.h"

void Motor::setup( int ilr_en, int il_pwm, int ir_pwm,
		   int iled_grn_pin){
  lr_en = ilr_en;
  l_pwm = il_pwm;
  r_pwm = ir_pwm;
  led_grn_pin = iled_grn_pin;
  motor_state = MTR_UNKNOWN;
  
  pinMode( led_grn_pin, OUTPUT);
  pinMode( lr_en, OUTPUT);
  pinMode( r_pwm, OUTPUT);
  pinMode( l_pwm, OUTPUT);
}

void Motor::disable( void){
  digitalWrite( lr_en, LOW);
  digitalWrite( r_pwm, LOW);
  digitalWrite( l_pwm, LOW);
  digitalWrite( led_grn_pin, LOW);
}

void Motor::openclose( int pin_en, int pin_pwm, int motor_mode){
  disable(); // turn off any running
  digitalWrite( pin_en, HIGH);
  digitalWrite( pin_pwm, HIGH);
  wait( motor_mode);
  disable();
}

void Motor::open( void){
  // if we are already open, then bail
  if( motor_state == MTR_OPEN) return;
  openclose( lr_en, l_pwm, 1);
  motor_state = MTR_OPEN;
}

void Motor::close( void){
  if( motor_state == MTR_CLOSED) return;
  openclose( lr_en, r_pwm, 0);
  motor_state = MTR_CLOSED;
}

void Motor::wait( boolean openstate){
  unsigned long blink_dur1 = 50, blink_dur2 = 100;
  unsigned const long total_delay = 35000;
  unsigned long waited = 0;

  while( waited < total_delay){
    digitalWrite( led_grn_pin, openstate);
    delay( blink_dur1);
    
    digitalWrite( led_grn_pin, !openstate);
    delay( blink_dur2);
    
    waited += blink_dur1 + blink_dur2;
  }
}
