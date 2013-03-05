/*
 * The objFile files (.h,c) are responsible for creating the .ob file.
 */

#include <stdlib.h>
#include "objFile.h"
#include "fileHelper.h"
#include "../globalHelper.h"
#include "../config.h"
#include "../instructionHandler.h"
#include "../mem/instlineMem.h"
#include "../mem/datastringMem.h"

extern int datastringArray[MAX_ARRAY_LENGTH];
extern int DC;
extern int IC;

// Writes the opening line for the file - instruction count and data count (words).
void WriteOpenningLine(FILE* objFile)
{
	WriteInBase4(objFile, IC, 0);		// Program length
	fputc(' ', objFile);
	WriteInBase4(objFile, DC, 0);		// Data length
	fputc('\n', objFile);
}

// Writes an instruction to the .ob file.
// Returns the number of words added to the file, or 0 on error.
int WriteInstruction (FILE* objFile, instLine line, int startAddr)
{
	int length = 1;
	instructionWord iw;

	// Append the starting address.
	WriteInBase4(objFile, startAddr, 4);
	fputc('\t', objFile);

	// Create the instruction word object
	iw.instructionCode = line.ins->opCode;
	iw.sourceOperandType = line.op1.addrType;
	iw.sourceRegister = line.op1.reg;
	iw.destOperandType = line.op2.addrType;
	iw.destRegister = line.op2.reg;

	// Cast this instructionWord into unsigned short and append it to the file in base 4
	WriteInBase4(objFile, *((unsigned short*)&iw), 8);
	fputs("\tA\n", objFile);

	// Extract and append words from index-typed operands
	if (line.ins->numOfOp > 0)
	{
		if ((line.ins->numOfOp == 2) && (iw.sourceOperandType >= 0) && (iw.sourceOperandType <= 3))	// Src - not always available
		{
			startAddr++;
			int wordsAdded = AppendOperandWords(objFile, line.op1, startAddr);
			if (wordsAdded == 0) return 0;
			length += wordsAdded;
		}
		if (iw.destOperandType >= 0 && iw.destOperandType <= 3)										// Dest - should always be available
		{
			startAddr++;
			int wordsAdded = AppendOperandWords(objFile, line.op2, startAddr);
			if (wordsAdded == 0) return 0;
			length += wordsAdded;
		}
	}
	return length;
}


/* Appends an operand into file. */
/* The function should get only an operand whose type is 0 to 3 (contains label/immediate). */
/* Returns the number of words appended (0 = error). */
int AppendOperandWords (FILE* objFile, operand op, int startAddr)
{
	int imm;
	int addr = startAddr;
	char *label1, *label2;
	Symbol *sym1, *sym2;

	WriteInBase4(objFile, startAddr, 4);				/* First, write the starting address for the first symbol. */
	fputc('\t', objFile);

	switch (op.addrType)
	{
	case IMMEDIATE:
		label1 = strtokEx(op.name, "#", 0);
		if (label1 == NULL) return 0;

		imm = atoi(label1); /* Convert the immediate string to int. */
		WriteInBase4(objFile, imm, 8); /* Write it in base 4. */
		fputs("\tA\n", objFile);
		free(label1);
		return 1;
	case LABEL:
		sym1 = GetSymbol(op.name); /* Get the wanted symbol. */
		if (sym1 == NULL)
		{
			fprintf(stderr, "Could find symbol: %s\n", op.name);
			return 0;
		}
		AppendSymbol(objFile, sym1, addr); /* Append symbol to file. */
		return 1;
	case INDEX_LABLE:
		label1 = strtokEx(op.name, "[]", 0); /* Getting the first label (host). */
		label2 = strtokEx(op.name, "[]", 1); /* Getting the second label (offset). */
		sym1 = GetSymbol(label1), sym2 = GetSymbol(label2);

		if (sym1 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label1); free(label1); free(label2); return 0;
		}
		if (sym2 == NULL) {
			fprintf(stderr, "Could find symbol: %s\n", label2); free(label1); free(label2); return 0;
		}

		/* Append first symbol. */
		AppendSymbol(objFile, sym1, addr);

		addr = ++startAddr;

		/* We started a new line, write the next address. */
		WriteInBase4(objFile, addr, 4);
		fputc('\t', objFile);

		/* Append the second symbol. */
		AppendSymbol(objFile, sym2, addr);

		free(label1);
		free(label2);
		return 2;
	case INDEX_REGISTER:
		label1 = strtokEx(op.name, "[]", 0); /* Get the label (host). */
		sym1 = GetSymbol(label1);

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

/* Appends a symbol into the file. */
void AppendSymbol (FILE* objFile, Symbol* sym, unsigned int fileAddr)
{
	/* If this is a data symbol we should add the IC to it's address. */
	if (sym->type == DATA)
		WriteInBase4(objFile, IC + sym->address, 8);
	else
		WriteInBase4(objFile, sym->address, 8);

	/* Checking if this symbol is extern. */
	if (sym->isExtern)
	{
		fputs("\tE\n", objFile);

		/* Collecting this external symbol with it's referenced address. */
		AddExternalSymbol(sym, fileAddr);
	}
	else
		fputs("\tR\n", objFile);
}


/* Collects an external symbol into the externalArr. */
/* This array is a set of an external symbol coupled with it's referenced address in file. */
void AddExternalSymbol (Symbol *sym, unsigned int refAddr)
{
	//externalArr[externalCounter].sym = sym;
	//externalArr[externalCounter++].refAddress = refAddr;
}

/* Appends the datastringArray into the .ob file. */
void WriteDataToFile (FILE* objFile, int startAddr)
{
	int i;
	for (i = 0; i < DC; i++)
	{
		WriteInBase4(objFile, startAddr++, 4);
		fputc('\t', objFile);
		WriteInBase4(objFile, datastringArray[i], 8);
		fputc('\n', objFile);
	}
}
