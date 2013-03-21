/*
 * The objFile files (.h,c) are responsible for creating the .ob file.
 */
#include <stdlib.h>
#include "objFile.h"
#include "fileHelper.h"
#include "extFile.h"
#include "../globalHelper.h"
#include "../config.h"
#include "../instructionHandler.h"
#include "../mem/instlineMem.h"
#include "../mem/datastringMem.h"

extern int datastringArray[MAX_ARRAY_LENGTH];
extern int DC;
extern int IC;

// Writes the opening line for the file - instruction count and data count
void WriteOpenningLine(FILE* objFile)
{
	WriteInBase4(objFile, IC, 0);		// Code length
	fputc(' ', objFile);
	WriteInBase4(objFile, DC, 0);		// Data length
	fputc('\n', objFile);
}

// Converts an elaborate addressing type to the 0-3 basic options
int ConvertAddrType(int addrType)
{
	if ((addrType == IMMEDIATE) || (addrType == LABEL))
		return addrType;
	else if ((addrType == INDEX_LABLE) || (addrType == INDEX_DIRECT) || (addrType == INDEX_REGISTER))
		return 2;
	else if (addrType == DIRECT_REG)
		return 3;
	return -1;
}

// Writes an instruction to the .ob file.
// Returns the number of words added to the file, or 0 on error.
int WriteInstruction (FILE* objFile, instLine line, int startAddr)
{
	int length = 1;
	WriteInBase4(objFile, startAddr, 4);			// Append the starting address.
	fputc('\t', objFile);

	instructionWord iw;								// Create the instruction word object
	iw.comb = line.comb;
	iw.instructionCode = line.ins->opCode;
	iw.sourceOperandType = ConvertAddrType(line.op1.addrType);
	iw.sourceRegister = line.op1.reg;
	iw.destOperandType = ConvertAddrType(line.op2.addrType);
	iw.destRegister = line.op2.reg;
	iw.type = line.type;

	// Cast this instructionWord into unsigned short and append it to the file in base 4
	WriteInBase4(objFile, *((unsigned short*)&iw), 10);
	fputs("\tA\n", objFile);

	// Additional words - Extract and append words from index-typed operands
	if (line.ins->numOfOp > 0)
	{
		if ((line.ins->numOfOp == 2) && (iw.sourceOperandType != 3))				// Src - not always available
		{
			startAddr++;
			int wordsAdded = AppendOperandWords(objFile, line.op1, startAddr);
			if (wordsAdded == 0) return 0;
			length += wordsAdded;
		}
		if (iw.destOperandType != 3)												// Dest - should always be available
		{
			startAddr++;
			int wordsAdded = AppendOperandWords(objFile, line.op2, startAddr);
			if (wordsAdded == 0) return 0;
			length += wordsAdded;
		}
	}
	return length;
}

// This function appends an operand into the ob file.
// Should only get an operand whose type is not DIRECT_REG (contains label/immediate)
// Returns the number of words appended, 0 means error.
int AppendOperandWords(FILE* objFile, operand op, int startAddr)
{
	int addr = startAddr;
	char *label1, *label2;
	Symbol *sym1, *sym2;

	WriteInBase4(objFile, startAddr, 4);				// Write starting address of the first symbol
	fputc('\t', objFile);

	switch (op.addrType)
	{
	case IMMEDIATE:										// Example: #3
		label1 = strtokEx(op.name, "#", 0);
		if (label1 == NULL) return 0;

		WriteInBase4(objFile, atoi(label1), 10);
		fputs("\tA\n", objFile);
		free(label1);
		return 1;

	case LABEL:
		sym1 = GetSymbol(op.name, -1);					// Example: MAIN
		if (sym1 == NULL)
		{
			fprintf(stderr, "Could find symbol: %s\n", op.name);
			return 0;
		}
		AppendSymbol(objFile, sym1, addr);
		return 1;

	case INDEX_DIRECT:
		label1 = strtokEx(op.name, "{}", 0);			// Example: label1{5}
		label2 = strtokEx(op.name, "{}", 1);
		sym1 = GetSymbol(label1, -1);

		if (sym1 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label1); free(label1); free(label2); return 0;
		}

		if (!IsNumber(label2)) {
			fprintf(stderr, "Index is not valid: %s\n", label2); free(label1); free(label2); return 0;
		}

		AppendSymbol(objFile, sym1, addr);				// Append first symbol
		addr = ++startAddr;

		WriteInBase4(objFile, addr, 4);					// Write address
		fputc('\t', objFile);
		WriteInBase4(objFile, atoi(label2), 10);		// Append second symbol (integer index)
		fputs("\tA\n", objFile);

		free(label1);
		free(label2);
		return 2;

	case INDEX_LABLE:
		label1 = strtokEx(op.name, "{}", 0); 			// Example: label1{label2}
		label2 = strtokEx(op.name, "{}", 1);
		sym1 = GetSymbol(label1, -1), sym2 = GetSymbol(label2, -1);

		if (sym1 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label1); free(label1); free(label2); return 0;
		}
		if (sym2 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label2); free(label1); free(label2); return 0;
		}

		AppendSymbol(objFile, sym1, addr);				// Append first symbol
		addr = ++startAddr;

		WriteInBase4(objFile, addr, 4);					// Write address
		fputc('\t', objFile);
		AppendSymbol(objFile, sym2, addr);				// Append second symbol (index)

		free(label1);
		free(label2);
		return 2;
	case INDEX_REGISTER:
		label1 = strtokEx(op.name, "{}", 0);			// Example: label1{r3}
		sym1 = GetSymbol(label1, -1);

		if (sym1 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label1);  free(label1); return 0;
		}

		AppendSymbol(objFile, sym1, addr);

		free(label1);
		return 1;
	default:
		return 0;
	}
}

// This function appends a symbol into the ob file.
// Also, it will notice external symbols and add them for later.
void AppendSymbol (FILE* objFile, Symbol* sym, unsigned int fileAddr)
{
	if (sym->type == DATA)								// Add the IC to it's address
		WriteInBase4(objFile, IC + sym->address, 10);
	else
		WriteInBase4(objFile, sym->address, 10);

	if (sym->isExtern)									// An external symbol
	{
		fputs("\tE\n", objFile);
		AddExternalSymbol(sym, fileAddr);
	}
	else
		fputs("\tR\n", objFile);
}

// This function appends the datastringArray into the .ob file (at the end)
void WriteDataToFile (FILE* objFile, int startAddr)
{
	for (int i = 0; i < DC; i++)
	{
		WriteInBase4(objFile, startAddr++, 4);
		fputc('\t', objFile);
		WriteInBase4(objFile, datastringArray[i], 10);
		fputc('\n', objFile);
	}
}
