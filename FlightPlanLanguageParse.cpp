#include "pch.h"
#include <cctype>
#include <iostream>
#include "FlightPlanLanguage.h"


// Flight Plan Language class version 1.2

// The FlightPlanLanguageParse subset of the FlightPlanLanguage member functions concentrates
// on parsing a FPL program.


using namespace std;


// The FPL constructor initializes all data members, and selects the command trace
// and target drone options.

FlightPlanLanguage::FlightPlanLanguage(TraceMode trace, DroneMode drone)
{
	num_int_variables     = 0;
	num_labels            = 0;
	num_drone_commands    = 0;
	num_instructions      = 0;
	trace_mode            = trace;
	drone_mode            = drone;
	timer_start           = 0;
	program_counter       = 0;
	compare_returns_equal = false;
	end_program           = false;
	tello_drone           = nullptr;
}


// Accepts a string corresponding to the next line in a FPL program, which can be either
// a label or an instruction.
// White space (space and tab characters) are skipped over.
// Any tokens appearing in the line are extracted and processed.
// At present the FPL specification permits from 1 to 3 tokens per line, and a message is
// generated if this is not the case.

void FlightPlanLanguage::parseLine(string line)
{
	static const int MAX_TOKENS = 3;

	string tokens[MAX_TOKENS];

	int num_tokens = 0;

	// Insert your code here

	addLabelOrInstruction(tokens);
}


// Writes the current contents of the integer variable table to cout.
// Each line consists of the table index starting at 0, the variable's string token, and the
// variable's current value.

void FlightPlanLanguage::displayIntVariables() const
{
	cout << endl << "Integer variable table: [index | variable name | initial value]"
		 << endl << endl;

	for (int i = 0; i < num_int_variables; i++) {
		cout << '\t' << i << '\t' << int_variable_table[i].name << '\t'
			<< int_variable_table[i].value << endl;
	}
}


// Writes the current contents of the label table to cout.
// Each line consists of the table index starting at 0, the label name excluding the ':',
// and the instruction index associated with the label.

void FlightPlanLanguage::displayLabels() const
{
	// Insert your code here
}


// Writes the current contents of the drone command table to cout.
// Each line consists of the table index starting at 0 and the drone command including the
// '<' and '>' characters.

void FlightPlanLanguage::displayDroneCommands() const
{
	// Insert your code here
}


// Writes the current contents of the instruction table to cout.
// Rather than a raw dump of the instruction table fields, the FPL source code is
// reconstructed by looking up token string values from other tables.
// First, any label associated with the instruction is displayed on a line by itself.
// Next, the opcode is displayed.
// Finally either 0, 1 or 2 operands are displayed as string tokens separated by blanks.

void FlightPlanLanguage::displayInstructions() const
{
	cout << endl << "Reconstructed instruction table:" << endl << endl;

	for (int i = 0; i < num_instructions; i++) {
		string label = instructionIndexToLabel(i);
		if (label != "") {
			cout << label << ":" << endl;
		}
		cout << '\t' << indexToInstructionLine(i) << endl;
	}
}


// The addLabelOrInstruction() member function is used during the parsing of FPL programs.
// The argument consists of an array of 1 to 3 string tokens.
// There is no comprehensive syntax error checking, and the array is assumed to contain
// a single label token or else an instruction consisting of an opcode token followed by
// 0, 1 or 2 operand tokens.
// Unused tokens should be empty strings.
// Messages are generated if the opcode is not recognized, or if the instruction table's
// maximum size is exceeded.

void FlightPlanLanguage::addLabelOrInstruction(string tokens[])
{
	if (isLabel(tokens[0])) {
		defineLabel(tokens[0]);
	}
	else {
		if (num_instructions < MAX_INSTRUCTIONS) {
			Opcodes opcode        = stringToOpcode(tokens[0]);
			int operand1          = -1;
			int operand2          = -1;
			bool constant_version = false;
			switch (opcode) {
			case INT_OPCODE:
				operand1 = defineIntVariable(tokens[1], tokens[2]);
				operand2 = stoi(tokens[2]);
				break;
			case ADD_OPCODE:
			case SUB_OPCODE:
			case MUL_OPCODE:
			case DIV_OPCODE:
			case SET_OPCODE:
				operand1 = lookupIntVariable(tokens[1]);
				operand2 = stoi(tokens[2]);
				break;
			case CMP_OPCODE:
				operand1 = lookupIntVariable(tokens[1]);
				if (isIntConstant(tokens[2])) {
					operand2 = stoi(tokens[2]);
					constant_version = true;
				}
				else {
					operand2 = lookupIntVariable(tokens[2]);
					constant_version = false;
				}
				break;
			case BEQ_OPCODE:
			case BNE_OPCODE:
			case BRA_OPCODE:
				operand1 = lookupLabel(tokens[1]);
				break;
			case CMD_OPCODE:
				operand1 = addDroneCommand(tokens[1]);
				break;
			case NOP_OPCODE:
				if (isIntConstant(tokens[1])) {
					operand1 = stoi(tokens[1]);
					constant_version = true;
				}
				else {
					operand1 = lookupIntVariable(tokens[1]);
					constant_version = false;
				}
				break;
			case END_OPCODE:
				break;
			default:
				cout << "Undefined opcode (" << tokens[0] << ") encountered" << endl;
				break;
			}
			instruction_table[num_instructions].opcode           = opcode;
			instruction_table[num_instructions].operand1         = operand1;
			instruction_table[num_instructions].operand2         = operand2;
			instruction_table[num_instructions].constant_version = constant_version;
			num_instructions++;
		}
		else {
			cout << "MAX_INSTRUCTIONS (" << MAX_INSTRUCTIONS << ") exceeded" << endl;
		}
	}
}


// Accepts a string consisting of a drone command (including the '<' and '>' delimiters)
// and adds the command to the end of the drone command table.
// A message is generated and -1 is returned if the drone command table has no available entry.
// Otherwise the index of the table entry used to store the drone command is returned.

int FlightPlanLanguage::addDroneCommand(string token)
{
	// Insert your code here
}


// Returns whether the string token argument represents a variable name.
// Variable name tokens are non-empty, begin with an alphabetic character and are not
// terminated with a ':' character.
// No other error checking is performed.

bool FlightPlanLanguage::isVariable(string token) const
{
	// Insert your code here
}


// Returns whether the string token argument represents an integer constant.
// Integer constants are non-empty and begin with a digit or an optional '+' or '-' sign.
// No other error checking is performed.

bool FlightPlanLanguage::isIntConstant(string token) const
{
	// Insert your code here
}


// Returns whether the string token argument represents a label.
// Labels are non-empty and are terminated with a ':' character.
// No other error checking is performed.

bool FlightPlanLanguage::isLabel(string token) const
{
	bool result = false;

	const int n = token.length();

	if (n > 0) {
		if (token[n - 1] == ':') {
			result = true;
		}
	}

	return result;
}


// Returns whether the string token argument represents an opcode.

bool FlightPlanLanguage::isOpcode(string token) const
{
	bool result = false;

	Opcodes opcode = stringToOpcode(token);

	if (opcode != UNDEFINED_OPCODE) {
		result = true;
	}

	return result;
}


// Returns whether the string token argument represents a drone command.
// Drone commands consist of at least 2 characters where the first character is '<'
// and the last character is '>'.
// No other error checking is performed.

bool FlightPlanLanguage::isDroneCommand(string token) const
{
	// Insert your code here
}


// Returns the index of the string token argument in the integer variable table,
// or -1 if the variable name is not found in the integer variable table.

int FlightPlanLanguage::lookupIntVariable(string token) const
{
	int index = -1;

	for (int i = 0; i < num_int_variables; i++) {
		if (int_variable_table[i].name == token) {
			index = i;
			break;
		}
	}

	return index;
}


// Adds integer variable name and initial value string tokens to the integer variable
// table, and returns the index of the table entry used to store the variable.
// If the variable name is already present in the integer variable table, a message is
// generated and the table entry index is returned.
// If there is no available entry in the integer variable table, a message is generated
// and -1 is returned.

int FlightPlanLanguage::defineIntVariable(string token1, string token2)
{
	int index = lookupIntVariable(token1);

	if (index == -1) {
		if (num_int_variables < MAX_INT_VARIABLES) {
			index = num_int_variables;
			int_variable_table[index].name  = token1;
			int_variable_table[index].value = stoi(token2);
			num_int_variables++;
		}
		else {
			cout << "MAX_INT_VARIABLES (" << MAX_INT_VARIABLES << ") exceeded" << endl;
		}
	}
	else {
		cout << "Variable " << token1 << " is declared more than once" << endl;
	}

	return index;
}


// Returns the index of the string token argument in the label table, or -1 if the
// label is not found in the label table.
// The label should not include a ':' character at the end.

int FlightPlanLanguage::lookupLabel(string token) const
{
	// Insert your code here
}


// Removes the trailing ':' character from the string token, adds the token to the
// label table, and returns the index of the table entry used to store the label.
// If the label is already present in the label table, a message is generated and the
// table entry index is returned.
// If there is no available entry in the label table, a message is generated and -1 is
// returned.

int FlightPlanLanguage::defineLabel(string token)
{
	// Insert your code here
}


// Returns the Opcode enumerated type matching the string token.
// An UNDEFINED_OPCODE value is retuned if the string token is not a valid opcode.

Opcodes FlightPlanLanguage::stringToOpcode(string token) const
{
	Opcodes result = UNDEFINED_OPCODE;

	if (token == "int") {
		result = INT_OPCODE;
	}
	else if (token == "add") {
		result = ADD_OPCODE;
	}
	else if (token == "sub") {
		result = SUB_OPCODE;
	}
	else if (token == "mul") {
		result = MUL_OPCODE;
	}
	else if (token == "div") {
		result = DIV_OPCODE;
	}
	else if (token == "set") {
		result = SET_OPCODE;
	}
	else if (token == "cmp") {
		result = CMP_OPCODE;
	}
	else if (token == "beq") {
		result = BEQ_OPCODE;
	}
	else if (token == "bne") {
		result = BNE_OPCODE;
	}
	else if (token == "bra") {
		result = BRA_OPCODE;
	}
	else if (token == "cmd") {
		result = CMD_OPCODE;
	}
	else if (token == "nop") {
		result = NOP_OPCODE;
	}
	else if (token == "end") {
		result = END_OPCODE;
	}

	return result;
}


// Returns the string matching the Opcode enumerated type argument, including
// "UNDEFINED_OPCODE" if the argument is UNDEFINED_OPCODE.

string FlightPlanLanguage::opcodeToString(Opcodes opcode) const
{
	string result;

	switch (opcode) {
	case INT_OPCODE:
		result = "int";
		break;
	case ADD_OPCODE:
		result = "add";
		break;
	case SUB_OPCODE:
		result = "sub";
		break;
	case MUL_OPCODE:
		result = "mul";
		break;
	case DIV_OPCODE:
		result = "div";
		break;
	case SET_OPCODE:
		result = "set";
		break;
	case CMP_OPCODE:
		result = "cmp";
		break;
	case BEQ_OPCODE:
		result = "beq";
		break;
	case BNE_OPCODE:
		result = "bne";
		break;
	case BRA_OPCODE:
		result = "bra";
		break;
	case CMD_OPCODE:
		result = "cmd";
		break;
	case NOP_OPCODE:
		result = "nop";
		break;
	case END_OPCODE:
		result = "end";
		break;
	default:
		result = "UNDEFINED_OPCODE";
		break;
	}

	return result;
}


// Returns the integer variable name string present in the integer variable table
// entry specified by the index argument.
// An error message string is returned if the index argument is out of bounds.

string FlightPlanLanguage::indexToIntVariableName(int index) const
{
	// Insert your code here
}


// Returns the label string present in the label table entry specified by the index
// argument.
// An error message string is returned if the index argument is out of bounds.

string FlightPlanLanguage::indexToLabel(int index) const
{
	// Insert your code here
}


// Returns the label string, if any, associated with an instruction specified by the
// instruction table index argument.
// If there is no label associated with the specified instruction, an empty string is
// returned.
// A label can optionally precede an instruction.
// The label string will not contain a trailing ':'.

string FlightPlanLanguage::instructionIndexToLabel(int index) const
{
	string result;

	for (int i = 0; i < num_labels; i++) {
		if (label_table[i].value == index) {
			result = label_table[i].name;
			break;
		}
	}

	return result;
}


// Returns the drone command string (including the '<' and '>' delimiters) in the
// drone command table entry specified by the index argument.
// An error message string is returned if the index argument is out of bounds.

string FlightPlanLanguage::indexToDroneCommand(int index) const
{
	// Insert your code here
}


// Reconstruct and return a string corresponding to instruction table entry specified
// by the index argument.
// The result should appear similar to the line that was originally parsed, except for
// possible differences in white space.
// The opcode and any operands will appear as substrings separated by white space.
// An error message string is returned if the index is out of bounds.

string FlightPlanLanguage::indexToInstructionLine(int index) const
{
	string result;

	if ((index >= 0) && (index < num_instructions)) {
		Opcodes opcode       = instruction_table[index].opcode;
		int operand1         = instruction_table[index].operand1;
		int operand2         = instruction_table[index].operand2;
		int constant_version = instruction_table[index].constant_version;
		result = opcodeToString(opcode);
		switch (opcode) {
		case INT_OPCODE:
			result += ' ';
			result += indexToIntVariableName(operand1);
			result += ' ';
			result += to_string(operand2);
			break;
		case ADD_OPCODE:
		case SUB_OPCODE:
		case MUL_OPCODE:
		case DIV_OPCODE:
		case SET_OPCODE:
			result += ' ';
			result += indexToIntVariableName(operand1);
			result += ' ';
			result += to_string(operand2);
			break;
		case CMP_OPCODE:
			result += ' ';
			result += indexToIntVariableName(operand1);
			result += ' ';
			if (constant_version) {
				result += to_string(operand2);
			}
			else {
				result += indexToIntVariableName(operand2);
			}
			break;
		case BEQ_OPCODE:
		case BNE_OPCODE:
		case BRA_OPCODE:
			result += ' ';
			result += indexToLabel(operand1);
			break;
		case CMD_OPCODE:
			result += ' ';
			result += indexToDroneCommand(operand1);
			break;
		case NOP_OPCODE:
			result += ' ';
			if (constant_version) {
				result += to_string(operand1);
			}
			else {
				result += indexToIntVariableName(operand1);
			}
			break;
		case END_OPCODE:
			break;
		default:
			break;
		}    
	}
	else {
		result = "UNDEFINED INSTRUCTION INDEX (" + to_string(index) + ')';
	}

	return result;
}
