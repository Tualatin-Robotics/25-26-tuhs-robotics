#include "stuff.h"
#include "main.h"
#include "replay.h"

const string REPLAY_FILE = "/usd/a_team_auton_25_26.txt";

pros::Controller playerController(pros::E_CONTROLLER_MASTER);
ReplayController replayController(REPLAY_FILE);


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Better a witty fool than a foolish wit!");
}



void disabled() {}



void competition_initialize() {}



void autonomous() {
	theFile.open(REPLAY_FILE, std::ios_base::in);
	while(true) {
		replayController.updateFrame(false);
		drive(replayController);
		pros::delay(20);
		//pros::delay(20-pros::millis()%20);
	}
	theFile.close();
}



void opcontrol() {
	while (true) {
		pros::delay(20);
	}
}



void drive(auto master){

}