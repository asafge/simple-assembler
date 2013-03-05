#include "datastringMem.h"

extern	int DC;
extern int datastringArray[MAX_ARRAY_LENGTH];

// Restart global variables
void RestartDatastrings()
{
	DC = 0;
}

// Stores DATA/STRING values in the dataArray
void StoreData(char* data)
{
	datastringArray[DC] = &data;			// FIXME: handle strings
	DC++;
}
