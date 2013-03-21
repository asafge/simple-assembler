#include <string.h>
#include <stdlib.h>
#include "datastringMem.h"

extern	int DC;
extern int datastringArray[MAX_ARRAY_LENGTH];

// Restart global variables
void RestartDatastrings()
{
	DC = 0;
}

// Stores DATA values in the dataArray
void StoreNumber(int data)
{
	datastringArray[DC] = data;
	DC++;
}

// Stores STRING values in the dataArray (as int)
void StoreString(char* data)
{
	int len = strlen(data);
	for (int i=0; i < len; i++, DC++)
		datastringArray[DC] = (int)(data[i]);

	datastringArray[DC] = '\0';
	DC++;
}
