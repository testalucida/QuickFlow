/*
 * CanvasController.h
 *
 *  Created on: 04.04.2020
 *      Author: martin
 */

#ifndef CANVASCONTROLLER_H_
#define CANVASCONTROLLER_H_

#include "enums.h"

class FlowChartCanvas;
class Fl_Widget;
class SymbolBox;
class Fl_Menu_Button;

class CanvasController {
public:
	CanvasController(FlowChartCanvas* p);
	virtual ~CanvasController();

	static void symbolSelectedCallback_static(SymbolId id, void* pThis) {
			((CanvasController*)pThis)->onSymbolSelected(id);
	}
	void onSymbolSelected(SymbolId id);

	SymbolBox* addSymbol( SymbolId, int x, int y );
	void addSymbol( SymbolBox* );
	void setLabel( SymbolBox*, const char* );

	bool openChanges() const;

	static void onQuitCallback_static( Fl_Widget*, void* );

	void onQuitCallback();

private:
	static void onSymbolBoxPush_static( SymbolBox*, int, void* );
	void onSymbolBoxPush( SymbolBox*, int whichButton );
	static void onCanvasPush_static( Fl_Widget*, void* );
	void onCanvasPush( Fl_Widget* );
	void connect( SymbolBox*, SymbolBox* );
	void destroy( SymbolBox* );

private:
	FlowChartCanvas* _pCanvas;
	SymbolId _symbolId = SymbolId::NONE;
	bool _isSymbolDialogOpen = false;
	Fl_Menu_Button* _symbolbox_popup = 0;
};


#endif /* CANVASCONTROLLER_H_ */
