#ifndef GLOBALHELPER_H
	#define GLOBALHELPER_H

	#include <stdlib.h>

	/* --- Declaration of functions --- */
	char *GetWord (char *line , char stop);
	char* strtokEx (char* str, char* seps, int index);
	int IsNumber(char* string);
	int FirstNonWhitespace(char* str, int i);
	int PrintSaveError(char* object, char* errMsg);
	void RestartMem();

	/* --- Global variables for Phase 1 --- */
	int pstatus;								// Phase status (is successful)
	int lineNumber;								// The line number we're in
	char* lineTxt;								// The original text of the current line
	int lineStat;								// Line status (does not contain errors)

#endif
