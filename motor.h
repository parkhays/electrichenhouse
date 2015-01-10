#ifndef MOTOR_INCLUDE
#define MOTOR_INCLUDE

#include <arduino.h>

enum motor_state_t {MTR_UNKNOWN, MTR_CLOSED, MTR_OPEN};

class Motor{
public:
  int motor_state;
  int lr_en;
  int l_pwm;
  int r_pwm;
  int led_grn_pin;
  int motor_action_time;
  
  void setup( int ilr_en, int il_pwm, int r_pwm, int led_grn_pin);
  void open( void );
  void close( void );
  void disable( void );
  void wait( boolean openstate );
  void openclose( int, int, int);
};

#endif
