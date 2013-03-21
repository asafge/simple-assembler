/* --- Definition of helper functions --- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "globalHelper.h"
#include "config.h"
#include "mem/instlineMem.h"
#include "mem/symbolMem.h"
#include "mem/datastringMem.h"

extern int pstatus;
extern int lineNumber;
extern int lineStat;
extern char* lineTxt;

// This function gets a line and return the word that's
// before the stop character given.
char* GetWord(char* line, char stop)
{
	int j = 0; 															// Indices for line, word accordingly
	int i = FirstNonWhitespace(line, 0);								// Ignore leading spaces
	if (i == -1) return "";

	char* word = (char*) malloc(strlen(line) * sizeof(char));			// Allocate memory

	while ((line[i] != stop) && !isspace(line[i]) 						// Did not reach: stop, space, '\0'
			&& (line[i]!='\n') && (line[i]!='\0'))
		word[j++] = line[i++];											// Save char to word and update indices

	word[j]='\0';														// Mark the end of this string

	if (j > 0)
	{
		return word;
	}
	else
	{
		free(word);
		return "";
	}
}

// This function checks if a given string is a number.
// Will ignore white-spaces and make sure the first char is numeric / sign.
int IsNumber(char* string)
{
	int i=0;
	char* word = GetWord(string,'\0');
	int len = strlen(word);

	if (((strncmp(&word[0],"-",1) == 0) || (strncmp(&word[0],"+",1) == 0)) && (len > 1)) i++;		// Allow sign
	for (; i<len; i++)																		// Make sure it's a number
		if (!isdigit(word[i]))
			return 0;

	return 1;
}

// This function will skip white spaces from a given pointer to a string.
// Returns the first index that is not a whitespace, -1 if non found.
int FirstNonWhitespace(char* str, int i)
{
	int len = strlen(str), flag = 0;

	for (; (i<len); i++)
		if (!isspace(str[i]))
		{ flag = 1; break; 	}

	return (flag) ? i : -1;
}

// This function prints an error message and changes the global variables pstatus, lineStatsss
// based on the fatality of the error, returns 1 if this is a recoverable error.
// TODO: Ignore \n when printing
int PrintSaveError(char* object, char* errMsg)
{
	printf(">>> Error in line %d | %s | %s %s.\n", lineNumber + 1, lineTxt, object, errMsg);
	return (lineStat = pstatus = 0);
}

// This function resets global variables of phase 1.
// These include all memory variables (instruction lines, string/data, symbols),
// as well as the global variables for the phase management.
void RestartMem()
{
	pstatus = 1;
	lineStat = 1;
	lineNumber = 0;
	RestartInstlines();
	RestartDatastrings();
	RestartSymbols();
}

// This function extends strtok by allowing user to request for the i-th
// result of a regula strtok call. Returns NULL if index is too large.
char* strtokEx (char* str, char* seps, int index)
{
    char* ret = (char*)calloc(MAX_STRING_LENGTH, sizeof(char)), strCopy[MAX_STRING_LENGTH];
    char* temp;
    int i = 0;

	strcpy(strCopy, str);
    temp = strtok(strCopy, seps);
    while (temp != NULL && i <= index)			// Sequence of strtok() calls
    {
        strcpy(ret, temp);
        temp = strtok(NULL, seps);
        i++;
    }

    if (i <= index)								// Index too large
    {
    	free(ret);
        return NULL;
    }
    else
        return ret;
}
