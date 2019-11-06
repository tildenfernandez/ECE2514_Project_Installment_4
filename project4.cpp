#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include "FlightPlanLanguage.h"


// Provides an example of using version 1.2 of the FlightPlanLanguage class, which
// supports FPL parsing, optional execution tracing, and optional drone communication.
// The program prompts for the name of a FPL file, parses the file, displays the
// parse tables generated, and possibly executes the FPL program according to the
// modes selected in the FPL class constructor.


using namespace std;


int main()
{
	FlightPlanLanguage flight_plan(TRACE_ALL_OPCODES_MODE, TELLO_DRONE_MODE);

	string file_name;

	cout << "Enter FPL file name: ";

	cin >> file_name;

	ifstream fppl_file(file_name);

	if (fppl_file.is_open()) {
		string line;
		while (getline(fppl_file, line)) {
			flight_plan.parseLine(line);
		}
		flight_plan.displayIntVariables();
		flight_plan.displayLabels();
		flight_plan.displayDroneCommands();
		flight_plan.displayInstructions();
		flight_plan.executeProgram();
	}
	else {
		cout << "File " << file_name << " not found" << endl;
	}

	return 0;
}
