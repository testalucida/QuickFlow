/*
 * flowchartui.h
 *
 *  Created on: 04.04.2020
 *      Author: martin
 */

#ifndef FLOWCHARTUI_H_
#define FLOWCHARTUI_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>

#include <fltk_ext/Canvas.h>

#include "symbolstuff.h"
#include "Connection.hpp"
#include "IClientArea.h"

//+++++++++++++++++++++++++++++++++++++++

class FlowChartMenuBar : public Fl_Menu_Bar {
public:
	FlowChartMenuBar(int x, int y, int w, int h);
};

//+++++++++++++++++++++++++++++++++++++++

class FlowChartToolBar : public Fl_Group {
public:
	FlowChartToolBar(int x, int y, int w, int h);
	void registerSymbolCallback(SymbolClickCallback*, void*);
	static void staticSymbolCallback(SymbolId, void*);
	void symbolCallback(SymbolId) const;
	SymbolBar* getSymbolBar() const {return _pSymbolBar;}
private:
	SymbolBar* _pSymbolBar;
	SymbolClickCallback* _cb = NULL;
	void* _data = NULL;
};

//+++++++++++++++++++++++++++++++++++++++

class StatusBar : public Fl_Group {
public:
	StatusBar(int x, int y, int w, int h);
	virtual void resize(int x, int y, int w, int h);
};

//+++++++++++++++++++++++++++++++++++++++

typedef void (*CanvasCallback)(int event, void* userdata);

//+++++++++++++++++++++++++++++++++++++++

class FlowChartCanvas : public Canvas {
public:
	FlowChartCanvas( int x, int y, int w, int h );
	void registerEventCallback( CanvasCallback ccb, void* pUserdata) ;
	/** creates a connection between the given SymbolBoxes */
	Connection* createConnection( SymbolBox* box1, SymbolBox* box2 );
	/** remove all connections concerning the given SymbolBox */
	void removeConnections( SymbolBox* );
protected:
	int handle( int e );
	virtual void draw_then();
private:
	void checkSelectConnectionAt( int x, int y );
	void resetSelections();
private:
	CanvasCallback _ccb = NULL;
	void* _callback_userdata = NULL;
	std::vector<Connection*> _connections;
};

//+++++++++++++++++++++++++++++++++++++++

class FlowChartMainWindow : public Fl_Window, public IClientArea {
public:
	FlowChartMainWindow(int x, int y, int w, int h);
	void registerSymbolSelectedCallback(SymbolClickCallback*, void*);
	FlowChartCanvas* getCanvas() const {return _pCanvas;}
	void selectSymbol(SymbolId id);

	FlxRectPtr getClientArea() const {
		FlxRectPtr rect( new FlxRect );
		rect->x = _margin_x;
		rect->y = _pToolBar->y() + _pToolBar->h();
		rect->w = w() - 2*_margin_x;
		rect->h = h() - _pMenuBar->h() - _pToolBar->h() - _pStatusBar->h();
		return rect;
	}
private:
	static void staticOnSymbolSelected(SymbolId, void*);
	void onSymbolSelected(SymbolId);
private:
	int _margin_x = 10;
	FlowChartMenuBar* _pMenuBar;
	FlowChartToolBar* _pToolBar;
	StatusBar* _pStatusBar;
	FlowChartCanvas* _pCanvas;
	SymbolClickCallback* _cb = NULL;
	void* _data = NULL;
};

//+++++++++++++++++++++++++++++++++++++++

#endif /* FLOWCHARTUI_H_ */
