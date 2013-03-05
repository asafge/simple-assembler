/*
 * The objFile files (.h,c) are responsible for creating the .ob file.
 */

#ifndef OBJFILE_H
	#define OBJFILE_H

	#include <stdio.h>
	#include "../instructionHandler.h"
	#include "../mem/symbolMem.h"
	#include "../mem/instlineMem.h"

	void WriteOpenningLine (FILE* objFile);

	void AppendSymbol (FILE* objFile, Symbol* sym, unsigned int fileAddr);
	void AddExternalSymbol (Symbol *sym, unsigned int refAddr);

	void WriteDataToFile (FILE* objFile, int startAddr);
	int WriteInstruction (FILE* objFile, instLine fins, int startAddr);
	int AppendOperandWords (FILE* objFile, operand op, int startAddr);

#endif
