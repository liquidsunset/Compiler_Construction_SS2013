
// Virtual Registers
int reg[32];

// Virtual Memory of 150kB
char mem[150000];

// Instruction register
int ir;

// Program counter
int pc;

int instruction;
int op;
int a;
int b;
int c;

// Globals

int TARGET_ADD;
int TARGET_ADDI;
int TARGET_AND;
int TARGET_ANDI;
int TARGET_BEQZ;
int TARGET_BNEZ;
int TARGET_DIV;
int TARGET_DIVI;
int TARGET_J;
int TARGET_JAL;
int TARGET_JALR;
int TARGET_JR;
int TARGET_LHI;
int TARGET_LW;
int TARGET_MUL;
int TARGET_MULI;
int TARGET_OR;
int TARGET_ORI;
int TARGET_SEQ;
int TARGET_SEQI;
int TARGET_SLE;
int TARGET_SLEI;
int TARGET_SLL;
int TARGER_SLLI;
int TARGET_SLT;
int TARGET_SLTI;
int TARGET_SNE;
int TARGET_SNEI;
int TARGET_SRA;
int TARGET_SRAI;
int TARGET_SRL;
int TARGET_SRLI;
int TARGET_SUB;
int TARGET_SUBI;
int TARGET_SW;
int TARGET_XOR;
int TARGET_XORI;
int TARGET_MODI;
int TARGET_CMPI;
int TARGET_MOD;
int TARGET_CMP;
int TARGET_POP;
int TARGET_PSH;
int TARGET_BEQ;
int TARGET_BGE;
int TARGET_BGT;
int TARGET_BLE;
int TARGET_BLT;
int TARGET_BNE;
int TARGET_BR;
int TARGET_BSR;
int TARGET_RET;
int TARGET_JSR;
int TARGET_FLO;
int TARGET_FLC;
int TARGET_RDC;
int TARGET_WRC;

// Set globals
void init() {

	// F1
	TARGET_ADDI;
	TARGET_SUBI;
	TARGET_MULI;
	TARGET_DIVI;
	TARGET_MODI;
	TARGET_CMPI;
	TARGET_LW;
	TARGET_SW;
	TARGET_POP;
	TARGET_PSH;
	TARGET_BEQ;
	TARGET_BGE;
	TARGET_BGT;
	TARGET_BLE;
	TARGET_BLT;
	TARGET_BNE;
	TARGET_BR;
	TARGET_BSR;

	// F2
	TARGET_ADD;
	TARGET_SUB;
	TARGET_MUL;
	TARGET_DIV;
	TARGET_MOD;
	TARGET_CMP;
	TARGET_RET;
	TARGET_FLO;
	TARGET_FLC;
	TARGET_RDC;
	TARGET_WRC;

	// F3
	TARGET_JSR;
	
	// TARGET_ANDI = 3;
	// TARGET_ORI = 13;
	// TARGET_SEQI = 15;
	// TARGET_SLEI = 17;
	// TARGET_XORI = 31;
	// TARGET_AND = 2;
	// TARGET_BEQZ = 4;
	// TARGET_BNEZ = 5;
	// TARGET_J = 6;
	// TARGET_JAL = 7;
	// TARGET_JALR = 8;
	// TARGET_JR = 9;
	// TARGET_LHI = 10;
	// TARGET_OR = 12;
	// TARGET_SEQ = 14;
	// TARGET_SLE = 16;
	// TARGET_SLL = 18;
	// TARGER_SLLI;
	// TARGET_SLT = 19;
	// TARGET_SLTI = 20;
	// TARGET_SNE = 21;
	// TARGET_SNEI = 22;
	// TARGET_SRA = 23;
	// TARGET_SRAI = 24;
	// TARGET_SRL = 25;
	// TARGET_SRLI = 26;
	// TARGET_SW = 29;
	// TARGET_XOR = 30;
	
}

// Loads code into memory and initializes registers
void load() {
	// TODO: read all content of file

	// TODO: store in mem

	// TODO: set pc to first mem address
}


// gets next instruction
void fetch() {
	

	instruction = mem[pc+0];
	instruction = instruction << 8;
	instruction = instruction | mem[pc+1];
	instruction = instruction << 8;
	instruction = instruction | mem[pc+2];
	instruction = instruction << 8;
	instruction = instruction | mem[pc+3];

	op = (instruction >> 26) & 63;

	// TODO: decide if F1 or F2 or F3
	//  and decode accordingly


}

void decode() {
	a = (instruction >> 21) & 31; // 0x1F: 5 lsbs
	b = (instruction >> 16) & 31; // 0x1F: 5 lsbs
	c = instruction & 65535; // 0xFFFF: 16 lsbs
	if (c >= 32768) {
		c = c - 65536; // 0x10000: 2^16
	}
}

void decodeF3() {
	c = instruction & 67108863; // 0x3FFFFFF: 26 lsbs
}

// executes last fetched instruction
void execute() {
	// TODO: switch of instruction type

	// TODO: F1

	// TODO: F2

	// TODO: F3
}

int main() {
	return 0;
}