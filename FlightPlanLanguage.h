#include <string>


// Flight Plan Language class version 1.2

// This class will serve as the basis for subsequent project installments including
// individual contributions from team members.

// When a compiler parses a language, it builds a variety of data structures such as tables.
// The FPL has the following types of tokens: opcodes, integer variables, integer constants,
// labels, and drone commands.
// There is a separate table maintained for integer variables, labels, and drone commands.
// There is also a table of instructions, where each instruction entry is defined by a line
// containing an opcode and from 0 to 2 operands.
// Opcodes are represented by an enumerated type rather than a table.
// A table is not needed for integer constants because the integer value is simply stored
// directly in the instruction table's operand1 or operand2 fields.
// These tables are used during FPL execution.


// The opcodes form an "enumerated type" that provides a symbolic name for each opcode.
// This is better than assigning a meaningless integer code (such as 2) to each opcode.
// The opcode names were changed from version 1.1 to avoid a clash with a Windows INT typedef.

enum Opcodes { UNDEFINED_OPCODE, INT_OPCODE, ADD_OPCODE, SUB_OPCODE, MUL_OPCODE,
	           DIV_OPCODE, SET_OPCODE, CMP_OPCODE, BEQ_OPCODE, BNE_OPCODE,
	           BRA_OPCODE, CMD_OPCODE, NOP_OPCODE, END_OPCODE };


// Select the degree to which FPL instructions are printed out during FPL execution.

enum TraceMode { TRACE_OFF, TRACE_CMD_NOP_MODE, TRACE_ALL_OPCODES_MODE };


// Select which drone to control during FPL execution.
// A target drone does not have to be selected.
// Targeting a virtual (AirSim) drone is currently not supported.

enum DroneMode { NO_DRONE_MODE, VIRTUAL_DRONE_MODE, TELLO_DRONE_MODE };


// An integer variable table entry has two fields: the variable name string (token) and its
// current integer value that can change during FPL execution.

struct IntVariableEntry {
	std::string name;
	int         value;
};


// A label table entry has two fields: the label string (without the ':') and the fixed index
// of the instruction line that the label precedes. 

struct LabelEntry {
	std::string name;
	int         value;
};


// An instruction consists of an opcode field, an optional first operand, and an optional second
// operand.
// An example of an instruction with zero operands is "end".
// An example of an instruction with one operand is "cmd".
// An example of an instruction with two operands is "cmp".
// Some instructions such as "nop" can have either an integer variable or constant operand, and
// the constant_version field will be set to true if the operand is a constant.

struct InstructionEntry {
	Opcodes opcode;
	int     operand1;
	int     operand2;
	bool    constant_version;
};


class Tello;	// Forward declaration to avoid the need for including TelloApi.h


// The FlightPlanLanguage class encapsulates all member functions and data structures needed
// to parse and execute FPL programs, and interface to a virtual or physical drone.
// Version 1.1 supports only parsing FPL source lines and displaying the tables created.
// Version 1.2 adds support for FPL execution and controlling a Tello drone.
// See FlightPlanLanguage.cpp for a description of the member functions.

class FlightPlanLanguage
{
public:		// member functions intended to be used by clients of the class

	FlightPlanLanguage(TraceMode trace, DroneMode drone);		// constructor
	~FlightPlanLanguage();										// destructor

	void parseLine(std::string line);

	void displayIntVariables() const;
	void displayLabels() const;
	void displayDroneCommands() const;
	void displayInstructions() const;

	void executeProgram();

private:	// member functions not intended to be used by clients of the class

	void addLabelOrInstruction(std::string tokens[]);
	int  addDroneCommand(std::string token);

	bool isVariable(std::string token) const;
	bool isIntConstant(std::string token) const;
	bool isLabel(std::string token) const;
	bool isOpcode(std::string token) const;
	bool isDroneCommand(std::string token) const;

	int lookupIntVariable(std::string token) const;
	int defineIntVariable(std::string token1, std::string token2);

	int lookupLabel(std::string token) const;
	int defineLabel(std::string token);

	Opcodes     stringToOpcode(std::string token) const;
	std::string opcodeToString(Opcodes opcode) const;

	std::string indexToIntVariableName(int index) const;
	std::string indexToLabel(int index) const;
	std::string instructionIndexToLabel(int index) const;
	std::string indexToDroneCommand(int index) const;
	std::string indexToInstructionLine(int index) const;

	void executeNextInstruction();

	void executeIntInstruction();
	void executeAddInstruction();
	void executeSubInstruction();
	void executeMulInstruction();
	void executeDivInstruction();
	void executeSetInstruction();
	void executeCmpInstruction();
	void executeBeqInstruction();
	void executeBneInstruction();
	void executeBraInstruction();
	void executeCmdInstruction();
	void executeNopInstruction();
	void executeEndInstruction();

	void executeTelloCommand(int index);

private:	// data members should always have private scope

	static const int MAX_INT_VARIABLES  = 1000;		// maximum number of integer variables allowed
	static const int MAX_LABELS         = 1000;		// maximum number of labels allowed
	static const int MAX_DRONE_COMMANDS = 1000;		// maximum number of drone commands allowed
	static const int MAX_INSTRUCTIONS   = 2000;		// maximum number of FPL instructions allowed

	IntVariableEntry int_variable_table[MAX_INT_VARIABLES];		// integer variables
	LabelEntry       label_table[MAX_LABELS];					// labels excluding ':'
	std::string      drone_command_table[MAX_DRONE_COMMANDS];	// drone commands including '<' and '>'
	InstructionEntry instruction_table[MAX_INSTRUCTIONS];		// instructions
	
	int num_int_variables;			// current number of integer variables defined
	int num_labels;					// current number of labels defined
	int num_drone_commands;			// current number of drone commands used
	int num_instructions;			// current number of instructions used

	TraceMode trace_mode;			// indicates the level of instruction tracing desired
	DroneMode drone_mode;			// indicates what drone to control, if any

	int timer_start;				// clock() value when the drone is initialized
	int program_counter;			// index of the next instruction to execute

	bool compare_returns_equal;		// result of a CMP instruction
	bool end_program;				// an END instruction or error was encountered

	Tello * tello_drone;			// pointer to a dynamically instantiated Tello object
};
