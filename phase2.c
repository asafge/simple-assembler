/*
 * The phase2 file is responsible for running the second phase of the compilation.
 * It uses other files for individual tasks and is responsible for the high level process.
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "config.h"
#include "file/entFile.h"
#include "file/extFile.h"
#include "file/objFile.h"
#include "file/fileHelper.h"

void RollbackPhase2(char fileName[MAX_FILE_NAME]);

/* --- Global variables for Phase 2 --- */
extern int LIC;
extern instLine instlineArray[MAX_ARRAY_LENGTH];

// Main function for Phase 2 execution.
// It gets a file name and creates its .ob .ext and .ent files.
void RunPhase2(char fileName[MAX_FILE_NAME])
{
	// Create and open the .ob file for writing
	FILE* objFile = CreateTypedFile(fileName, ".ob");
	if (objFile == NULL)
	{
		printf(stderr, "Could not open object file for write.\n");
		return;
	}

	// Start writing .ob file below
	int currentIC = 0;
	WriteOpenningLine(objFile);

	for (int i = 0; i < LIC; i++)														// Add instruction saved in phase 1
	{
		if (strcmp(instlineArray[i].ins->name, "entry") == 0) continue; 				// Handle later on
		
		int wordsAdded = WriteInstruction(objFile, instlineArray[i], currentIC);		// Write instruction and get # of words
		if (!wordsAdded)
		{
			fprintf(stderr, "Error while processing file instruction on address:");
			WriteInBase4(stderr, currentIC, 4);
			fputc('\n', stderr);
			fclose(objFile);

			RollbackPhase2(fileName);
			return;
		}
		else
			currentIC += wordsAdded;
	}
	WriteDataToFile(objFile, currentIC);												// Append Data to the end of the .ob file

	if (!WriteEntryFile(fileName))														// Write the .ent file
	{
		fclose(objFile);
		RollbackPhase2(fileName);
		return;
	}
	WriteExternalFile(fileName);														// Write the .ext file (If needed)
	fclose(objFile);
}

// Prints phase 2 error and removes the .ob file created.
void RollbackPhase2(char fileName[MAX_FILE_NAME])
{
	char fileNameTyped[MAX_FILE_NAME];
	fprintf(stderr, "Could not complete Phase2.\n");
	strcpy(fileNameTyped, fileName);
	strcat(fileNameTyped, ".ob");
	remove(fileNameTyped);
}

