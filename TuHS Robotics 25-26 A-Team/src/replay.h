
#include <vector>
fstream theFile;

string fileName = "/usd/test_motor_replay.txt";
bool recordButtonPressed = false;
bool isRecording = false;

int frame = 0;


void logMotorStates(vector<long> values){
    for(int i = 0; i < values.size(); i++){
        theFile << values[i] << " , ";
    }
    theFile <<"\n";
}



void record(pros::Controller master, vector<long> values){
    recordButtonPressed = (
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    );

    if(recordButtonPressed != (
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    )){
        recordButtonPressed = (
            master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
            master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
            master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
            master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
        );
    
		if(recordButtonPressed){
			isRecording = !isRecording;
		}
        if(isRecording){
            theFile.open(fileName, std::ios_base::out);
        }
	}

    if(isRecording){
        logMotorStates(values);
    }
}

// added startReplay, I have no idea if this works or if it solves the problem
void startReplay(){
    theFile.open(fileName, std::ios_base::in);
}

vector<long> updateFrame(){
    frame++;
    vector<long> values;
    char comma;
    for(int i = 0; i < 16; i++){
        theFile >> values[i];
        theFile >> comma;
    }
    return values;
}