
#include <vector>
fstream theFile;

string fileName = "/usd/TEST_01.txt";
bool recordButtonPressed = false;
bool isRecording = false;

int frame = 0;


void logMotorStates(vector<long> values){

    for(int i = 0; i < values.size(); i++){
        theFile << values[i] << " , ";
    }
    theFile << "\n";
}



void record(pros::Controller master, vector<long> values){
    // recordButtonPressed = (
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    // );

    // if(recordButtonPressed != (
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    // )){
    //     recordButtonPressed = (
    //         master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //         master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //         master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //         master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    //     );
    
	// 	if(recordButtonPressed){
	// 		isRecording = !isRecording;
	// 	}
    //     if(isRecording){
    //         theFile.open(fileName, std::ios_base::out);
    //     }
	// }

    if(true){
        logMotorStates(values);
    }
}

void openFile(){
    theFile.open(fileName, std::ios::out);
}

void closeFile() {
    theFile.close();
}

vector<long> updateFrame(){
    frame++;
    vector<long> values;
    char comma;
    for(int i = 0; i < 4; i++){
        theFile >> values[i];
        theFile >> comma;
    }
    return values;
}