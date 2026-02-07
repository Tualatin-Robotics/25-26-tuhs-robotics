#include "stuff.h"
#include "main.h"

// string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
// ReplayController replayController(REPLAY_FILE);

// Lift motors
float LIFT_MAX_SPEED = 127;
pros::Motor bottom(1);	// Full motor
pros::Motor middle(11); // Half motor
pros::Motor top(20);	// Half motor
pros::Motor deploy(6);	// Half motor
int DRIVE_MAX_SPEED = 100;

// MOTOR DEFINITIONS
pros::Motor right_front(8);
pros::Motor right_middle(9);
pros::Motor right_back(10);
pros::Motor left_front(3);
pros::Motor left_middle(4);
pros::Motor left_back(5);

// Pneumatics
pros::ADIDigitalOut extender('A');

int direction = 1;
bool backwardsPressed = false;
bool pneumatics_extended = false;

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

#include "replay.h"

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

void opcontrol()
{
	while (true)
	{
		drive(playerController);

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
		pros::delay(20);
		// pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}

void move(double distance, int32_t rpm = 30) {
	double revolutions = distance / 7.87402;
	double gr_b = 48.0 / 36.0;
	gr_b = 36.0 / 48.0;
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

void autonomous()
{
//TESTING AUTON
	reset_drivetrain_pos();
	int32_t rpm = 10;
	double diameter_b = 4;
	double diameter_s = 2.75;
	double circ_b = 12.56637;
	double drivetrain_gr = 48.0 / 36.0;
	double dt_wheel_cir = 7.87402;
	double gr_b = drivetrain_gr;
	//float gr_s 32:1 and 48:1
	/*right_front.move_absolute(360 * gr_b, rpm);
	left_front.move_absolute(-360 * gr_b, rpm);
	left_middle.move_absolute(-360 * gr_b, rpm);
	right_back.move_absolute(360 * gr_b, rpm);
	left_back.move_absolute(-360 * gr_b, rpm);
	right_middle.move_absolute(360 * gr_b, rpm);*/
	move(12.0 * 2.0);
	pros::delay(1000 * 2.5);
	rotate_right(0.22);
	pros::delay(1000);
	 bottom.move(LIFT_MAX_SPEED); 
		middle.move(LIFT_MAX_SPEED * 0.5);
		top.move(LIFT_MAX_SPEED * 0.5);
	move(12.0 * 2.0, 20);
	pros::delay(1000 * 4.5);
	move(12.0 * -0.5);
	pros::delay(1000 * 1);
	bottom.move(LIFT_MAX_SPEED * 0); 
		middle.move(LIFT_MAX_SPEED * 0);
		top.move(LIFT_MAX_SPEED * 0);
	rotate_right(-0.7);
		pros::delay(1000 * 1);
	move(10.0);
			pros::delay(1000 * 1.5);
bottom.move(-LIFT_MAX_SPEED);
		deploy.move(LIFT_MAX_SPEED * 0.5);
		top.move(0);
	/*right_front.move_absolute(0 * gr_b, rpm);
	left_front.move_absolute(0 * gr_b, rpm);
	right_back.move_absolute(0 * gr_b, rpm);
	left_back.move_absolute(0 * gr_b, rpm);
	right_middle.move_absolute(0 * gr_b, rpm);
	left_middle.move_absolute(0 * gr_b, rpm);*/

// 	bottom.move(-LIFT_MAX_SPEED);
// 		middle.move(-LIFT_MAX_SPEED * 0.5);
// 		top.move(0);

	return;
//END OF TESTING AUTON
	theFile.open(fileName, std::ios_base::in);
	while (true)
	{
		drive_replay();
		pros::delay(20);
		// pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}