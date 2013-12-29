// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define POSITION_FEEDBACK 300.0     // This is the pitch/roll feedback amount
#define POSITION_FEEDBACK_Z 1.0     // This is the yaw feedback amount
#define GYRO_FEEDBACK  0.06         // Rotational velovity correction
#define CONTROL_SENSITIVITY 0.6     // Pitch/roll control sensitivity
#define CONTROL_SENSITIVITY_Z 1.0   // Yaw control sensitivity
#define INTEGRATION_AMOUNT 0.002    // Pitch/roll integration amount

// Global Variables and Objects
double smoothed_control_x=0;        // Smoothed RC Input
double smoothed_control_y=0;        // Smoothed RC Input
double smoothed_control_t=0;        // Smoothed RC Input
double smoothed_control_z=0;      // Smoothed RC Input
int armed=0;                        // Motors enabled

double pos_x, pos_y, pos_z;              // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // Stabilization Output
double altitude_hold_correction;
double altitude_hold_control;
double integrated_x=0;
double integrated_y=0;
double integrated_z=0;

MPU6050 mpu;             // Motion processor
bool dmpReady = false;   // set true if DMP init was successful

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  
  // Process data
  process_rc_data();
  double x, y, z;
  x = smoothed_control_x - pos_x * POSITION_FEEDBACK - gyro_x * GYRO_FEEDBACK;
  y = smoothed_control_y - pos_y * POSITION_FEEDBACK - gyro_y * GYRO_FEEDBACK;
  z = smoothed_control_z - pos_z;
  if (z >  180) z -= 360;
  if (z < -180) z += 360;
  
  output_x = x + integrated_x;
  output_y = y + integrated_y;
  output_z = z;
  
  integrated_x += x * INTEGRATION_AMOUNT;
  integrated_y += y * INTEGRATION_AMOUNT;
  if(smoothed_control_t < 100) {
    integrated_x = 0;
    integrated_y = 0;
  }
  
  altitude_hold_correction = (sq(pos_x) + sq(pos_y)) * altitude_hold_control;
  Serial.print(altitude_hold_correction),
  Serial.print("\n");
  
  // Push data to motors
  set_velocities();
  
}

