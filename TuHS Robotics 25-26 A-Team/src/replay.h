
#include <vector>
fstream theFile;

string fileName = "/usd/AUTON25-26.txt";
bool recordButtonPressed = false;
bool recordButtonLastState = false;
bool isRecording = false;

int frame = 0;


void logMotorStates(vector<double> values){

    for(int i = 0; i < values.size(); i++){
        theFile << values[i] << " , ";
    }
    theFile << "\n";
}



void record(pros::Controller master, vector<double> values){
    recordButtonPressed = (
        master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) &&
        master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)
    );

    if (recordButtonPressed == false && recordButtonLastState == true) {
        if (isRecording) {
            isRecording = false;
            theFile.close();
            pros::lcd::set_text(1, "Recording stopped.");
        } else {
            isRecording = true;
            theFile.open(fileName, std::ios::out);
            pros::lcd::set_text(1, "Recording started!");
        }
    }

    recordButtonLastState = recordButtonPressed;

    if (isRecording) {
        logMotorStates(values);
    }
}

vector<double> updateFrame() {
    frame++;
    double fl;
    double fr;
    double bl;
    double br;
    char comma;
    theFile >> fr;
    theFile >> comma;
    theFile >> br;
    theFile >> comma;
    theFile >> fl;
    theFile >> comma;
    theFile >> bl;
    theFile >> comma;
    return vector<double>{fr, br, fl, bl};
}