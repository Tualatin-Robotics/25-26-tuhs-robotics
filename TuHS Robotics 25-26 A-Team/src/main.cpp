#include "stuff.h"
#include "main.h"
#include "replay.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
int DRIVE_MAX_SPEED = 100;

pros::Motor right_front(19);
pros::Motor right_back(20);
pros::Motor left_front(9);
pros::Motor left_back(10);


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
	vector<double> values = updateFrame();
	right_front.move_relative(values[0], 120);
	right_back.move_relative(values[1], 120);
	left_front.move_relative(values[2], 120);
	left_back.move_relative(values[3], 120);
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	right_front.set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_DEGREES);
	right_back.set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_DEGREES);
	left_front.set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_DEGREES);
	left_back.set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_DEGREES);

}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	theFile.open(fileName, std::ios::in);
	pros::lcd::set_text(1, "Auton started!");
	while (true) {
		drive_replay();
		pros::delay(20);
	}
	theFile.close();
}

void opcontrol() {
	pros::lcd::set_text(1, "Opcontrol started! Go Wolves!");

	while (true) {
		/*vector<int> values = {
			right_front.get_voltage(),
			right_back.get_voltage(),
			left_front.get_voltage(),
			left_back.get_voltage()
		};*/
		vector<double> values = {
			right_front.get_position(),
			right_back.get_position(),
			left_front.get_position(),
			left_back.get_position()
		};
		drive();
		record(master, values);

		pros::delay(20);
	}

	pros::lcd::set_text(1, "End of Opcontrol :D");
}