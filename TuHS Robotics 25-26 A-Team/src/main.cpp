#include "stuff.h"
#include "main.h"
#include "replay.h"

string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
ReplayController replayController(REPLAY_FILE);

// Lift motors
float LIFT_MAX_SPEED = 127;
pros::Motor bottom(1); // Full motor
pros::Motor middle(11); // Half motor
pros::Motor top(20); // Half motor
int DRIVE_MAX_SPEED = 100;

//MOTOR DEFINITIONS
pros::Motor right_front(8);
pros::Motor right_middle(9);
pros::Motor right_back(10);
pros::Motor left_front(3);
pros::Motor left_middle(4);
pros::Motor left_back(5);

int direction = 1;
bool backwardsPressed = false;


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Shall I compare thee to a summer's day?");
	pros::lcd::set_text(2, "Thou art more lovely and more temperate");
	pros::lcd::set_text(3, "And the rough winds shake");
	pros::lcd::set_text(4, "The darling buds of May.");
	pros::lcd::set_text(5, "And summer's lease");
	pros::lcd::set_text(6, "Hath all too short a date.");
}


void disabled() {}


void competition_initialize() {}


void drive(auto master){
	bool l1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
	bool l2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
	bool r1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
	bool r2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

	if(l1){// All the way up
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(-LIFT_MAX_SPEED * 0.5);
	}
	else if(l2){// All the way down
		bottom.move(-LIFT_MAX_SPEED);
		middle.move(-LIFT_MAX_SPEED * 0.5);
		top.move(0);
	}
	else if(r2){// Up and out
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * -0.5);
		top.move(0);
	}
	else if(r1){// Down and into basket
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(LIFT_MAX_SPEED * 0.5);
	}
	else{
		bottom.move(0);
		middle.move(0);
		top.move(0);
	}

	
	
	if(backwardsPressed != master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
		backwardsPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
		if(backwardsPressed && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
			direction *= -1;
		}
	}

	int left_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
	float right_speed = 0;
	float left_speed = 0;

	if(direction == 1){
		right_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
		left_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);
	}
	else if(direction == -1){
		right_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);
		left_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
	}

	right_front.move(right_speed);
	right_middle.move(right_speed);
	right_back.move(right_speed);

	left_front.move(left_speed);
	left_middle.move(left_speed);
	left_back.move(left_speed);
}

void opcontrol() {
	while (true) {
		drive(playerController);
		
		replayController.record(playerController, theFile);
		//pros::delay(20);
		pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}

void autonomous() {
	theFile.open(REPLAY_FILE, std::ios_base::in);
	while(true) {
		replayController.updateFrame(false);
		drive(replayController);
		//pros::delay(20);
		pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}