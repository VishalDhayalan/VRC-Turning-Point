/*
  VRC 2018-19 Turning Point
  Initialisation Code by Vishal Dhayalan
  Team Cygnus 7975E
*/

#include "main.h"
#include "cygnus.hpp"


char Team = 'B';
char side = 'C';

extern pros::ADIDigitalIn Var_Limit;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

static lv_res_t btn_click_action(lv_obj_t * btn) {
	uint8_t id = lv_obj_get_free_num(btn);
	Team = 'R';
	side = 'C';
	std::cout << id << "\n";
	return LV_RES_OK;
}

void initialize() {
	variable variable(pitchDia, rack_len);
	variable.reset();																	// Initialise variable position at the start of program
	puncher puncher(5, 60, true);											// Initliase puncher (puncher is pulled back and ready to shoot)
  arm arm;																					// Initalise arm
  arm.reset();																			// Move arm from intial position to reset position (which is at 1 deg) to release tension on the motor
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// Basic GUI for selecting team and side for each match
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_pos(btn1, 10, 10);
	lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action);
	lv_obj_t * lbl = lv_label_create(btn1, NULL);
	lv_label_set_text(lbl, "Blue Auton");
	lv_obj_set_pos(lbl, 0, 0);
}
