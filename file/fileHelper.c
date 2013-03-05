/*
 * The fileHelper files (.h,c) include helper types and functions.
 * Note: Assuming here that unsigned short has the length of 16 bits.
 */
#include <stdio.h>
#include <string.h>
#include "../config.h"

/* This function writes to file a given number in base 4. */
/* You can also ask for a wanted length for the converted number and it will be padded with zeros. */
void WriteInBase4 (FILE* file, int num, int wantedLength)
{
	int mod = 0, div = num, missing, i;
	char finalStr[100];

	/* Converting to base 4 while collecting the characters into finalStr. */
	finalStr[0] = 0;
	do
	{
		char str[2];
		mod = div % 4;
		div = div / 4;
		sprintf(str, "%d", mod);
		strcat(finalStr, str);
	} while (div > 0);

	/* Do we need to pad this string because of a desired length? */
	missing = wantedLength - strlen(finalStr);
	while (missing-- > 0)
	{
		fputc('0', file);
	}

	/* Finally, writing this string into the file while reading it backwards. */
	for (i = strlen(finalStr)-1; i >= 0; i--)
	{
		fputc(finalStr[i], file);
	}
}

/* Attemps to create a file with a specific extension. */
/* The file will be created with write permissions only. */
/* For example: CreateTypedFile("ps", ".ext") is a valid call. */
FILE* CreateTypedFile (char fileName[MAX_FILE_NAME], char type[MAX_FILE_NAME])
{
	char tFileName[MAX_FILE_NAME];
	FILE* file;

	strcpy(tFileName, fileName);
	strcat(tFileName, type);
	file = fopen(tFileName, "w");

	return file;
}

