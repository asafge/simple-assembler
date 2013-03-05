/*
 * The fileHelper files (.h,c) include helper types and functions.
 * Note: Assuming here that unsigned short has the length of 16 bits.
 */
#ifndef FILEHELPER_H
	#define FILEHELPER_H

	#include <stdio.h>
	#include "../config.h"
	#include "../mem/symbolMem.h"

	/* --- Declaration of functions --- */
	FILE* CreateTypedFile (char fileName[MAX_FILE_NAME], char type[MAX_FILE_NAME]);
	void WriteInBase4 (FILE* file, int num, int wantedLength);

	/* --- Declaration of data types --- */
	typedef struct									// 16 bit instruction
	{
		unsigned short destRegister:3;
		unsigned short destOperandType:3;
		unsigned short sourceRegister:3;
		unsigned short sourceOperandType:3;
		unsigned short instructionCode:4;
	} instructionWord;
	typedef struct									// External symbol
	{
		Symbol* sym;								// Symbol
		unsigned int refAddress;					// Symbol's address
	} externalSymbol;

#endif
