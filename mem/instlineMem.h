/*
 * The instlineMem files (.h,c) are responsible for saving instruction lines to memory.
 */
#ifndef INSTLINEMEM_H
	#define INSTLINEMEM_H

	#include "../config.h"
	#include "../instructionHandler.h"

	/* --- Declaration of data types --- */
	typedef struct								// Operand structure
	{
		char name[MAX_LINE];
		short reg; 								// Register number for this operand if any
		int addrType;							// Addressing Type
	} operand;

	typedef struct								// Instruction-line structure
	{
		inst* ins;
		operand op1;
		operand op2;
		int comb;
		int type;
	} instLine;

	int IC;										// Instruction counter
	instLine instlineArray[MAX_ARRAY_LENGTH];	// Array containing opcode instructions ("mov") or the "entry" instruction
	int LIC;									// A counter for the instlineArray

	/* --- Declaration of functions --- */
	void RestartInstlines();
	int StoreInstline(inst* ins, operand* ops[MAX_ARGUMENTS], int comb, int type);
	int GetInstAdditionalLength(int addressType);

#endif
