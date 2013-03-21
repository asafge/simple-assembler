/*
 * The phase1 file is responsible for running the first phase of the compilation.
 * It actually uses instruction handler in order to handle different instruction types,
 * and this is just the handling of the high-level algorithm.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "globalHelper.h"
#include "instructionHandler.h"

void HandleLine(char* line);

/* --- Global variables for Phase 1 --- */
extern int pstatus;								// Phase status (is successful)
extern int lineNumber;							// The line number we're in
extern int lineStat;							// Line status (does not contain errors)
extern char* lineTxt;							// The original text of the current line

// Main function for Phase 1 execution.
// It reads a file, and for every line makes sure it shouldn't be
// ignored (comment / empty), and calls handleLine() for processing.
int RunPhase1(FILE* file)
{
	RestartMem();
	lineTxt = calloc(MAX_LINE, 1* sizeof(char));
	for (char line[MAX_LINE + 1] ; (fgets(line,sizeof(line),file) != NULL); lineNumber++)
	{
		strcpy(lineTxt, line);
		HandleLine(line);
	}
	free(lineTxt);
	return pstatus;
}

// This function is responsible for handling a single line in the first phase.
// Uses instructionHandler. Algorithm from page 28 in booklet is implemented as follows:
// * Line 6 = Algorithm lines 3-4.
// * Line 7 = Algorithm lines 5-7.
// * Line 8 = Algorithm lines 8-10.
// * Line 9 = Algorithm lines 11-13.
void HandleLine(char* line)
{
	lineStat = 1;
	int i = FirstNonWhitespace(line, 0);					// Line position index
	if ((i == -1) || line[i]==';') return; 					// Process line only if not empty or comment

	int sIndex = IsSymbol(line);							// Is this a leading symbol? Return length of its name.
	if(!IsStringOrData(line,sIndex))						// Is this an instruction to save data? (.data / .string)
		if(lineStat && !IsEntryOrExtern(line,sIndex))		// Is this an .entry or an .extern?
			if (lineStat) IsOpInstruction(line,sIndex);		// Is this an operation instruction? ("mov" etc)
}
