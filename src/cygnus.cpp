/*
  VRC 2018-19 Turning Point
  Cygnus class by Vishal Dhayalan
  Team Cygnus 7975E
*/

#include "cygnus.hpp"

int balls = 0;                                                                  // Number of balls carried
bool ITK_active = false;                                                        // Intake active flag
long ball_time;                                                                 // Start time of ball related timing tasks
long CV_ballTime;                                                               // Start time of intake CV ball detection
char _team;
char _team_side;

void set_Team(char team, char side) {
  _team = team;
  _team_side = side;
}

float mapToRPM(float input, float UB_input, float UB_output) {                  // Maps joystick values (-127 to +127) to RPM values for motor (-200 to +200 RPM)
  return ((input/UB_input) * UB_output);
}

// -----------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------  DRIVE CLASS  ----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

drive::drive(int wheel_dia, float wheel_base_width, float turn_factor) {        // Drive Construct - Init drive related variables and motor brake modes
  circumference = pi * wheel_dia;
  _wheelBase_W = wheel_base_width;
  _turn_fac = turn_factor;
  Right_M1.set_brake_mode(MOTOR_BRAKE_BRAKE);
  Right_M2.set_brake_mode(MOTOR_BRAKE_BRAKE);
  Left_M1.set_brake_mode(MOTOR_BRAKE_BRAKE);
  Left_M2.set_brake_mode(MOTOR_BRAKE_BRAKE);
}

void drive::Right_drive(char dir, int speed, float distance, bool Complete) {   // Drive function for Right channel
    float degs = (distance / circumference) * 360;                              // Degrees the wheel has to rotate to cover that distance (stored in degs variable)
    if (dir == 'r') {
      if (distance == 0) {
        speed = -1 * speed;
      }
      else {
        degs = -1 * degs;                                                       // Set degs to -degs if wheels need to reverse
      }
    }
    // Reset Right drive encoders to 0 at current positon
    Right_M1.tare_position();
    Right_M2.tare_position();
    // Open/unlimited drive
    if (distance == 0) {
        Right_M1.move_velocity(speed);
        Right_M2.move_velocity(speed);
    }
    // Controlled distance drive
    else {
        // Below lines move the motors that many degrees to cover the distance given - last argument (true/false) is whether or not it should wait for motor to complete the degrees
        Right_M1.move_relative(degs, speed);
        Right_M2.move_relative(degs, speed);
        if (Complete) {
            while(!((Right_M2.get_position() < degs + 2) && (Right_M2.get_position() > degs - 2))) {
              pros::delay(1);
            }
        }
    }
}

void drive::Left_drive(char dir, int speed, float distance, bool Complete) {
    float degs = (distance / circumference) * 360;                              // Degrees the wheel has to rotate to cover that distance (stored in degs variable)
    if (dir == 'r') {
      if (distance == 0) {
        speed = -1 * speed;
      }
      else {
        degs = -1 * degs;                                                       // Set degs to -degs if wheels need to reverse
      }
    }
    // Reset Left drive encoders to 0 at current position
    Left_M1.tare_position();
    Left_M2.tare_position();
    if (distance == 0) {
        Left_M1.move_velocity(speed);
        Left_M2.move_velocity(speed);
    }
    else {
        // Below lines move the motors that many degrees to cover the distance given - last argument (true/false) is whether or not it should wait for motor to complete the degrees
        Left_M1.move_relative(degs, speed);
        Left_M2.move_relative(degs, speed);
        if (Complete) {
            while(!((Left_M2.get_position() < degs + 2) && (Left_M2.get_position() > degs - 2))) {
              pros::delay(1);
            }
        }
    }
}

void drive::turn(char dir, int angle, int speed, bool complete) {
  float displacement = ((angle * _turn_fac)/360.0) * pi * _wheelBase_W;         // Calculate the distance each wheel should travel to perform a turn of given angle
  // printf("Displacement: %f\n", displacement);                                // Output calculated displacement (for debugging)
  if(dir == 'c') {                                                              // Clockwise turning
    Left_drive('f', speed, displacement, false);
    Right_drive('r', speed, displacement, complete);
  }
  else if(dir == 'a') {                                                         // Anti-Clockwise turning
    Right_drive('f', speed, displacement, false);
    Left_drive('r', speed, displacement, complete);
  }
}

// -----------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------  PUNCHER CLASS  ---------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

puncher::puncher(float ext_gear_factor, int slip_angle, bool init) {
  // Initialize private variables
  _ScaleFactor = ext_gear_factor;
  _slipAngle = slip_angle;
  if(init) {
    Puncher.move_relative((360 - _slipAngle - 3) * _ScaleFactor, 200);          // Pull back puncher at start, 200 RPM - stop 3 degs from slip angle
  }
}

void puncher::shoot() {
  Puncher.move_relative(15 * _ScaleFactor, 200);                                // Move 12 degrees into slip angle (was 3 degs from slip angle)
  if(balls > 0) {
    balls = balls - 1;                                                          // Decrement number of balls held by robot by 1
  }
  pros::delay(200);                                                             // Wait for puncher to complete fire
  ball_time = pros::millis();
  // Reload: (slipAngle - 10) degs to re-engage gear + (360 - _slipAngle - 3) to pull back and stop 3 degs from slip
  Puncher.move_relative((_slipAngle - 10 + (360 - _slipAngle - 3)) * _ScaleFactor, 200);
}


// ------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------  VARIABLE CLASS  ---------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

variable::variable(float pinion_PitchDia, float rack_length) {                  // Construct - initialises private varibales
  _pitchDia = pinion_PitchDia;
  _rackLen = rack_length;
  _travelLimit = 360 * (_rackLen/_pitchDia);                                    // Max travel of the motor in degrees
}

void variable::reset() {                                                        // Reset variable to max angle (pinion at the bottom of the rack)
  Var_Puncher.set_brake_mode(MOTOR_BRAKE_BRAKE);
  Var_Puncher.move_velocity(-200);
  while(!(Var_Limit.get_value())) {                                             // Move motor until limit switch is pressed
    pros::delay(5);
  }
  // Stop motor and reset variable encoder to 0 at current position
  Var_Puncher.move_velocity(0);
  Var_Puncher.tare_position();
}

void variable::manualSet(float travel_dist) {                                   // Moves Variable to a certain distance on the track (bottom of the track is 0)
  Var_Puncher.move_absolute(360 * (travel_dist/pitchDia), 200);                 // Calculate motor rotation needed and move motor at 200RPM
}

void variable::handle() {                                                       // Handles automatic aiming at flags using computer vision
  pros::vision_object_s_t flags = ITK_cam.get_by_code(0, red_flags);            // Get largest red flag by default
  if(_team == 'R') {
    pros::vision_object_s_t flags = ITK_cam.get_by_code(0, blue_flags);         // If Red team, get largest blue flag
  }

  if(flags.y_middle_coord > 10) {
    Var_Puncher.move_velocity(150);                                             // Start decreasing Puncher angle at 150 RPM if flag is lower by > 10
  }
  else if(flags.y_middle_coord < 10) {
    Var_Puncher.move_velocity(-150);                                            // Start increasing Puncher angle at 150RPM if flag is higher by > 10
  }
  else {
    Var_Puncher.move_velocity(0);                                               // Flag within +- 10 from vertical centre : stop variable Motor
  }
}

// ------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------  INTAKE CLASS  ----------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

intake::intake(int init_balls) {
  balls = init_balls;
  _ITK_ballMode = false;                                                        // Intake set to flip caps by default
  Intake.set_brake_mode(MOTOR_BRAKE_BRAKE);
}

// Start/Stop intake
void intake::run(bool run) {
  if(run) {
    Intake.move_velocity(200);
  }
  else {
    Intake.move_velocity(0);
  }
  ITK_active = run;
}

// Switch intake direction (and hence it's mode)
void intake::switch_dir() {
  run(false);                                                                   // Stop intake
  if(_ITK_ballMode) {                                                           // If in ball intake mode, reverse to cap flipping mode
    Intake.set_reversed(false);
    _ITK_ballMode = false;
  }
  else {                                                                        // Cap flipping mode to ball intake mode
    Intake.set_reversed(true);
    _ITK_ballMode = true;
  }
  run(true);                                                                    // Start intake (motor direction will be switched)
}

// Handles smart intake: always on cap flipping mode except when ball detected near intake region. Switches to intake ball and reverts back to cap flipping
void intake::handle() {
  // printf("Balls: %d\n", balls);                                              // Print number of ball held by the robot (for debugging)
  if(balls < 2) {
    run(true);                                                                  // Intake is only running if there is <2 balls held by the robot
  }
  // Tracks number of balls held by the robot and controls intake accordingly, to ensure only a max of 2 balls is held at any time
  if(Conv_ballDetection.get_value() && ITK_active && (pros::millis() - ball_time) > 1000) {
    // If limit switch in conveyor is pressed (by a ball) + intake active + gap between previous ball detection in conveyor and current is >1000ms (1 second)
    balls = balls + 1;                                                          // Increment count of balls held by robot
    ball_time = pros::millis();                                                 // Set time of previous detection of ball in the conveyor to now
    if(balls == 2) {
      run(false);                                                               // Stop intake if this is the second ball in the robot
    }
  }

  // Handles computer vision of smart intake
  if(_ITK_CV) {                                                                 // If smart intake is on...
    pros::vision_object_s_t ball = ITK_cam.get_by_sig(0, 1);                    // Get largest object of the ball's colour detected by the camera
    if(ITK_active) {
      if(50 < ball.left_coord && ball.left_coord < 240 && ball.top_coord < 180 && ball.height > 30 && ball.width > 30) {
        // If detected object is around the size of a ball and is in the defined region around the intake...
        CV_ballTime = pros::millis();                                           // Set time of last ball detection in smart intake region to now
        if(!(_ITK_ballMode)) {
          switch_dir();                                                         // Switch to ball intake mode if not already in this mode
        }
      }
      // Object doesn't meet requirements or no objects detected by camera
      else if (_ITK_ballMode && (pros::millis() - CV_ballTime) > 1750 && (pros::millis() - ball_time) > 1000) {
         // If in ball intake mode
         // and time since last ball detected in smart intake region is >1750ms (1.75 secs) - to allow for a previously seen ball to be intaked and travel to conveyor limit switch
         // and time since last ball detection in conveyor >1000ms (1 sec) - to allow for a ball to travel from limit switch to puncher
        switch_dir();                                                           // Switch intake back to cap flipping mode
      }
    }
  }
}

void intake::toggleCV() {                                                       // Activate/deactivate smart intake
  _ITK_CV = !_ITK_CV;
}

bool intake::CV_status() {                                                      // Get status of smart intake (active or inactive)
  return _ITK_CV;
}

// -----------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------  ARM CLASS  -----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

arm::arm() {
  Arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
  // Initliase arm at the start of program, to be fully down
  Arm.move_velocity(-100);
  while(!(Arm_Limit.get_value())) {                                             // Move arm down until limit switch is pressed
    pros::delay(5);
  }
  Arm.move_velocity(0);                                                         // Stop arm
  pros::delay(25);
  Arm.tare_position();                                                          // Set intialised postion to be 0 by reseting it's encoder
}

void arm::reset() {
  Arm.move_absolute(1, 150);                                                    // Reset arm by moving it to the 1 degree positon of it's encoder, at 150 RPM
}

void arm::flip_Cap(int angle) {                                                 // Flip cap sequence
  Arm.move_absolute(angle, 180);
  pros::delay(300);
  reset();
}

void arm::descore() {                                                           // Sequence to descore caps from poles
  if(Arm.get_position() < 160) {
    Arm.move_absolute(180*5, 200);
    pros::delay(750);
  }
  else {
    reset();
  }
}

void arm::platform() {                                                          // Raise arm a bit, to allow robot to get on platforms
  if(Arm.get_position() < 35) {
    Arm.move_absolute(40*5, 200);                                               // Move arm up by 40 degs if current poisition is <35 degs (thus at reset postion)
    pros::delay(200);
  }
  else {
    reset();                                                                    // (Already at raised position) reset arm
  }
}
