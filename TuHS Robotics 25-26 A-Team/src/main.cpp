#include "stuff.h"
#include "main.h"

//string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
//ReplayController replayController(REPLAY_FILE);

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

void reset_drivetrain_pos() {
	right_front.tare_position();
	right_back.tare_position();
	left_front.tare_position();
	left_back.tare_position();
	right_middle.tare_position();
	left_middle.tare_position();
	bottom.tare_position();
	top.tare_position();
	middle.tare_position();
}

#include "replay.h"

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

void drive_replay(){
	vector<double> values = updateFrame();

	right_front.move_voltage(int(values[0]));
	right_back.move_voltage(int(values[1]));
	left_front.move_voltage(int(values[2]));
	left_back.move_voltage(int(values[3]));
	right_middle.move_voltage(int(values[6]));
	left_middle.move_voltage(int(values[7]));
	top.move_voltage(int(values[8]));
	middle.move_voltage(int(values[9]));
	bottom.move_voltage(int(values[10]));
}

void opcontrol() {
	while (true) {
		drive(playerController);
		
		record(playerController, vector<double>{
			(double)right_front.get_voltage(),
			(double)right_back.get_voltage(),
			(double)left_front.get_voltage(),
			(double)left_back.get_voltage(),
			(double)right_middle.get_voltage(),
			(double)left_middle.get_voltage(),
			(double)top.get_voltage(),
			(double)middle.get_voltage(),
			(double)bottom.get_voltage(),
			right_front.get_position(),
			left_front.get_position()
		});
		pros::delay(20);
		//pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}

void autonomous() {
	theFile.open(fileName, std::ios_base::in);
	while(true) {
		updateFrame();
		drive_replay();
		pros::delay(20);
		//pros::delay(20 - pros::millis() % 20);
	}
	theFile.close();
}