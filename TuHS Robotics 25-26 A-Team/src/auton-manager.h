/*

The purpose of the auton manager is to provide functionality to store an autonomous 'mode' or 'flag' that can be changed at runtime*.
This shall allow us to change which auton we want(perhaps to change sides) right before a competition, without needing a computer.


*Not during an actual match (hopefully), but perhaps right before it starts or on the queing tables.

TODO:
    - Create a file with a single number
    - Change the number in that file
    - Clear the file
    - That's really all we need lol
*/

#include "main.h"
#include "tools.h"

string auton_flag_filename = "/usd/26AUTON_FLAG";

enum MODE {
    REPLAY,
    RIGHT_SIDE,
    LEFT_SIDE
};

void set_autonomous_mode(MODE mode) {
    fstream flag_file;
    flag_file.open(auton_flag_filename, std::ios_base::out);
    /*if (mode == REPLAY) {
        //Set flag for replay
    } else if (mode == RIGHT_SIDE) {
        //Set flag for right side auton
    } else if (mode == LEFT_SIDE) {
        //Set flag for left side auton
    } else {
        pros::lcd::set_text(1, "ERROR: NO AUTON MODE RECOGNIZED");
    }*/
   flag_file << (int)mode;
   pros::lcd::set_text(1, "SET AUTON MODE TO:");
    if (mode == REPLAY) {
        pros::lcd::set_text(2, "REPLAY");
    } else if (mode == RIGHT_SIDE) {
        pros::lcd::set_text(2, "RIGHT SIDE");
    } else if (mode == LEFT_SIDE) {
        pros::lcd::set_text(2, "LEFT SIDE");
    } else {
        pros::lcd::set_text(2, "ERROR: AUTON MODE NOT RECOGNIZED");
    }
    flag_file.close();
}

MODE get_autonomous_mode() {
    fstream flag_file;
    flag_file.open(auton_flag_filename, std::ios_base::in);
    int current_mode;
    flag_file >> current_mode;
    MODE mode = (MODE)current_mode;
    pros::lcd::set_text(1, "CURRENT AUTON MODE IS:");
    if (mode == REPLAY) {
        pros::lcd::set_text(2, "REPLAY");
    } else if (mode == RIGHT_SIDE) {
        pros::lcd::set_text(2, "RIGHT SIDE");
    } else if (mode == LEFT_SIDE) {
        pros::lcd::set_text(2, "LEFT SIDE");
    } else {
        pros::lcd::set_text(2, "ERROR: AUTON MODE NOT RECOGNIZED");
    }
    flag_file.close();
    return mode;
}