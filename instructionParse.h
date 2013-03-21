/*
 * The instructionParse files (.h,c) are responsible for the more lower-level
 * handling of the instructions, making sure they don't contain errors etc.
 */
#ifndef INSTRUCTIONPARSE
	#define INSTRUCTIONPARSE

	#include "instructionHandler.h"
	#include "mem/instlineMem.h"

	/* --- Declaration of functions --- */
	int HandleDataInstruction (char* data);
	int HandleStringInstruction(char* string);
	int HandleExternInstruction(char* label);
	int HandleEntryInstruction(char* data);
	int HandleOpInstruction(char* line, inst* ins);

	operand* GetOperand(char* line, inst* ins, int isSrc);
	int GetOpComb(char* data);
	int GetOpType(char* data);
	int GetOpRegister(char* operand, int addrType);
	char* GetOpIndex(char* data);

	int GetAddressingType(char* operand);
	int IsAllowedAddressing(inst* ins , int opType , int srcOrDest);

#endif

