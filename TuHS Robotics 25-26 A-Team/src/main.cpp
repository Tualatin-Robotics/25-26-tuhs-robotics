#include "stuff.h"
#include "main.h"
#include "replay.h"

const string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
ReplayController replayController(REPLAY_FILE);

int DRIVE_MAX_SPEED = 100;

//MOTOR DEFINITIONS
pros::Motor right_front(1, true);
pros::Motor right_middle(2, true);
pros::Motor right_back(3, true);
pros::Motor left_front(11);
pros::Motor left_middle(19);
pros::Motor left_back(20);



void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Better a witty fool than a foolish wit!");
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	theFile.open(REPLAY_FILE, std::ios_base::in);
	while(true) {right_front
		replayController.updateFrame(false);
		drive(replayController);
		pros::delay(20);
		//pros::delay(20-pros::millis()%20);
	}
	theFile.close();
}


void opcontrol() {
	while (true) {
		drive(playerController);
		pros::delay(20);
	}
}


void drive(auto master){
	int left_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

	int right_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
	int left_speed = left_joystick * (DRIVE_MAX_SPEED / 127.0);

	right_front.move(right_speed);
	right_middle.move(right_speed);
	right_back.move(right_speed);

	left_front.move(left_speed);
	left_middle.move(left_speed);
	left_back.move(left_speed);
}