/*
 * The extFile files (.h,c) are responsible for creating the .ext file.
 */
#include <stdlib.h>
#include "fileHelper.h"
#include "extFile.h"
#include "../config.h"

// Variables for collecting external symbols in phase 2
int externalCounter = 0;
externalSymbol externalArr[MAX_ARRAY_LENGTH];

// This function writes the .ext file.
void WriteExternalFile(char fileName[MAX_FILE_NAME])
{
	if (externalCounter == 0) return;

	FILE* extFile = CreateTypedFile(fileName, ".ext");
	if (extFile == NULL)
	{
		fprintf(stderr, "Could not create the Externals file for \"%s.as\"\n", fileName);
		return;
	}

	for (int i = 0; i < externalCounter; i++)			// Iterate all external symbols
	{
		externalSymbol exSym = externalArr[i];
		fprintf(extFile, "%s\t", exSym.sym->name);		// Write symbol
		WriteInBase4(extFile, exSym.refAddress, 4);		// Write reference address
		fputc('\n', extFile);
	}
	fclose(extFile);
}

// This function collects the external symbols and their addresses.
void AddExternalSymbol(Symbol* sym, unsigned int fileAddr)
{
	externalArr[externalCounter].sym = sym;
	externalArr[externalCounter++].refAddress = fileAddr;
}
