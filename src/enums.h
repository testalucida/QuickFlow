/*
 * enums.h
 *
 *  Created on: 04.04.2020
 *      Author: martin
 */

#ifndef ENUMS_H_
#define ENUMS_H_


enum SymbolId {
	START,
	LINE,
	DECISION,
	PROCESS,
	INPUT_OUTPUT,
	MANUAL_INPUT,
	DOCUMENT,
	MANUAL_OP,
	DATA_STORAGE,
	CONNECTOR,
	END,
	NONE
	//count = 12
};

static const int ALL_SYMBOLS_COUNT  __attribute__ ((unused)) = 12;


#endif /* ENUMS_H_ */
