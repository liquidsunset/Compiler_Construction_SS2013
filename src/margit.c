
// Virtual Registers
int reg[32];

// Virtual Memory of 150kB
int mem[150000];

// Instruction register
int ir;

// Program counter
int pc;


// Loads code into memory and initializes registers
void load() {
	// TODO: read all content of file

	// TODO: store in mem

	// TODO: set pc to first mem address
}


// gets next instruction
void fetch() {
	// TODO: get instruction from current pc

	// TODO: shift to get op

	// TODO: decide if F1 or F2 or F3

	// TODO: shift to get a

	// TODO: shift to get b

	// TODO: shift to get c
}

// executes last fetched instruction
void execute() {
	// TODO: switch of instruction type

	// TODO: F1

	// TODO: F2

	// TODO: F3
}