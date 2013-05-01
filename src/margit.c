#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MEMSIZE 1001

// Virtual Registers
int reg[32];

// Virtual Memory of 4*150kB
unsigned int mem[MEMSIZE];

// Instruction register
int ir;

// Program counter
unsigned int pc;

unsigned int instruction;
unsigned int op;
unsigned int a;
unsigned int b;
unsigned int c;

// Globals
unsigned int TARGET_NOP;
unsigned int TARGET_ADD;
unsigned int TARGET_ADDI;
unsigned int TARGET_AND;
unsigned int TARGET_ANDI;
unsigned int TARGET_BEQZ;
unsigned int TARGET_BNEZ;
unsigned int TARGET_DIV;
unsigned int TARGET_DIVI;
unsigned int TARGET_J;
unsigned int TARGET_JAL;
unsigned int TARGET_JALR;
unsigned int TARGET_JR;
unsigned int TARGET_LHI;
unsigned int TARGET_LW;
unsigned int TARGET_MUL;
unsigned int TARGET_MULI;
unsigned int TARGET_OR;
unsigned int TARGET_ORI;
unsigned int TARGET_SEQ;
unsigned int TARGET_SEQI;
unsigned int TARGET_SLE;
unsigned int TARGET_SLEI;
unsigned int TARGET_SLL;
unsigned int TARGER_SLLI;
unsigned int TARGET_SLT;
unsigned int TARGET_SLTI;
unsigned int TARGET_SNE;
unsigned int TARGET_SNEI;
unsigned int TARGET_SRA;
unsigned int TARGET_SRAI;
unsigned int TARGET_SRL;
unsigned int TARGET_SRLI;
unsigned int TARGET_SUB;
unsigned int TARGET_SUBI;
unsigned int TARGET_SW;
unsigned int TARGET_XOR;
unsigned int TARGET_XORI;
unsigned int TARGET_MODI;
unsigned int TARGET_CMPI;
unsigned int TARGET_MOD;
unsigned int TARGET_CMP;
unsigned int TARGET_POP;
unsigned int TARGET_PSH;
unsigned int TARGET_BEQ;
unsigned int TARGET_BGE;
unsigned int TARGET_BGT;
unsigned int TARGET_BLE;
unsigned int TARGET_BLT;
unsigned int TARGET_BNE;
unsigned int TARGET_BR;
unsigned int TARGET_BSR;
unsigned int TARGET_RET;
unsigned int TARGET_JSR;
unsigned int TARGET_FLO;
unsigned int TARGET_FLC;
unsigned int TARGET_RDC;
unsigned int TARGET_WRC;
unsigned int TARGET_J;

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
	TARGET_BGT = 13;
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
	TARGET_J = 44;
	
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
	int i = 0;
	int j;

	fp = fopen(filename, "r");

	if(fp != NULL)
	{
		for(j = 0; j < MEMSIZE; j++)
		{
			mem[j] = TARGET_NOP;
		}

		while(i < MEMSIZE)
		{
			unsigned int temp;
			unsigned char c0, c1, c2, c3;
			temp = fgetc(fp);
			if(temp == EOF)
			{
				break;
			}

			c0 = (char) temp;
			c1 = fgetc(fp);
			c2 = fgetc(fp);
			c3 = fgetc(fp);
		
			mem[i] =  (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
			i++;
		}
		
		pc = 0;
	}
}


// gets next instruction
int fetch() {

	if(pc >= MEMSIZE)
	{
		return 0;
	}

	instruction = mem[pc/4]; //(mem[pc+0] << 24) | (mem[pc+1] << 16) | (mem[pc+2] << 8) | mem[pc+3];

	op = (instruction >> 26) & 63;

	if(op == TARGET_NOP)
	{
		return 0;
	}

	if(isF1(op) || isF2(op))
	{
		decode();

		// F1 immediate addressing
		if(op == TARGET_ADDI)
		{
			reg[a] = reg[b] + c;
			pc = pc + 4;
		}
		else if(op == TARGET_SUBI)
		{
			reg[a] = reg[b] - c;
			pc = pc + 4;
		}
		else if(op == TARGET_MULI)
		{
			reg[a] = reg[b] * c;
			pc = pc + 4;
		}
		else if(op == TARGET_DIVI)
		{
			reg[a] = reg[b] / c;
			pc = pc + 4;
		}
		else if(op == TARGET_MODI)
		{
			reg[a] = reg[b] % c;
			pc = pc + 4;
		}
		else if(op == TARGET_CMPI)
		{
			reg[a] = reg[b] - c;
			pc = pc + 4;
		}
		// F1 memory instructions
		else if(op == TARGET_LW)
		{
			printf("%d LW %d, %d, %d", pc, a, b, c);
			reg[a] = mem[(reg[b]+c)/4];
			pc = pc + 4;
		}
		else if(op == TARGET_SW)
		{
			printf("%d SW %d, %d, %d", pc, a, b, c);
			mem[(reg[b]+c)/4] = reg[a];
			pc = pc + 4;
		}
		else if(op == TARGET_POP)
		{
			reg[a] = mem[(reg[b])/4];
			reg[b] = reg[b]+c;
			pc = pc + 4;
		}
		else if(op == TARGET_PSH)
		{
			reg[b] = reg[b]-c;
			mem[(reg[b])/4] = reg[a];
			pc = pc + 4;
		}
		// F1 conditional branching
		else if(op == TARGET_BEQ)
		{
			printf("%d BEQ %d, %d, %d", pc, a, b, c);
			if(reg[a] == 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		else if(op == TARGET_BGE)
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
		else if(op == TARGET_BGT)
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
		else if(op == TARGET_BLE)
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
		else if(op == TARGET_BLT)
		{
			printf("%d BLT %d, %d, %d", pc, a, b, c);
			if(reg[a] < 0)
			{
				pc = pc + c * 4;
			}
			else
			{
				pc = pc + 4;
			}
		}
		else if(op == TARGET_BNE)
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
		else if(op == TARGET_BR)
		{
			pc = pc + c*4;
		}
		else if(op == TARGET_BSR)
		{
			reg[31] = pc + 4;
			pc = pc + c*4;
		}

		// F2 register addressing
		else if(op == TARGET_ADD)
		{
			reg[a] = reg[b] + reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_SUB)
		{
			printf("%d SUB %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] - reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_MUL)
		{
			reg[a] = reg[b] * reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_DIV)
		{
			reg[a] = reg[b] / reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_MOD)
		{
			reg[a] = reg[b] % reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_CMP)
		{
			printf("%d CMP %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] - reg[c];
			pc = pc + 4;
		}

		// F2 return from subroutine
		else if(op == TARGET_RET)
		{
			pc = reg[c];
		}

		// F2 IO
		else if(op == TARGET_FLO)
		{
			int mode = O_RDONLY;
			if(mem[reg[b]] == 'w')
			{
				mode = O_WRONLY;
			}
			reg[c] = open((char *) mem[reg[a]], mode);

			pc = pc + 4;
		}
		else if(op == TARGET_FLC)
		{
			close(reg[c]);
			pc = pc + 4;
		}
		else if(op == TARGET_RDC)
		{
			char buf;
			read(reg[a], &buf, 1);
			reg[c] = buf;
			pc = pc + 4;
		}
		else if(op == TARGET_WRC)
		{
			const char buf = (char)reg[c];
			write(reg[a], &buf, 1);
			pc = pc + 4;
		}

	}
	else if(isF3(op))
	{
		decodeF3();

		// F3 unconditional jump
		if(op == TARGET_JSR)
		{
			reg[31] = pc + 4;
			pc = c;
		}
		if(op == TARGET_J)
		{
			printf("%d J %d", pc, c);
			pc = c;
		}
	}

	printf("(R1: %d, R2: %d, R3: %d)\n", reg[1], reg[2], reg[3]);
	reg[0] = 0; // keep it zero
	return 1; // continue
}

int main() {
	init();
	load("test/gcd.bin");
    //load("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/gcd.bin");
	while(fetch());
    printf("%d",mem[15]);
	return 0;
}