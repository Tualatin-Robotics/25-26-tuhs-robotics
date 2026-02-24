#include "motors.h"
#include "replay.h"
#include "auton-manager.h"

void move(double distance, int32_t rpm = 30) {
	//7.87402 is wheel circimference (A bot standard drivetrain)
	double dt_wheel_cir = 7.87402;
	double revolutions = distance / dt_wheel_cir;
	double gr_b = 48.0 / 36.0;
	gr_b *= 1.36;
	right_front.move_relative(revolutions * 360 * gr_b, rpm);
	left_front.move_relative(revolutions * -360 * gr_b, rpm);
	left_middle.move_relative(revolutions * -360 * gr_b, rpm);
	right_back.move_relative(revolutions * 360 * gr_b, rpm);
	left_back.move_relative(revolutions * -360 * gr_b, rpm);
	right_middle.move_relative(revolutions * 360 * gr_b, rpm);
}

void rotate_right(double revolutions) {
	double gr_b = 48.0 / 36.0;
	double rpm = 30;
	right_front.move_relative(revolutions * -360 * gr_b, rpm);
	left_front.move_relative(revolutions * -360 * gr_b, rpm);
	left_middle.move_relative(revolutions * -360 * gr_b, rpm);
	right_back.move_relative(revolutions * -360 * gr_b, rpm);
	left_back.move_relative(revolutions * -360 * gr_b, rpm);
	right_middle.move_relative(revolutions * -360 * gr_b, rpm);
}

void right_side_auton() {
	move(12.0 * 2.0);
	pros::delay(1000 * 2.5);
	rotate_right(0.22);
	pros::delay(1000 * 0.7);
	bottom.move(LIFT_MAX_SPEED); 
	middle.move(LIFT_MAX_SPEED * 0.5);
	top.move(LIFT_MAX_SPEED * 0.5);
	move(12.0 * 2.0, 20);
	pros::delay(1000 * 4.2);
	move(12.0 * -0.5);
	pros::delay(1000 * 1);
	rotate_right(-0.7);
	pros::delay(1000 * 1);
	move(10.0);
	pros::delay(1000 * 1.5);
	bottom.move(-LIFT_MAX_SPEED);
	deploy.move(LIFT_MAX_SPEED * 0.5);
	top.move(0);
	pros::delay(1000 * 3);
	deploy.move(0);
}

void left_side_auton() {
	move(12.0 * 2.2);
	pros::delay(1000 * 2.8);
	rotate_right(-0.21);
	pros::delay(1000 * 0.7);
	bottom.move(LIFT_MAX_SPEED); 
	middle.move(LIFT_MAX_SPEED * 0.5);
	top.move(LIFT_MAX_SPEED * 0.5);
	move(12.0 * 1.7, 16);
	pros::delay(1000 * 3.3);

	
	deploy.move(LIFT_MAX_SPEED * -0.5);
	pros::delay(1000 * 1.0);
	
	bottom.move(LIFT_MAX_SPEED); 
	middle.move(LIFT_MAX_SPEED * 0.5);
	top.move(LIFT_MAX_SPEED * 0.5);
	
	move(12.0 * -0.3);
	pros::delay(1000 * 1);
	bottom.move(LIFT_MAX_SPEED * 0); 
	middle.move(LIFT_MAX_SPEED * 0);
	top.move(LIFT_MAX_SPEED * 0);
	rotate_right(0.75);
	pros::delay(1000 * 1);
	move(9.8);
	pros::delay(1000 * 1.5);
	bottom.move(LIFT_MAX_SPEED);
	middle.move(LIFT_MAX_SPEED * -0.5);
	top.move(0);
	deploy.move(LIFT_MAX_SPEED * 0.5);
}