#include "pch.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#include "FlightPlanLanguage.h"


// Flight Plan Language class version 1.2

// The FlightPlanLanguageExecute subset of the FlightPlanLanguage member functions concentrates
// on executing a FPL program.


using namespace std;


// Execute the FPL program beginnig at index 0 of the instruction_table.
// Execution continues until an "end" instruction is executed or an invalid opcode is encountered.

void FlightPlanLanguage::executeProgram()
{
	cout << endl << "Program execution: [instruction index | operation]"
		 << endl << endl;

	timer_start           = clock();
	program_counter       = 0;
	compare_returns_equal = false;
	end_program           = false;

	while (!end_program) {
		executeNextInstruction();
	}
}


// Execute the instruction appearing at instruction_table[program_counter],
// and update the program_counter.

void FlightPlanLanguage::executeNextInstruction()
{
	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		cout << '\t' << program_counter << "\t";
	}

	Opcodes opcode = instruction_table[program_counter].opcode;

	switch (opcode) {
	case INT_OPCODE:
		executeIntInstruction();
		break;
	case ADD_OPCODE:
		executeAddInstruction();
		break;
	case SUB_OPCODE:
		executeSubInstruction();
		break;
	case MUL_OPCODE:
		executeMulInstruction();
		break;
	case DIV_OPCODE:
		executeDivInstruction();
		break;
	case SET_OPCODE:
		executeSetInstruction();
		break;
	case CMP_OPCODE:
		executeCmpInstruction();
		break;
	case BEQ_OPCODE:
		executeBeqInstruction();
		break;
	case BNE_OPCODE:
		executeBneInstruction();
		break;
	case BRA_OPCODE:
		executeBraInstruction();
		break;
	case CMD_OPCODE:
		executeCmdInstruction();
		break;
	case NOP_OPCODE:
		executeNopInstruction();
		break;
	case END_OPCODE:
		executeEndInstruction();
		break;
	default:
		cout << "Undefined instruction opcode (" << opcode << ") at location "
			 << program_counter << " - program terminated" << endl;
		end_program = true;
		break;
	}
}


// Initialize an integer variable.

void FlightPlanLanguage::executeIntInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;
	int operand2 = instruction_table[program_counter].operand2;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		cout << indexToIntVariableName(operand1) << " = " << operand2 << endl;
	}

	int_variable_table[operand1].value = operand2;

	program_counter++;
}


// Add a constant to an integer variable.

void FlightPlanLanguage::executeAddInstruction()
{
	// Insert your code here
}


// Subtract a constant from an integer variable.

void FlightPlanLanguage::executeSubInstruction()
{
	// Insert your code here
}


// Multiply an integer variable by a constant.

void FlightPlanLanguage::executeMulInstruction()
{
	// Insert your code here
}


// Divide an integer variable by a constant.
// Attempting to divide by zero causes program termination.

void FlightPlanLanguage::executeDivInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;
	int operand2 = instruction_table[program_counter].operand2;

	if (operand2 == 0) {
		cout << "Attempted division by zero at location " << program_counter
			 << " - program terminated" << endl;
		end_program = true;
	}
	else {
		if (trace_mode == TRACE_ALL_OPCODES_MODE) {
			int old_value = int_variable_table[operand1].value;
			cout << indexToIntVariableName(operand1) << " = " << old_value << " / "
				<< operand2 << " = " << (old_value / operand2) << endl;
		}
		int_variable_table[operand1].value /= operand2;
		program_counter++;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Set an integer variable to a constant.///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FlightPlanLanguage::executeSetInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;
	int operand2 = instruction_table[program_counter].operand2;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		int old_value = int_variable_table[operand1].value;
		cout << indexToIntVariableName(operand1) << " = " << operand2 << endl;
	}
	int_variable_table[operand1].value = operand2;
	program_counter++;
}


// Compare two integer variables, or compare an integer variable to a constant.

void FlightPlanLanguage::executeCmpInstruction()
{
	int  operand1         = instruction_table[program_counter].operand1;
	int  operand2         = instruction_table[program_counter].operand2;
	bool constant_version = instruction_table[program_counter].constant_version;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		int right_operand = (constant_version ? operand2 : (int_variable_table[operand2].value));
		cout << int_variable_table[operand1].value << " == " << right_operand << " ?" << endl;
	}

	if (constant_version) {
		compare_returns_equal = (int_variable_table[operand1].value == operand2);
	}
	else {
		compare_returns_equal = (int_variable_table[operand1].value ==
			                     int_variable_table[operand2].value);
	}

	program_counter++;
}


// Branch to a label if the previous compare instruction operands were equal.

void FlightPlanLanguage::executeBeqInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		if (compare_returns_equal) {
			cout << "BEQ taken to label " << indexToLabel(operand1) << endl;
		}
		else {
			cout << "BEQ skipped" << endl;
		}
	}

	if (compare_returns_equal) {
		program_counter = label_table[operand1].value;
	}
	else {
		program_counter++;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Branch to a label if the previous compare instruction operands were unequal./////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FlightPlanLanguage::executeBneInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		if (compare_returns_equal) {
			cout << "BNE skipped" << endl; 
		}
		else {
			cout << "BNE taken to label " << indexToLabel(operand1) << endl;
		}
	}

	if (compare_returns_equal) {
		program_counter++;
	}
	else {
		program_counter = label_table[operand1].value;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Unconditionally branch to a label.///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FlightPlanLanguage::executeBraInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;

	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		cout << "BRA taken to label " << indexToLabel(operand1) << endl;
	}

	program_counter = label_table[operand1].value;
}


// Execute a drone command.

void FlightPlanLanguage::executeCmdInstruction()
{
	int operand1 = instruction_table[program_counter].operand1;

	if ((trace_mode == TRACE_ALL_OPCODES_MODE) || (trace_mode == TRACE_CMD_NOP_MODE)) {
		cout << "CMD " << indexToDroneCommand(operand1) << endl;
	}

	if (drone_mode == TELLO_DRONE_MODE) {
		executeTelloCommand(operand1);
	}

	program_counter++;
}


// The main() application thread (rather than the drone) will suspend until the number
// of seconds contained in an integer variable or constant has elapsed.
// Time is relative to the start of the FPL program execution.
// Let n = the number of seconds.
// This behavior is not the same as a delay of n seconds.
// For example, if the current time is 5 seconds and n = 7, the application thread will
// resume in 2 seconds.
// The application thread does not suspend if the current time is greater than n.

void FlightPlanLanguage::executeNopInstruction()
{
	int  operand1         = instruction_table[program_counter].operand1;
	int  operand2         = instruction_table[program_counter].operand2;
	bool constant_version = instruction_table[program_counter].constant_version;

	int wait_until_time;

	if (constant_version) {
		wait_until_time = operand1;
	}
	else {
		wait_until_time = int_variable_table[operand1].value;
	}

	if ((trace_mode == TRACE_ALL_OPCODES_MODE) || (trace_mode == TRACE_CMD_NOP_MODE)) {
		cout << "Wait until " << wait_until_time << " seconds since initialization" << endl;
	}

	int pause = (wait_until_time * CLOCKS_PER_SEC) - clock() + timer_start;

	if (pause > 0) {
		Sleep(pause);
	}

	program_counter++;
}


// Terminate program execution.

void FlightPlanLanguage::executeEndInstruction()
{
	if (trace_mode == TRACE_ALL_OPCODES_MODE) {
		cout << "END" << endl;
	}

	end_program = true;
}
