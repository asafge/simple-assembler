/*
 * The instructionHandler files (.h,c) are responsible for declaring and defining data types and
 * static data objects for the use of the instructionParse files. Also, they handle instruction check.
 */
#ifndef INSTRUCTIONHANDLER
	#define INSTRUCTIONHANDLER

	#include <stdlib.h>
	#include "config.h"

	/* --- Declaration of data types and enums --- */
	typedef struct {																			// An instruction structure:
		char name[5];																				// Largest command is "entry" + 1 for '/0'
		int opCode;																					// A number representation of opcode (0-15)
		int sourceAllowedAddr[6];																	// Arrays representing allowed addressing ...
		int destAllowedAddr[6];																		// ... methods, based on opAddrType enum.
		int numOfOp;																				// Number of allowed operands for this command
	} inst;
	typedef struct {																			// A register structure
		char* name;
		int code;
	} reg;

	enum opAddrType {IMMEDIATE, LABEL, INDEX_LABLE, INDEX_DIRECT, INDEX_REGISTER, DIRECT_REG};
	enum opSrcOrDest {SOURCE,DEST};

	/* --- Declaration of functions --- */
	int IsSymbol (char *line);
	int IsStringOrData(char *line , int symbolIndex);
	int IsEntryOrExtern(char *line, int symbolIndex);
	int IsOpInstruction(char *line , int symbolIndex);

	inst* GetInstByName(char* name);
	reg* GetRegisterByName(char* name);

#endif
