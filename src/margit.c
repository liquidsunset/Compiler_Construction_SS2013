#include <stdio.h>

#define MEMSIZE 150000

// Virtual Registers
int reg[32];

// Virtual Memory of 150kB
char mem[MEMSIZE];

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
int TARGET_NOP;
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

int isF1(int opcode) {
	return (0 <= opcode) && (opcode < 21);
}

int isF2(int opcode) {
	return (22 <= opcode) && (opcode < 42);
}

int isF3(int opcode) {
	return (42 <= opcode) && (opcode < 64);
}

// Set globals
void init() {

	TARGET_NOP = 0;

	// F1 (1-23)
	TARGET_ADDI = 1;
	TARGET_SUBI = 2;
	TARGET_MULI = 3;
	TARGET_DIVI = 4;
	TARGET_MODI = 5;
	TARGET_CMPI = 6;
	TARGET_LW = 7;
	TARGET_SW = 8;
	TARGET_POP = 9;
	TARGET_PSH = 10;
	TARGET_BEQ = 11;
	TARGET_BGE = 12;
	TARGET_BGT 13;
	TARGET_BLE = 14;
	TARGET_BLT = 15;
	TARGET_BNE = 16;
	TARGET_BR = 17;
	TARGET_BSR = 18;

	// F2 (23-43)
	TARGET_ADD = 23;
	TARGET_SUB = 24;
	TARGET_MUL = 25;
	TARGET_DIV = 26;
	TARGET_MOD = 27;
	TARGET_CMP = 28;
	TARGET_RET = 29;
	TARGET_FLO = 30;
	TARGET_FLC = 31;
	TARGET_RDC = 32;
	TARGET_WRC = 33;

	// F3 (43-63)
	TARGET_JSR = 43;
	
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

// Loads code into memory and initializes registers
void load(char * filename) {
	FILE *fp;
	int temp;
	int i = 0;
	int j;

	fp = fopen(filename, "r");

	if(fp != NULL)
	{
		for(j = 0; j < MEMSIZE; j++)
		{
			mem[j] = TARGET_NOP;
		}

		while((temp = fgetc(fp)) && i < MEMSIZE)
		{
			mem[i] = (char) temp;
			i++;
		}
		
		pc = 0;
	}
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

	if(isF1(op) || isF2(op))
	{
		decode();

		// F1 immediate addressing
		if(op == TARGET_ADDI)
		{
			reg[a] = reg[b] + c;
			pc = pc + 4;
		}

		if(op == TARGET_SUBI)
		{
			reg[a] = reg[b] - c;
			pc = pc + 4;
		}

		if(op == TARGET_MULI)
		{
			reg[a] = reg[b] * c;
			pc = pc + 4;
		}

		if(op == TARGET_DIVI)
		{
			reg[a] = reg[b] / c;
			pc = pc + 4;
		}

		if(op == TARGET_MODI)
		{
			reg[a] = reg[b] % c;
			pc = pc + 4;
		}

		if(op == TARGET_CMPI)
		{
			reg[a] = reg[b] - c;
			pc = pc + 4;
		}

		// F1 memory instructions
		if(op == TARGET_LW)
		{
			reg[a] = mem[(reg[b]+c)/4];
			pc = pc + 4;
		}
		if(op == TARGET_SW)
		{
			mem[(reg[b]+c)/4] = reg[a];
			pc = pc + 4;
		}
		if(op == TARGET_POP)
		{
			reg[a] = mem[(reg[b])/4];
			reg[b] = reg[b]+c;
			pc = pc + 4;
		}
		if(op == TARGET_PSH)
		{
			reg[b] = reg[b]-c;
			mem[(reg[b])/4] = reg[a];
			pc = pc + 4;
		}

		// F1 conditional branching
		if(op == TARGET_BEQ)
		{
			if(reg[a] == 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		if(op == TARGET_BGE)
		{
			if(reg[a] >= 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		if(op == TARGET_BGT)
		{
			if(reg[a] > 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		if(op == TARGET_BLE)
		{
			if(reg[a] <= 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		if(op == TARGET_BLT)
		{
			if(reg[a] < 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		if(op == TARGET_BNE)
		{
			if(reg[a] != 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}

		// F1 unconditional branching
		if(op == TARGET_BR)
		{
			pc = pc + c*4;
		}
		if(op == TARGET_BSR)
		{
			reg[31] = pc + 4;
			pc = pc + c*4;
		}

		// F2 register addressing
		if(op == TARGET_ADD)
		{
			reg[a] = reg[b] + reg[c];
			pc = pc + 4;
		}
		if(op == TARGET_SUB)
		{
			reg[a] = reg[b] - reg[c];
			pc = pc + 4;
		}
		if(op == TARGET_MUL)
		{
			reg[a] = reg[b] * reg[c];
			pc = pc + 4;
		}
		if(op == TARGET_DIV)
		{
			reg[a] = reg[b] / reg[c];
			pc = pc + 4;
		}
		if(op == TARGET_MOD)
		{
			reg[a] = reg[b] % reg[c];
			pc = pc + 4;
		}
		if(op == TARGET_CMP)
		{
			reg[a] = reg[b] - reg[c];
			pc = pc + 4;
		}

		// F2 return from subroutine
		if(op == TARGET_RET)
		{
			pc = reg[c];
		}

		// F2 IO
		if(op == TARGET_FLO)
		{
			// TODO: open file
			pc = pc + 4;
		}
		if(op == TARGET_FLC)
		{
			// TODO: close file
			pc = pc + 4;
		}
		if(op == TARGET_RDC)
		{
			// TODO: read char
			pc = pc + 4;
		}
		if(op == TARGET_WRC)
		{
			// TODO: close file
			pc = pc + 4;
		}

	}


	if(isF3(op))
	{
		decodeF3();

		// F3 unconditional jump
		if(op == TARGET_JSR)
		{
			reg[31] = pc + 4;
			pc = c;
		}
		return;
	}
}



int main() {
	load("test/main.dlx");
	return 0;
}