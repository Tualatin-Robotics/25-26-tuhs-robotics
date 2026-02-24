#ifndef MOTORS_H
#define MOTORS_H
#include "tools.h"

// MOTOR DEFINITIONS
pros::Motor right_front(8);
pros::Motor right_middle(9);
pros::Motor right_back(10);
pros::Motor left_front(3);
pros::Motor left_middle(4);
pros::Motor left_back(5);

// Lift motors
pros::Motor bottom(1);	// Full motor
pros::Motor middle(11); // Half motor
pros::Motor top(20);	// Half motor
pros::Motor deploy(6);	// Half motor

float LIFT_MAX_SPEED = 127;

int DRIVE_MAX_SPEED = 100;

// Pneumatics
pros::ADIDigitalOut extender('A');

bool pneumatics_extended = false;

void reset_drivetrain_pos()
{
	right_front.tare_position();
	right_back.tare_position();
	left_front.tare_position();
	left_back.tare_position();
	right_middle.tare_position();
	left_middle.tare_position();
	bottom.tare_position();
	top.tare_position();
	middle.tare_position();
	deploy.tare_position();
}

#endif