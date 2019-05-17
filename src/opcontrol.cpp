/*
  VRC 2018-19 Turning Point
  Driver Control Code by Vishal Dhayalan
  Team Cygnus 7975E
*/

#include "main.h"
#include "cygnus.hpp"         // Include Cygnus Library

// Config definitions and variables needed from Config.cpp file
extern pros::Controller master;
extern pros::Motor Var_Puncher;

const float pi = 3.1415926535;

// Driver Control function
void opcontrol() {
  drive cygnus(4, 18.5, 2.0);                                         // Drive Object
  intake intake(1);                                                   // Intake Object
  puncher puncher(5, 60, false);                                      // Puncher Object - Init set to False as it would have been done in initialise.cpp
  arm arm;                                                            // Arm Object
  variable Var_Puncher(pitchDia, rack_len);                           // Puncher Variable Object

  arm.reset();                                                        // Reset arm position
	Var_Puncher.reset();                                                // Reset Variable positon

  intake.run(true);                                                   // Start Intake
  Var_Puncher.manualSet(rack_len);                                    // Set Varaible to minimum angle by moving to the end of the track

  // Main Loop: runs for the entirity of the driver control period
	while (true) {
    // ------------ Controller Buttons handling ------------
    // Button R1 used to shoot balls by firing puncher
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {    // If button R1 is pressed, wait till released
        pros::delay(10);
      }
      puncher.shoot();                                              // Shoot Puncher after R1 is clicked (pressed & realesed)
    }

    // Button L1 used to flip caps using the arm
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {    // If button L1 is presed, wait till released
        pros::delay(10);
      }
      arm.flip_Cap(30*5);                                           // Run arm 'flip' sequence after L1 is clicked (pressed & released)
    }

    // Button L2 used to descore caps from poles using the arm
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {    // If button L2 is pressed, wait till released
        pros::delay(10);
      }
      arm.descore();                                                // Call arm 'descore' sequence after L2 is clicked (pressed & released)
    }

    // Button B used to activate/de-activate Smart Intake: A computer vision (CV) controlled intake system
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {     // If Button B is pressed, wait till released
        pros::delay(10);
      }
      intake.toggleCV();                                            // Toggle Intake CV after B is clicked (pressed & released)
    }

    // Up arrow button used to move arm away from the ground and allow the robot to climb onto platform from the front
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
      while(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {    // If Up arrow is pressed, wait till released
        pros::delay(10);
      }
      arm.platform();                                               // call arm 'platform' sequence after Up arrow is clicked (pressed & released)
    }

    // ------------ End of Controller Button handling section ------------

    intake.handle();                                                // Handle Intake functionalities : stop if 2 balls is possesed by the robot and CV based smart intake
    Var_Puncher.handle();                                           // Handle Variable functionalities : Auto adjust variable angle using Computer Vision to aim at flags

    // Remote Controlled Driving code section
		int left = (int) (mapToRPM(master.get_analog(ANALOG_LEFT_Y), 127.0, 200.0));    // Get Left joystick's Y-axis values and map them to Motor RPM values
		int right = (int) (mapToRPM(master.get_analog(ANALOG_RIGHT_Y), 127.0, 200.0));  // Get Right joystick's Y-axis values and map them to Motor RPM values
    cygnus.Right_drive('f', right, 0, false);                                       // Drive right side motors at Right motors' RPM as determined above
    cygnus.Left_drive('f', left, 0, false);                                         // Drive left side motors at Left motors' RPM as determined above

    pros::delay(10);                                                // Small Delay to free resources and ensure system stability
	}
}
