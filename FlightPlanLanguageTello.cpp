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
				string xString;
				string yString;
				string zString;
				int control = 0;

				//Extract each number command from the string
				for (int i = 6; i < command.length(); i++) {
					switch (control) {
					case 1:
						if (command.substr(i, 1) = " ") {
							control++;
						}
						else {
							xString.append(command.substr(i, 1));
						}
						break;
					case 2:
						if (command.substr(i, 1) = " ") {
							control++;
						}
						else {
							yString.append(command.substr(i, 1));
						}
						break;
					case 3:
						if (command.substr(i, 1) = ">") {
							control++;
						}
						else {
							zString.append(command.substr(i, 1));
						}
						break;
					default:
						i = command.length();
					}
				}

				//Convert the strings to integers
				int xMove = std::stoi(xString);
				int yMove = std::stoi(yString);
				int zMove = std::stoi(zString);

				//Tell the drone to move
				tello_drone->move({ xMove, yMove, zMove });

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
