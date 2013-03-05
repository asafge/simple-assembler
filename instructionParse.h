/*
 * The instructionParse files (.h,c) are responsible for the more lower-level
 * handling of the instructions, making sure they don't contain errors etc.
 */
#ifndef INSTRUCTIONPARSE
	#define INSTRUCTIONPARSE

	#include "instructionHandler.h"

	/* --- Declaration of functions --- */
	int HandleDataInstruction (char* data);
	int HandleStringInstruction(char* string);
	int HandleExternInstruction(char* label);
	int HandleEntryInstruction(char* data);

	inst* GetInstByName(char* name);
	reg* GetRegisterByName(char* name);

	int GetAddressingType(char* operand);
	int IsAllowedAddressing(inst* ins , int operandType , int addressingType);

#endif

