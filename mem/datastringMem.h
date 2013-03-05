/*
 * dataMem.h
 *
 *  Created on: Mar 2, 2013
 *      Author: asafg
 */

#ifndef DATASTRINGMEM_H_
	#define DATASTRINGMEM_H_

	#include "../config.h"

	/* --- Declaration of functions --- */
	void RestartDatastrings();
	void StoreData(char* data);

	/* --- Global data/string variables --- */
	int DC;											// Data Counter
	int datastringArray[MAX_ARRAY_LENGTH];			// Array containing DATA/STRING values

#endif
