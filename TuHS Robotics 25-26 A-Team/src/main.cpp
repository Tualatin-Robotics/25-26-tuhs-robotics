#include "stuff.h"
#include "main.h"
#include "replay.h"

const string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
ReplayController replayController(REPLAY_FILE);

// Lift motors
float LIFT_MAX_SPEED = 127;
pros::Motor bottom(); // Full motor
pros::Motor middle(); // Half motor
pros::Motor top(); // Half motor
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
	while(true) {
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
	bool l1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
	bool l2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
	bool r1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
	bool r2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

	if(l1){// All the way up
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(0);
	}
	else if(l2){// All the way down
		bottom.move(-LIFT_MAX_SPEED);
		middle.move(-LIFT_MAX_SPEED * 0.5);
		top.move(0);
	}
	else if(r1){// Up and out
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(-LIFT_MAX_SPEED * 0.5);
	}
	else if(r2){// Down and into basket
		bottom.move(-LIFT_MAX_SPEED);
		middle.move(-LIFT_MAX_SPEED * 0.5);
		top.move(LIFT_MAX_SPEED * 0.5);
	}
	else{
		bottom.move(0);
		middle.move(0);
		top.move(0);
	}
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