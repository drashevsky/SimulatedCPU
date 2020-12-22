/**
  * C4C1 CPU Emulator
  * Daniel Rashevsky (2017)
  * Simulates the C4C1 CPU's state and executes a program from its virtual ROM.
  */

#include <stdio.h>

unsigned int ROM[256] = {0x3FC02000, 	//Contents of ROM - 30 bits used
			 0x3FA00020, 
			 0x3FE00000, 
			 0x3EC0A000, 
			 0x3EA02000, 
			 0x3E000000};	
unsigned int RAM[256];			//Contents of RAM - 30 bits used
unsigned char REG[16];			//Registers AX - PX - 8 bits used
unsigned char INST = 0;			//Instruction pointer - 8 bits used
unsigned int currInst; 			//Current instruction loaded
unsigned int state = 1;			//State of the computer, on or off
unsigned char OPERANDA;  		//Operand A
unsigned char OPERANDB;  		//Operand B
unsigned char getData = '\0'; 		//Character received from input

void main() {

	//Walking through each instruction and looping on overflow
	while (INST <= 256 && state == 1) {
	
		//printf("%d\n", INST); 	//Debug
		currInst = ROM[INST];		//Load instruction from ROM and increment instruction pointer
		++INST;
		
		if ((0x3E000000 & currInst) == 0x3E000000) {			//If this is an instruction, identify instruction and execute it
			OPERANDA = (currInst << 11) >> 24; 			//Get operand A value - from INST OPERANDA, OPERANDB
			OPERANDB = (currInst << 19) >> 24;			//Get operand B value
			//printf("Cell: %d Inst: %x Operand A: %x Operand B: %x\n", INST - 1, ((currInst << 7) >> 28), OPERANDA, OPERANDB); //Debug

			switch ((currInst << 7) >> 28) {
				case 0:			//HALT
					state = 0;
					break;
				case 1:			//MOV
					REG[OPERANDA] = REG[OPERANDB];
					break;
				case 2:			//STORE
					RAM[OPERANDA] = REG[OPERANDB];
					break;
				case 3:			//LOADRAM
					REG[OPERANDA] = RAM[OPERANDB];
					break;
				case 4:			//LOADROM
					REG[OPERANDA] = ROM[OPERANDB];
					break;
				case 5:			//JMP
					INST = OPERANDA;
					break;
				case 6:			//JZ
					if (REG[OPERANDB] == 0) {
						INST = OPERANDA;
					}
					break;
				case 7:			//JNZ
					if (REG[OPERANDB] != 0) {
						INST = OPERANDA;
					}
					break;
				case 8:			//OR
					REG[OPERANDA] |= REG[OPERANDB];
					break;
				case 9:			//AND
					REG[OPERANDA] &= REG[OPERANDB];
					break;
				case 10:			//NOT
					~REG[OPERANDA]; 
					break;
				case 11:			//SHL
					REG[OPERANDA] << 1;
					break;
				case 12:			//SHR
					REG[OPERANDA] >> 1;
					break;
				case 13:			//ADD
					REG[OPERANDA] += REG[OPERANDB];
					break;
				case 14:			//RECV
					while (!(getData >= 'a' && getData <= 'f') && !(getData >= 'A' && getData <= 'F') && !(getData >= '0' && getData <= '9')) {
						getData = getchar();	
					}
					if (getData >= 'a' && getData <= 'f') {
						getData -= ('a' - 0xA);	
					}
					if (getData >= 'A' && getData <= 'F') {
						getData -= ('A' - 0xA);	
					}
					if (getData >= '0' && getData <= '9') {
						getData -= '0';
					}
					REG[OPERANDA] = getData;
					getData = '\0';
					rewind(stdin);
					break;
				case 15:			//SEND
					printf("%x\n", REG[OPERANDA]);
					break;
			}
		} 
	}
}
