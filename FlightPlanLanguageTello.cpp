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
				// Tilden Fernandez -- Individual code contribution

				//Create strings to hold each substring number, the index of the command
				//and the actual values in the move command
				string outString[3];
				int location[3];
				int final_move_command[3];

				//Extract the variables from the move command
				outString[0] = command.substr(7, 1);
				outString[1] = command.substr(10, 1);
				outString[2] = command.substr(13, 1);

				//Use the lookupInVariable method to find the index of each command
				location[0] = lookupIntVariable(outString[0]);
				location[1] = lookupIntVariable(outString[1]);
				location[2] = lookupIntVariable(outString[2]);

				//Find the values stored at the variable location
				final_move_command[0] = int_variable_table[location[0]].value;
				final_move_command[1] = int_variable_table[location[1]].value;
				final_move_command[2] = int_variable_table[location[2]].value;

				//Creat a coordinates object with the values extracted
				Coordinates moveCoords = { final_move_command[0], final_move_command[1], final_move_command[2] };

				//Tell the drone to move
				tello_drone->move(moveCoords);

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
