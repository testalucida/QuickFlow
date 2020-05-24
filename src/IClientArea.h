/*
 * IClientArea.h
 *
 *  Created on: 12.05.2020
 *      Author: martin
 */

#ifndef ICLIENTAREA_H_
#define ICLIENTAREA_H_

#include <fltk_ext/FlxRect.h>

class IClientArea {
public:
	virtual FlxRectPtr getClientArea() const = 0;
	~IClientArea() {}
private:
	//IClientArea() {}

};


#endif /* ICLIENTAREA_H_ */
