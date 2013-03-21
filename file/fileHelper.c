/*
 * The fileHelper files (.h,c) include helper types and functions.
 */
#include <stdio.h>
#include <string.h>
#include "../config.h"

// This function writes to file a given number in base 4.
// Also supports padding if wantedLength not reached.
void WriteInBase4 (FILE* file, int num, int wantedLength)
{
	int mod = 0, div = num;
	char finalStr[100];

	finalStr[0] = 0;									// Convert to base 4
	do
	{
		char str[2];
		mod = div % 4;
		div = div / 4;
		sprintf(str, "%d", mod);
		strcat(finalStr, str);
	} while (div > 0);

	int missing = wantedLength - strlen(finalStr);		// Padding
	while (missing-- > 0)
		fputc('0', file);

	for (int i = strlen(finalStr)-1; i >= 0; i--)		// Write string to file backwards
		fputc(finalStr[i], file);
}

// This function creates a file ending with an "extension".
// Example usage: CreateTypedFile("abc","txt").
FILE* CreateTypedFile (char fileName[MAX_FILE_NAME], char type[MAX_FILE_NAME])
{
	char tFileName[MAX_FILE_NAME];
		strcpy(tFileName, fileName);
		strcat(tFileName, type);

	FILE* file = fopen(tFileName, "w");
	return file;
}

