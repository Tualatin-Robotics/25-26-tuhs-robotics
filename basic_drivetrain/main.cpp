#include "main.h"

pros::Motor left(2);
pros::Motor right(1);

float speed_reduction = 0.4;
float turning_speed = 0.3;

void initialize() {
	
}

void autonomous() {

}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {
		int left_side = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int right_side = left_side;

		//if right stick x pos is larger than 0, then reduce right side speed
		//and increase left side speed

		//if left stick x pos smaller than 0, increase right side and reduce left side

		right_side -= (master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * turning_speed);
		left_side += (master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * turning_speed);

		left.move(-left_side * (1.0 - speed_reduction));
		right.move(right_side * (1.0 - speed_reduction));



		pros::delay(20);

	}
}
