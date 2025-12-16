#include "stuff.h"
#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
int DRIVE_MAX_SPEED = 100;

pros::Motor right_front(19);
pros::Motor right_back(20);
pros::Motor left_front(9);
pros::Motor left_back(10);

void reset_drivetrain_pos() {
	right_front.tare_position();
	right_back.tare_position();
	left_front.tare_position();
	left_back.tare_position();
}

#include "replay.h"

double t = 0.0;

void drive(){
	//right_front.move_relative(t, 100);
	//right_back.move_relative(t, 100);


	float left_joystick = round(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
	float right_joystick = round(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

	//t = double(left_joystick * 100);

	float right_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
	float left_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);

	right_front.move(right_speed);
	right_back.move(right_speed);

	left_front.move(left_speed);
	left_back.move(left_speed);
}

void drive_replay(){
	vector<double> values = updateFrame();
	int s = 200;
	/*right_front.move_absolute(values[0], s);
	right_back.move_absolute(values[1], s);
	left_front.move_absolute(values[2], s);
	left_back.move_absolute(values[3], s);*/

	//printf("%f\n", values[0]);

	double left = values[2];
	double right = values[0];

	double right_offset = values[4] - right_front.get_position();
	double left_offset = values[5] - left_front.get_position();

	right += right_offset * 2.0;
	left += left_offset * 2.0;

	right_front.move_voltage(int(right));
	right_back.move_voltage(int(right));
	left_front.move_voltage(int(left));
	left_back.move_voltage(int(left));
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
	reset_drivetrain_pos();
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

	/*right_front.move_relative(360, 360);
	right_back.move_relative(360, 360);
	left_front.move_relative(-360, 360);
	left_back.move_relative(-360, 360);*/


	while (true) {
		float r = 1.0;
		vector<double> values = {
			round((double)right_front.get_voltage() / r) * r,
			round((double)right_back.get_voltage() / r) * r,
			round((double)left_front.get_voltage() / r) * r,
			round((double)left_back.get_voltage() / r) * r,
			right_front.get_position(),
			left_front.get_position()
		};
		/*vector<double> values = {
			right_front.get_position(),
			right_back.get_position(),
			left_front.get_position(),
			left_back.get_position()
		};*/
		drive();
		record(master, values);

		pros::delay(20);
	}

	pros::lcd::set_text(1, "End of Opcontrol :D");
}