
#include <vector>
fstream theFile;

string fileName = "/usd/TEST_02.txt";
bool recordButtonPressed = false;
bool isRecording = false;

int frame = 0;


void logMotorStates(vector<int> values){

    for(int i = 0; i < values.size(); i++){
        theFile << values[i] << " , ";
    }
    theFile << "\n";
}



void record(pros::Controller master, vector<int> values){
    // recordButtonPressed = (
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
    //     master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)
    // );fl, fr, bl, br}

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
    //         master.get_digital(profl, fr, bl, br}s::E_CONTROLLER_DIGITAL_DOWN)
    //     );fl, fr, blfl, fr, bl, br}, br}
    
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

vector<int> updateFrame() {
    frame++;
    int fl;
    int fr;
    int bl;
    int br;
    char comma;
    theFile >> fr;
    theFile >> comma;
    theFile >> br;
    theFile >> comma;
    theFile >> fl;
    theFile >> comma;
    theFile >> bl;
    theFile >> comma;
    return vector<int>{fr, br, fl, bl};
}