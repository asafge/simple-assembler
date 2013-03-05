/*
 * The instlineMem files (.h,c) are responsible for saving instruction lines to memory.
 */
#ifndef INSTLINEMEM_H
	#define INSTLINEMEM_H

	#include "../config.h"
	#include "../instructionHandler.h"

	/* --- Declaration of functions --- */
	void RestartInstlines();
	int StoreInstline(inst* ins, char *argument[], int model[], int registerNum[]);
	int CreateAddLine(inst* ins, char op1Name[MAX_LINE], int op1AddType, int registerNum1, char op2Name[MAX_LINE], int op2AddType, int registerNum2);
	int GetInstAdditionalLength(int addressType);

	/* --- Declaration of data types --- */
	typedef struct								// Operand structure
	{
		char name[MAX_LINE];
		short reg; 								// Do we have a register used in this operand?
		int addrType;							// Addressing Type
	} operand;

	typedef struct								// Instruction-line structure
	{
		inst* ins;
		operand op1 ;
		operand op2;
	} instLine;

	int IC;										// Instruction counter
	instLine instlineArray[MAX_ARRAY_LENGTH];	// Array containing opcode instructions ("moc") or the "entry" instruction
	int LIC;									// A counter for the instlineArray

#endif
