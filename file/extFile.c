/*
 * extFile.c
 *
 *  Created on: Mar 4, 2013
 *      Author: asafg
 */

#include <stdlib.h>
#include "fileHelper.h"
#include "extFile.h"
#include "../config.h"

/* This is used to collect external symbols during phase 2. */
int externalCounter = 0;
externalSymbol externalArr[MAX_ARRAY_LENGTH];

/* Writes the External file. */
void WriteExternalFile(char fileName[MAX_FILE_NAME])
{
	FILE* extFile;
	int i;

	if (externalCounter == 0)
		return;

	/* Creating the .ext file. */
	extFile = CreateTypedFile(fileName, ".ext");

	if (extFile == NULL)
	{
		fprintf(stderr, "Could not create the Externals file for \"%s.as\"\n", fileName);
		return;
	}

	/* Iterating all external symbols. */
	for (i = 0; i < externalCounter; i++)
	{
		externalSymbol exSym = externalArr[i];

		/* Writing the external symbol. */
		fprintf(extFile, "%s\t", exSym.sym->name);

		/* Writing the external referenced address. */
		WriteInBase4(extFile, exSym.refAddress, 0);
		fputc('\n', extFile);
	}

	fclose(extFile);
}

