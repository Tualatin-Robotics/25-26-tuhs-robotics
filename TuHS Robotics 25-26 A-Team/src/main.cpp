#include "stuff.h"
#include "main.h"
#include "replay.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
int DRIVE_MAX_SPEED = 100;

pros::Motor right_front(19);
pros::Motor right_back(20);
pros::Motor left_front(9);
pros::Motor left_back(10);

//string REPLAY_FILE = "/usd/test_motor_replay.txt";


void drive(){

	int left_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

	float right_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
	float left_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);

	right_front.move(right_speed);
	right_back.move(right_speed);

	left_front.move(left_speed);
	left_back.move(left_speed);
}

void drive_replay(){
	vector<int> values = updateFrame();
	right_front.move(values[0]);
	right_back.move(values[1]);
	left_front.move(values[2]);
	left_back.move(values[3]);
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	//openFile();
	theFile.open(fileName, std::ios::in);
	while (true) {
		drive_replay();
		pros::delay(20);
	}
	closeFile();
}

void opcontrol() {
	openFile();
	while (true) {
		vector<int> values = {
			right_front.get_voltage(),
			right_back.get_voltage(),
			left_front.get_voltage(),
			left_back.get_voltage()
		};
		drive();
		record(master, values);

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			break;
		}

		pros::delay(20);
	}
	pros::lcd::set_text(1, "End of Opcontrol");
	closeFile();
}