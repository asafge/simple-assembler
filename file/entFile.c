/*
 * entFile.c
 *
 *  Created on: Mar 4, 2013
 *      Author: asafg
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

/* This is used to collect entry symbols during phase 2. */
int entryCounter = 0;
Symbol* entryArr[MAX_ARRAY_LENGTH];


/* Writes the Entry file. */
int WriteEntryFile (char fileName[MAX_FILE_NAME])
{
	int i;
	Symbol *lastSymbol = NULL;
	FILE* entFile;

	/* Mark relevant symbols as entries. */
	/* This action is done by iterating all .entry instructions alone. */
	for (i = 0; i < LIC; i++)
	{
		Symbol *sym;

		instLine fins = instlineArray[i];
		if (strcmp(fins.ins->name, ".entry") != 0)
			continue;

		sym = GetSymbol(fins.op1.name);

		if (sym == NULL)
		{
			fprintf(stderr, "Could not find symbol for entry: %s\n", fins.op1.name);
			return 0;
		}

		/* Collecting this symbol. */
		AddEntrySymbol(sym);
	}

	/* If no entries were found, return in success right now. */
	if (!entryCounter)
		return 1;

	/* Create the .ent file. */
	entFile = CreateTypedFile(fileName, ".ent");

	if (entFile == NULL)
	{
		fprintf(stderr, "Could not create the Entries file for \"%s.as\"\n", fileName);
		return 0;
	}

	/* Get the first entry symbol. */
	lastSymbol = GetNextEntrySymbol(-1);

	/* Write all entry symbols in ascending order (by address). */
	do
	{
		int addr = lastSymbol->address;
		if (lastSymbol->type == DATA) /* If it's a DATA symbol, add the IC to it. */
			addr += IC;

		fprintf(entFile, "%s\t", lastSymbol->name);
		WriteInBase4(entFile, addr, 0);
		fputc('\n', entFile);

		/* Get the next symbol. */
		lastSymbol = GetNextEntrySymbol(addr);
	} while (lastSymbol != NULL);

	fclose(entFile);
	return 1;
}

/* This function returns the next entry symbol out of the entryArr. */
/* The symbol returned is in address which is bigger than the lastAddress given. */
Symbol* GetNextEntrySymbol (int lastAddress)
{
	int minAddr = INT_MAX;
	Symbol *minSymbol = NULL;
	int i;

	/* Finding the minimum addressed symbol whose address is bigger than lastAddress which was given to the function. */
	for (i = 0; i < entryCounter; i++)
	{
		Symbol *sym = entryArr[i];
		int addr = sym->address;

		if (sym->type == DATA)
			addr += IC;

		if (addr < minAddr && addr > lastAddress)
		{
			minAddr = addr;
			minSymbol = sym;
		}
	}

	return minSymbol;
}

/* Collects an entry symbol into the entryArr. */
void AddEntrySymbol (Symbol *sym)
{
	/* Checking if this symbol was already marked and added. */
	if (sym->isEntry)
		return;

	sym->isEntry = 1;
	entryArr[entryCounter++] = sym;
}
