#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "globals.c"

#define MEMSIZE 30000

// Virtual Registers
int reg[32];

// File IO
FILE * file[32];
int file_ptr;

// Virtual Memory of 4*150kB
unsigned int mem[MEMSIZE];

// Global pointer
unsigned int GP;

// Frame pointer
unsigned int FP;

// Bump pointer for continous allocation
unsigned int bump_ptr;

// Instruction register
int ir;

// Program counter
unsigned int pc;

unsigned int instruction;
unsigned int op;
int a;
int b;
int c;

// Globals

int isF1(int opcode) {
	return (0 <= opcode) && (opcode < 24);
}

int isF2(int opcode) {
	return (24 <= opcode) && (opcode < 42);
}

int isF3(int opcode) {
	return (42 <= opcode) && (opcode < 64);
}

// Set globals
void init() {

	// TARGET_NOP = 0;

	// // F1 (1-23)
	// TARGET_ADDI = 1;
	// TARGET_SUBI = 2;
	// TARGET_MULI = 3;
	// TARGET_DIVI = 4;
	// TARGET_MODI = 5;
	// TARGET_CMPI = 6;
	// TARGET_LW = 7;
	// TARGET_SW = 8;
	// TARGET_POP = 9;
	// TARGET_PSH = 10;
	// TARGET_BEQ = 11;
	// TARGET_BGE = 12;
	// TARGET_BGT = 13;
	// TARGET_BLE = 14;
	// TARGET_BLT = 15;
	// TARGET_BNE = 16;
	// TARGET_BR = 17;
	// TARGET_BSR = 18;

	// // F2 (23-43)
	// TARGET_ADD = 23;
	// TARGET_SUB = 24;
	// TARGET_MUL = 25;
	// TARGET_DIV = 26;
	// TARGET_MOD = 27;
	// TARGET_CMP = 28;
	// TARGET_RET = 29;
	// TARGET_FLO = 30;
	// TARGET_FLC = 31;
	// TARGET_RDC = 32;
	// TARGET_WRC = 33;

	// // F3 (43-63)
	// TARGET_JSR = 43;
	// TARGET_J = 44;

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
				file_ptr = 1;
				bump_ptr = i;
				GP = bump_ptr;
				reg[27] = 0;
				reg[28] = GP * 4;
				reg[30] = (MEMSIZE-1) * 4;
				break;
			}

			c0 = (char) temp;
			c1 = fgetc(fp);
			c2 = fgetc(fp);
			c3 = fgetc(fp);
		
			mem[i] =  (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
			// printf("%x\n", mem[i]);
			i++;
		}
		printf("Loaded %d bytes", (i*4));
		
		pc = 4;
	}
}

char * getString(int startAddress)
{
	int word;
	char * string;
	int i;

	i = 0;
	string = malloc(sizeof(char) * 1000);
	word = mem[startAddress];
	while(word != 0)
	{
		string[i] = word;
		i++;

		word = mem[startAddress+i];
	}
	string[i] = 0;

	return string;
}

// gets next instruction
int fetch() {

	if((pc/4) >= MEMSIZE)
	{
		printf("\n%d ERROR: PC exceeded memory size\n", pc);
		return 0;
	}

	instruction = mem[pc/4]; //(mem[pc+0] << 24) | (mem[pc+1] << 16) | (mem[pc+2] << 8) | mem[pc+3];

	op = (instruction >> 26) & 63;

	if(pc >= (GP*4))
	{
		printf("\n%d ERROR: PC exceeded code segment\n", pc);
		return 0;
	}

	if(op == TARGET_NOP)
	{
		printf("\n%d ERROR: Hit NOP %d\n", pc, TARGET_NOP);
		return 0;
	}
	

	if(isF1(op) || isF2(op))
	{
		decode();

		// F1 immediate addressing
		if(op == TARGET_ADDI)
		{
			printf("%d ADDI %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] + c;
			pc = pc + 4;
		}
		else if(op == TARGET_SUBI)
		{
			printf("%d SUBI %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] - c;
			pc = pc + 4;
		}
		else if(op == TARGET_MULI)
		{
			printf("%d MULI %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] * c;
			pc = pc + 4;
		}
		else if(op == TARGET_DIVI)
		{
			printf("%d DIVI %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] / c;
			pc = pc + 4;
		}
		else if(op == TARGET_MODI)
		{
			printf("%d MODI %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] % c;
			pc = pc + 4;
		}
		else if(op == TARGET_CMPI)
		{
			printf("%d CMPI %d, %d, %d", pc, a, b, c);
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
			printf("%d POP %d, %d, %d", pc, a, b, c);
			//SP = SP + 1;
			reg[a] = mem[(reg[b])/4];
			reg[b] = reg[b]+c;
			pc = pc + 4;
		}
		else if(op == TARGET_PSH)
		{
			printf("%d PSH %d, %d, %d", pc, a, b, c);
			//SP = SP - 1;
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
			printf("%d BGE %d, %d, %d", pc, a, b, c);
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
			printf("%d BGT %d, %d, %d", pc, a, b, c);
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
			printf("%d BLE %d, %d, %d", pc, a, b, c);
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
			printf("%d BNE %d, %d, %d", pc, a, b, c);
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
			printf("%d BR %d", pc, c);
			pc = pc + c*4;
		}
		else if(op == TARGET_BSR)
		{
			printf("%d BSR %d", pc, c);
			reg[31] = pc + 4;
			pc = pc + c*4;
		}
		else if(op == TARGET_FOPEN)
		{
			printf("%d FOPEN %d, %d, %d", pc, a, b, c);
			char * filename = getString((reg[b] + c) / 4);
			printf("\nOpening file \"%s\" as #%d", filename, file_ptr);
			file[file_ptr] = fopen(filename, "r+");
			reg[a] = file_ptr;
			file_ptr += 1;
			pc = pc + 4;
		}
		else if(op == TARGET_FGETC)
		{
			printf("%d FGETC %d, %d, %d", pc, a, b, c);
			int ch;
			if(reg[b]<1 || reg[b] > file_ptr)
			{
				printf("\nERROR: Bad file access\n");
				return 0;
			}
			ch = fgetc(file[reg[b]]);
			printf("\nCharacter '%c'\n", ch);
			getchar();
			reg[a] = ch;
			pc = pc + 4;
		}
		else if(op == TARGET_FPUTC)
		{
			printf("%d FPUTC %d, %d, %d", pc, a, b, c);
			fputc(reg[c], file[reg[a]]);
			pc = pc + 4;
		}

		// F2 register addressing
		else if(op == TARGET_ADD)
		{
			printf("%d ADD %d, %d, %d", pc, a, b, c);
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
			printf("%d MUL %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] * reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_DIV)
		{
			printf("%d DIV %d, %d, %d", pc, a, b, c);
			reg[a] = reg[b] / reg[c];
			pc = pc + 4;
		}
		else if(op == TARGET_MOD)
		{
			printf("%d MOD %d, %d, %d", pc, a, b, c);
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
			printf("%d RET %d", pc, c);
			pc = reg[c];
		}
		
		else if(op == TARGET_MALLOC)
		{
			printf("%d MALLOC %d, %d, %d", pc, a, b, c);
			// save value of bump pointer
			int s = 4 * bump_ptr;

			// move bump pointer by reg[c]
			bump_ptr = bump_ptr + reg[c]/4;

			// write saved value to reg[a]
			reg[a] = s;

			pc = pc + 4;
		}
		else if(op == TARGET_PRINTF)
		{
			printf("%d PRINTF %d, %d, %d", pc, a, b, c);
			printf("'%s'", getString((reg[b] + c) / 4));
			getchar();
			pc = pc + 4;
		}
		else if(op == TARGET_PRINTFI)
		{
			printf("%d PRINTFI %d, %d, %d", pc, a, b, c);
			printf("\n> '%d'", reg[c]);
			getchar();
			pc = pc + 4;
		}

	}
	else if(isF3(op))
	{
		decodeF3();

		// F3 unconditional jump
		if(op == TARGET_JSR)
		{
			printf("%d JSR %d", pc, c);
			reg[31] = pc + 4;
			pc = c;
		}
		if(op == TARGET_J)
		{
			printf("%d J %d", pc, c);
			pc = c;
		}
		if(op == TARGET_TRAP)
		{
			printf("Trapped");
			return 0;
		}
		else if(op == TARGET_FCLOSE)
		{
			printf("%d FCLOSE %d, %d, %d", pc, a, b, c);
			printf("\nClosing %d", reg[c]);
			fclose(file[reg[c]]);
			pc = pc + 4;
		}
	}

	printf("\t(R1: %2d, R2: %2d, R3: %d, R4: %d, R27 (RR): %d, R28 (GP): %d, R29 (FP): %d, R30 (SP): %d, R31 (LINK): %d)",
		reg[1], reg[2], reg[3], reg[4], reg[27], reg[28], reg[29], reg[30], reg[31]);

	reg[0] = 0; // keep it zero
	reg[28] = GP * 4; // global pointer is constant too

	return 1; // continue
}

int main(int argc,  char ** argv) {
	printf("\nPhoenix: Margit\n===============\n");
	initTokens();
	init();
	
	if(argc == 1)
	{
		load("a.out");
	}
	else
	{
		load(argv[1]);
	}

	getchar();
	while(fetch())
	{
        printf("\n");
        //getchar();
	}
	
    printf("\nExecution stopped.\n");
	return 0;
}