/*
* This is the source code for Tualatin High School's Replay System!
* Written by @HackerMagnificent and @quasark-main

* This file contains the ReplayController class: a fake controller
* that emulates input values that have been previously recorded as 
* VEX V5 input.

* ReplayController has methods:
* - updateFrame, this has to be called at the end of the main loop
* to update the current frame the controller is emulating.
* - record, call this method when you want to start recording your
* controller's input. It will continue recording until you press the
* four directional buttons.
* - get_digital and get_analog emulate their respective pros::Controller
* methods.


* TODO: make logInputs private method of ReplayController
* TODO: change record method to execute without checking for button combination
* TODO: finish this description

*/

fstream theFile;

void logInputs(pros::Controller controller, fstream &theFile){
	theFile <<
		controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) << " , " <<
		controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) << " , " <<
		controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) << " , " <<
		controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_B) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y) << " , " <<
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) << " ,\n";
}

class ReplayController{
	public:
	int buttons[16];
	int frame = 0;
	bool donePlaying = false;
	string fileName;

	bool is_recording_auton = false;
	
	int32_t get_analog(int stick){
		switch(stick){
			case pros::E_CONTROLLER_ANALOG_LEFT_X: return buttons[0];
			case pros::E_CONTROLLER_ANALOG_LEFT_Y: return buttons[1];
			case pros::E_CONTROLLER_ANALOG_RIGHT_X: return buttons[2];
			case pros::E_CONTROLLER_ANALOG_RIGHT_Y: return buttons[3];
			default: return 0;
		}
	}

	int32_t get_digital(int button){
		switch(button){
			case pros::E_CONTROLLER_DIGITAL_L1: return buttons[4];
			case pros::E_CONTROLLER_DIGITAL_L2: return buttons[5];
			case pros::E_CONTROLLER_DIGITAL_R1: return buttons[6];
			case pros::E_CONTROLLER_DIGITAL_R2: return buttons[7];
			case pros::E_CONTROLLER_DIGITAL_RIGHT: return buttons[8];
			case pros::E_CONTROLLER_DIGITAL_DOWN: return buttons[9];
			case pros::E_CONTROLLER_DIGITAL_UP: return buttons[10];
			case pros::E_CONTROLLER_DIGITAL_LEFT: return buttons[11];
			case pros::E_CONTROLLER_DIGITAL_X: return buttons[12];
			case pros::E_CONTROLLER_DIGITAL_B: return buttons[13];
			case pros::E_CONTROLLER_DIGITAL_Y: return buttons[14];
			case pros::E_CONTROLLER_DIGITAL_A: return buttons[15];
			default: return 0;
		}
	}

	void updateFrame(bool optimized = false){
		frame++;
		char comma;
		for(int i = 0; i < 16; i++){
			theFile >> buttons[i];
			theFile >> comma;
		}
		//recursively calls updateFrame() until it finds a line that isn't all 0's
		if(!donePlaying && buttons[0]+buttons[1]+buttons[2]+buttons[3]+buttons[4]+buttons[5]+buttons[6]+buttons[7]+buttons[8]+buttons[9]+buttons[10]+buttons[11]+buttons[12]+buttons[13]+buttons[14]+buttons[15] < 1024){
			if(optimized && buttons[0] == 0 && buttons[1]==0 && buttons[2]==0 && buttons[3]==0 && buttons[4]+buttons[5]+buttons[6]+buttons[7]+buttons[8]+buttons[9]+buttons[10]+buttons[11]+buttons[12]+buttons[13]+buttons[14]+buttons[15] == 0){
				updateFrame(true);
			}
		}
		else{
			donePlaying=false;
			for(int i = 0; i < 16; i++) buttons[i] = 0;
		}
		
	}

	void record(pros::Controller controller, fstream &theFile){
		if(is_recording_auton){
			logInputs(controller, theFile);
			if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
				theFile << 1024;//1024 is greater than the highest possible value of the sum of all the inputs
				theFile.close();
				cout << "Recording Stopped!";
				is_recording_auton = false;
				initialize();
				pros::delay(1000);
			}
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)
			&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
			if(!is_recording_auton){
				pros::delay(1000);
				theFile.open(fileName, std::ios_base::out);
				cout << "Recording!";
				is_recording_auton = true;
				autonomous();
			}
		}
	}

	ReplayController(string a){
		fileName = a;
	}
};