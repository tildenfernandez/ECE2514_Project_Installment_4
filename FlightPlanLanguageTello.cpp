#include "pch.h"
#include <iostream>
#include "FlightPlanLanguage.h"
#include "TelloApi.h"
#include <string>

// Flight Plan Language class version 1.2

// The FlightPlanLanguageTello subset of the FlightPlanLanguage member functions concentrates
// on communicating with the Tello drone.


using namespace std;


// The FPL destructor deallocates the Tello object if it was created.

FlightPlanLanguage::~FlightPlanLanguage()
{
	if (tello_drone != nullptr) {
		delete tello_drone;
		tello_drone = nullptr;
	}
}


// Execute the Tello command appearing at the specified index in the drone_command_table.
// A Tello object will be created when an "<initialize>" drone command occurs.

void FlightPlanLanguage::executeTelloCommand(int index)
{
	string command = indexToDroneCommand(index);

	if (command == "<initialize>") {
		if (tello_drone == nullptr) {
			tello_drone = new Tello();
		}
		if (!(tello_drone->canInitialize())) {
			cout << "Tello initialization failed" << endl;
			delete tello_drone;
			tello_drone = nullptr;
		}
	}
	else {
		if (tello_drone == nullptr) {
			cout << "Tello not initialized before " << command << endl;
		}
		else {
			if (command.substr(0, 6) == "<move ") {
				// Insert your code here

				//Create strings to hold each substring number
				string outString[3];
				int index1;
				int index2 = 0;
				int location[3];
				int final_move_command[3];

				//Extract each number command from the string
				for (int i = 0; i < 0; i++) {
					index1 = command.substr(index2).find_first_of("%");
					index2 = line.substr(index1).find_first_of(" ")-1;
					outstring[i] = command.substr(index1, index2);
				}

				location[0] = lookupIntVariable(outstring[0]);
				location[1] = lookupIntVariable(outstring[1]);
				location[2] = lookupIntVariable(outstring[2]);

				final_move_command[0] = int_variable_table[location[0]].value;
				final_move_command[1] = int_variable_table[location[1]].value;
				final_move_command[2] = int_variable_table[location[2]].value;

				//Tell the drone to move
				tello_drone->move({ final_move_command[0], final_move_command[1], final_move_command[2] });

			}

			else if (command == "<arm>") {
				tello_drone->arm();
			}
			else if (command == "<takeoff>") {
				tello_drone->takeoff();
			}
			else if (command == "<land>") {
				tello_drone->land();
			}
			else {
				cout << "Unrecognized Tello \"" << command << "\" command" << endl;
			}
		}
	}
}
