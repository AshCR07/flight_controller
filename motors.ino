// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions

void set_velocities() {
  float ratio = smoothed_control_z / 730;
  int fl=1064 + (500 + roll_output + pitch_output) * ratio;
  int fr=1064 + (500 - roll_output + pitch_output) * ratio;
  int rl=1064 + (500 + roll_output - pitch_output) * ratio;
  int rr=1064 + (500 - roll_output - pitch_output) * ratio;
  
  PORTB=16;
  TCNT1=0;
  while(TCNT1 < fl * 2);
  PORTB=32;
  TCNT1=0;
  while(TCNT1 < fr * 2);
  PORTB=64;
  TCNT1=0;
  while(TCNT1 < rl * 2);
  PORTB=128;
  TCNT1=0;
  while(TCNT1 < rr * 2);
  PORTB=0;
}

