/*
  VRC 2018-19 Turning Point
  Autonomous Code by Vishal Dhayalan
  Team Cygnus 7975E
*/

#include "main.h"
#include "cygnus.hpp"         // Include Cygnus Library

const char Team = 'B';
const char side = 'C';

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
  set_Team(Team, side);
  drive cygnus(4, 18.5, 1.0);
  intake intake(1);
  intake.run(true);
  arm arm;
  arm.reset();
  variable variable(pitchDia, rack_len);
  variable.reset();
  puncher puncher(5, 60, false);
  Var_Puncher.move_absolute(360 * (rack_len/pitchDia), 200);

  if (Team == 'B') {
    if (side == 'C') {
      //==================================================== BLUE CAPS AUTON
      cygnus.Right_drive('f', 200, 47, false);
      cygnus.Left_drive('f', 200, 47, true);
      pros::delay(200);
      cygnus.Right_drive('r', 125, 28, false);
      cygnus.Left_drive('r', 125, 28, true);
      pros::delay(10);
      cygnus.turn('a', 60, 200, true);
      pros::delay(10);
      cygnus.Right_drive('f', 200, 28, false);
      cygnus.Left_drive('f', 200, 28, true);
      arm.flip_Cap(30*5);
      pros::delay(20);
      cygnus.Right_drive('r', 150, 8, false);
      cygnus.Left_drive('r', 150, 8, true);
      cygnus.turn('c', 130, 160, true);
      pros::delay(20);
      cygnus.Right_drive('r', 150, 3, false);
      cygnus.Left_drive('r', 150, 3, true);
      puncher.shoot();
    }
    //==================================================== BLUE FLAGS AUTON
    else if (side == 'F') {

    }
  }
  else if (Team == 'R') {
    if (side == 'C') {
      //==================================================== RED CAPS AUTON
      cygnus.Right_drive('f', 200, 47, false);
      cygnus.Left_drive('f', 200, 47, true);
      pros::delay(200);
      cygnus.Right_drive('r', 125, 28, false);
      cygnus.Left_drive('r', 125, 28, true);
      pros::delay(10);
      cygnus.turn('c', 40, 200, true);
      pros::delay(10);
      cygnus.Right_drive('f', 200, 24, false);
      cygnus.Left_drive('f', 200, 24, true);
      arm.flip_Cap(30*5);
      pros::delay(20);
      cygnus.Right_drive('r', 150, 10, false);
      cygnus.Left_drive('r', 150, 10, true);

      cygnus.turn('a', 140, 160, true);
      pros::delay(20);
      cygnus.Right_drive('r', 100, 8, false);
      cygnus.Left_drive('r', 100, 8, true);
      pros::delay(25);
      puncher.shoot();
    }
    //==================================================== RED FLAGS AUTON
    else if(side == 'F') {

    }
  }
}
