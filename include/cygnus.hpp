/*
  VRC 2018-19 Turning Point
  Cygnus class by Vishal Dhayalan
  Team Cygnus 7975E
*/

#ifndef CYGNUS_HPP
#define CYGNUS_HPP

#include "main.h"

extern pros::Motor Right_M1;						        // Front Right motor setup
extern pros::Motor Right_M2;						        // Rear Right motor setup
extern pros::Motor Left_M1;						          // Front Left motor setup
extern pros::Motor Left_M2;						          // Rear Left motor setup
extern pros::Motor Intake;							        // Intake motor setup
extern pros::Motor Arm;					                // Multi-Purpose Arm motor setup
extern pros::Motor Puncher;						          // Puncher motor setup
extern pros::Motor Var_Puncher;				          // Var_Puncher motor setup

extern pros::Vision ITK_cam;                    // Camera for smart intake setup
extern pros::Vision VAR_cam;                    // Variable puncher camera setup

extern pros::vision_color_code_t red_flags;     // Colour code for detecting red flags
extern pros::vision_color_code_t blue_flags;    // Colour code for detecting blue flags

extern pros::ADIDigitalIn Var_Limit;            // Variable Limit Switch setup
extern pros::ADIDigitalIn Conv_ballDetection;   // Conveyor Limit Switch (to detect balls) setup
extern pros::ADIDigitalIn Arm_Limit;            // Multi-Purpose Arm Limit switch setup


float mapToRPM(float input, float UB_input, float UB_output);
void set_Team(char team, char side);

class drive {
    private:
      float circumference, _wheelBase_W, _turn_fac, pi = 3.1415926535;
    public:
      drive(int wheel_dia, float wheel_base_width, float turn_factor);
      void Right_drive(char dir, int speed, float distance, bool Complete);
      void Left_drive(char dir, int speed, float distance, bool Complete);
      void turn(char dir, int angle, int speed, bool complete);
      void stop(std::string channel, char type);
};

class puncher {
    private:
      float _ScaleFactor, _slipAngle;
    public:
      puncher(float ext_gear_factor, int slip_angle, bool init);
      void shoot();
};

class variable {
    private:
      float _pitchDia, _rackLen, _travelLimit;
    public:
      variable(float pinion_PitchDia, float rack_length);
      void reset();
      void manualSet(float travel_dist);
      void handle();
};

class intake {
    private:
      bool _ITK_ballMode, _ITK_CV;
    public:
      intake(int init_balls);
      void run(bool run);
      void switch_dir();
      void handle();
      int* get_status();
      void toggleCV();
      bool CV_status();
};

class arm {
    public:
      arm();
      void reset();
      void flip_Cap(int angle);
      void descore();
      void platform();
};

#endif
