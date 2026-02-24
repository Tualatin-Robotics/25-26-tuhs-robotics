#include "tools.h"
#include "motors.h"
#include "autons.h"

pros::Controller playerController(pros::E_CONTROLLER_MASTER);

int direction = 1;
bool backwardsPressed = false;

int frame_count = 0;

void initialize()
{
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

void drive(pros::Controller master)
{
	bool l1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
	bool l2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
	bool r1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
	bool r2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

	if (l1)
	{ // All the way up
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(-LIFT_MAX_SPEED * 0.5);
	}
	else if (l2)
	{ // All the way down
		bottom.move(-LIFT_MAX_SPEED);
		middle.move(-LIFT_MAX_SPEED * 0.5);
		top.move(0);
	}
	else if (r2)
	{ // Up and out
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * -0.5);
		top.move(0);
	}
	else if (r1)
	{ // Down and into basket
		bottom.move(LIFT_MAX_SPEED);
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(LIFT_MAX_SPEED * 0.5);
	}
	else
	{
		bottom.move(0);
		middle.move(0);
		top.move(0);
	}

	deploy.move((int)master.get_digital(pros::E_CONTROLLER_DIGITAL_B) * 127 / 2);

	if (backwardsPressed != master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
	{
		backwardsPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
		if (backwardsPressed && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
		{
			direction *= -1;
		}
	}

	int left_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_joystick = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
	float right_speed = 0;
	float left_speed = 0;

	if (direction == 1)
	{
		right_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
		left_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);
	}
	else if (direction == -1)
	{
		right_speed = -left_joystick * (DRIVE_MAX_SPEED / 127.0);
		left_speed = right_joystick * (DRIVE_MAX_SPEED / 127.0);
	}

	right_front.move(right_speed);
	right_middle.move(right_speed);
	right_back.move(right_speed);

	left_front.move(left_speed);
	left_middle.move(left_speed);
	left_back.move(left_speed);

	// Pneumatic code
	bool y_just_pressed = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
	if (y_just_pressed)
	{
		pneumatics_extended = !pneumatics_extended;
		extender.set_value(pneumatics_extended);
	}
}

void drive_replay()
{
	vector<double> values = updateFrame();

	// Indecies 4 and 5 are position data

	right_front.move_voltage(int(values[0]));
	right_back.move_voltage(int(values[1]));
	left_front.move_voltage(int(values[2]));
	left_back.move_voltage(int(values[3]));
	right_middle.move_voltage(int(values[6]));
	left_middle.move_voltage(int(values[7]));
	top.move_voltage(int(values[8]));
	middle.move_voltage(int(values[9]));
	bottom.move_voltage(int(values[10]));
	deploy.move_voltage(int(values[11]));
}

void check_for_auton_change(pros::Controller master) {
	//pros::lcd::set_text(1, asctime(&start);
	//pros::lcd::set_text(2, "AWAITING USER INPUT...");
	//Check if we are under 10 seconds
	if (frame_count > (1000 / 20) * 10) {
		return;
	}
	if (
		master.get_digital(pros::E_CONTROLLER_DIGITAL_A) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_B) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && 
		master.get_digital(pros::E_CONTROLLER_DIGITAL_X) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && 
		master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)
	) {
		pros::lcd::set_text(1, "SETTING AUTON");
		pros::lcd::set_text(2, "AWAITING USER INPUT...");
		//Stop all motors
		right_front.move_voltage(0);
		right_back.move_voltage(0);
		left_front.move_voltage(0);
		left_back.move_voltage(0);
		right_middle.move_voltage(0);
		left_middle.move_voltage(0);
		top.move_voltage(0);
		middle.move_voltage(0);
		bottom.move_voltage(0);
		deploy.move_voltage(0);

		//Enter into a special auton selection mode
		while (true) {
			if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_LEFT)) {
				set_autonomous_mode(LEFT_SIDE);
				return;
			} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
				set_autonomous_mode(RIGHT_SIDE);
				return;
			} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_DOWN)) {
				set_autonomous_mode(REPLAY);
				return;
			}
			frame_count += 1;
			pros::delay(20);
		}
	}
}

void opcontrol()
{
	while (true)
	{
		drive(playerController);

		check_for_auton_change(playerController);

		record(playerController, vector<double>{
			(double)right_front.get_voltage(),
			(double)right_back.get_voltage(),
			(double)left_front.get_voltage(),
			(double)left_back.get_voltage(),
			right_front.get_position(),
			left_front.get_position(),
			(double)right_middle.get_voltage(),
			(double)left_middle.get_voltage(),
			(double)top.get_voltage(),
			(double)middle.get_voltage(),
			(double)bottom.get_voltage(),
			(double)deploy.get_voltage()});

		frame_count += 1;
		pros::delay(20);
	}
	theFile.close();
}

void autonomous()
{
	MODE auton_mode = get_autonomous_mode();
	if (auton_mode == RIGHT_SIDE) {
		right_side_auton();
		return;
	}

	if (auton_mode == LEFT_SIDE) {
		left_side_auton();
		return;
	}

	//Otherwise, do the replay system

	theFile.open(fileName, std::ios_base::in);
	while (true)
	{
		drive_replay();
		pros::delay(20);
	}
	theFile.close();
}