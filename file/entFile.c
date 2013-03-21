/*
 * The entFile files (.h,c) are responsible for creating the .ent file.
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "fileHelper.h"
#include "entFile.h"
#include "../config.h"
#include "../mem/instlineMem.h"

extern int LIC;
extern int IC;

// Variables for collecting entry symbols in phase 2
int entryCounter = 0;
Symbol* entryArr[MAX_ARRAY_LENGTH];

// This function writes the .ent file.
int WriteEntryFile (char fileName[MAX_FILE_NAME])
{
	Symbol *lastSymbol = NULL;
	FILE* entFile;

	for (int i = 0; i < LIC; i++)							// Mark relevant symbols as entries
	{
		instLine ins = instlineArray[i];
		if (strcmp(ins.ins->name, "entry") != 0)			// Skip all other instructions
			continue;
		Symbol* sym = GetSymbol(ins.op2.name, -1);
		if (sym == NULL)
		{
			fprintf(stderr, "Could not find symbol for entry: %s\n", ins.op1.name);
			return 0;
		}
		AddEntrySymbol(sym);
	}

	if (!entryCounter) return 1;							// Nothing to write - success

	entFile = CreateTypedFile(fileName, ".ent");			// Create the file
	if (entFile == NULL)
	{
		fprintf(stderr, "Could not create the Entries file for \"%s.as\"\n", fileName);
		return 0;
	}
	lastSymbol = GetNextEntrySymbol(-1);

	do														// Write all entry symbols ASC
	{
		int addr = lastSymbol->address;
		if (lastSymbol->type == DATA) addr += IC;			// If it's a DATA symbol add IC

		fprintf(entFile, "%s\t", lastSymbol->name);			// Write label and address to file
		WriteInBase4(entFile, addr, 4);
		fputc('\n', entFile);

		lastSymbol = GetNextEntrySymbol(addr);				// Next symbol
	} while (lastSymbol != NULL);

	fclose(entFile);
	return 1;
}

// This function returns the next entry symbol out of the entryArr.
// The symbol returned is in address which is bigger than the lastAddress given.
Symbol* GetNextEntrySymbol (int lastAddress)
{
	int minAddr = INT_MAX;
	Symbol *minSymbol = NULL;

	// Finding the minimum addressed symbol whose address is bigger than lastAddress
	for (int i = 0; i < entryCounter; i++)
	{
		Symbol* sym = entryArr[i];
		int addr = sym->address;
		if (sym->type == DATA) addr += IC;
		if (addr < minAddr && addr > lastAddress)
		{
			minAddr = addr;
			minSymbol = sym;
		}
	}
	return minSymbol;
}

// This function adds an entry symbol into the entryArr.
void AddEntrySymbol (Symbol* sym)
{
	if (sym->isEntry) return;								// Ignore previously added symbols

	sym->isEntry = 1;
	entryArr[entryCounter++] = sym;
}
