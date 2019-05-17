/*
  VRC 2018-19 Turning Point
  Robot configuration file by Vishal Dhayalan
  Team Cygnus 7975E
*/

#ifndef CONFIG_CPP
#define CONFIG_CPP

#include "main.h"

pros::Controller master(CONTROLLER_MASTER);																				// Controller definition

pros::Motor Right_M1(1, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);						// Front Right motor setup
pros::Motor Right_M2(2, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);						// Rear Right motor setup
pros::Motor Left_M1(4, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);						// Front Left motor setup
pros::Motor Left_M2(3, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);						// Rear Left motor setup
pros::Motor Intake(5, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);							// Intake motor setup
pros::Motor Arm(6, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);					      // Multi-Purpose Arm motor setup
pros::Motor Puncher(7, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);						// Puncher motor setup
pros::Motor Var_Puncher(8, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);				// Var_Puncher motor setup

pros::Vision ITK_cam(12);
pros::Vision VAR_cam(13, pros::E_VISION_ZERO_CENTER);

pros::vision_color_code_t red_flags = VAR_cam.create_color_code(1, 3);
pros::vision_color_code_t blue_flags = VAR_cam.create_color_code(2, 3);

pros::ADIDigitalIn Var_Limit('a');                                                // Variable Limit Switch setup
pros::ADIDigitalIn Conv_ballDetection('b');                                       // Conveyor Limit Switch (to detect balls) setup
pros::ADIDigitalIn Arm_Limit('c');                                                // Multi-Purpose Arm Limit switch setup

#endif
