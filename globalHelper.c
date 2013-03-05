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

// This function gets a line and return the word that's
// before the stop character given.
char* GetWord(char* line, char stop)
{
	int i = 0, j = 0; 													// Indices for line, word accordingly
	char* word = (char*) malloc(strlen(line) * sizeof(char));			// Allocate memory

	while (isspace(line[i])) i++;										// Ignore leading spaces

	while ((line[i] != stop) && !isspace(line[i]) && (line[i]!='\n'))	// Didn't reach stop or space or newline
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
	char* word = GetWord(string,'');
	int len = strlen(word);

	if (((strcmp(word[0],'-') == 0) || (strcmp(word[0],'+') == 0)) && (len > 1)) i++;		// Allow sign
	for (; i<len; i++)																		// Make sure it's a number
		if (!isdigit(word[i]))
			return 0;

	return 1;
}

// This function will skip white spaces from a given pointer to a string.
// Returns the first string index that is not a whitespace.
int FirstNonWhitespace(char* str, int i)
{
	int len = strlen(str);
	for (; (isspace(str[i]) && i<len); i++) ;
	return i;
}

/* This function is an extension of the known strtok. */
/* It gets an index of a wanted word out of the splitted expression. */
/* Returns NULL if the index is too big for the expression. */
char* strtokEx (char* str, char* seps, int index)
{
    char* ret = (char*)calloc(MAX_STRING_LENGTH, sizeof(char)), strCopy[MAX_STRING_LENGTH];
    char* temp;
    int i = 0;

	strcpy(strCopy, str);

    /* start tokenising this string until you get what you want. */
    temp = strtok(strCopy, seps);
    while (temp != NULL && i <= index)
    {
        strcpy(ret, temp);
        temp = strtok(NULL, seps);
        i++;
    }

    /* we finished parsing the string before the requested index was found. */
    if (i <= index)
    {
    	free(ret);
        return NULL;
    }
    else
        return ret;
}

// This function prints an error message and changes the global variable pstatus
// based on the fatality of the error, returns 1 if this is a recoverable error.
int PrintErrIsRecoverable(char* line, int lineNumber, char* object, char* errMsg, int isRecoverable)
{
	printf("Error in line %d | %s | %s %s.\n", lineNumber, line, object, errMsg);
	return (pstatus = isRecoverable);
}

// This function resets global variables of phase 1.
// These include all memory variables (instruction lines, string/data, symbols),
// as well as the global variables for the phase management.
void RestartMem()
{
	pstatus = 1;
	lineNumber = 0;
	RestartInstlines();
	RestartDatastrings();
	RestartSymbols();
}
