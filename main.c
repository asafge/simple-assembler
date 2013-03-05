/*
 ============================================================================
 Name        : Maman 14.
 Author      : Asaf Geva.
 Description : This is the main entry for the program. It will process the
 	 	 	   files and initiate Phase1 and Phase2.
 Notes:		 : I've used include guards in all header files in order to
 	 	 	   prevent errors when function/ variables gets declared twice.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "globalHelper.h"

void HandleFile (char* filePath);
int RunPhase1(FILE* file);

extern int pstatus;

// Main entry function
int main(int argc, char *argv[])
{
	if (argc < 2)
		puts("Nothing to compile. Use: \"assembler <file1> <file2>\" instead.");
	else
	{
		RestartMem();
		// Ignore first argument (program name) and handle files.
		puts("Starting to read files...");
		for (int i=1; i<argc; i++)
			HandleFile(argv[i]);
	}
	exit(EXIT_SUCCESS);
}

// Handle each specific file in the arguments list.
// First add .as extension and check that the file exists and readable.
// Then run phase 1, and if successful run phase 2.
void HandleFile(char* name)
{
	// Create a string of the predicted file name
	char fileName[strlen(name)+2];
	strcpy(fileName,name);
	strcat(fileName,".as");

	// Check if such a file exists and is ready for reading
	FILE* file;
	if((file = fopen(fileName, "r")) == NULL)
		printf("> Could not open \"%s\" for reading. Please make sure it exists and has read permissions.\n",fileName);
	else
	{
		printf("> Found file: %s.\n>> Running phase 1...\n",fileName);
		RunPhase1(file);
		fclose(file);

		if(pstatus)
		{
			printf(">> Running phase 2...\n");
			//RunPhase2(fileName);
			printf("----- Done -----\n");
		}
		else
			printf(">> Found errors in %s, object file would not be created.\n",fileName);

		RestartMem();
	}
}
