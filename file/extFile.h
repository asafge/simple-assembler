/*
 * The extFile files (.h,c) are responsible for creating the .ext file.
 */
#ifndef EXTFILE_H
	#define EXTFILE_H

	#include "../config.h"

	void WriteExternalFile(char fileName[MAX_FILE_NAME]);
	void AddExternalSymbol(Symbol* sym, unsigned int fileAddr);

#endif
