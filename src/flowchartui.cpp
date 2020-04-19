/*
 * flowchartui.cpp
 *
 *  Created on: 04.04.2020
 *      Author: martin
 */

#include "flowchartui.h"
#include <FL/Fl_Toggle_Button.H>
#include "std.h"

//#include <fltk_ext/Canvas.h>

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartMainWindow::FlowChartMainWindow(int x, int y, int w, int h) :
		Fl_Double_Window(x, y, w, h, "FlowChart Creator")
{
	int margin = 10;
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	_pMenuBar = new FlowChartMenuBar(0, 0, w, 25);
	_pToolBar = new FlowChartToolBar(0, _pMenuBar->h(), w, 35);
	_pToolBar->registerSymbolCallback(
			FlowChartMainWindow::staticOnSymbolSelected, this);
	int status_h = 30;
	_pStatusBar = new StatusBar(0, h - status_h, w, status_h);

	int canvas_h = h - _pMenuBar->h() - _pToolBar->h() - _pStatusBar->h();
	_pCanvas = new FlowChartCanvas(0+margin, _pToolBar->y() + _pToolBar->h(),
			              w-2*margin, canvas_h);
	_pCanvas->end();

	resizable(_pCanvas);

	end();
}

void FlowChartMainWindow::
registerSymbolSelectedCallback(SymbolClickCallback* cb, void* data) {
	_cb = cb;
	_data = data;
}

void FlowChartMainWindow::selectSymbol(SymbolId id) {
	SymbolBar* pBar = _pToolBar->getSymbolBar();
	pBar->setSymbolSelected(id);
}

void FlowChartMainWindow::staticOnSymbolSelected(SymbolId id, void* data) {
	FlowChartMainWindow* win = (FlowChartMainWindow*)data;
	win->onSymbolSelected(id);
}

void FlowChartMainWindow::onSymbolSelected(SymbolId id) {
	//fprintf(stderr, "FlowChartMainWindow::onSymbolSelected\n");
	if (_cb) {
		(*_cb)(id, _data);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartMenuBar::FlowChartMenuBar(int x, int y, int w, int h) :
		Fl_Menu_Bar(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_LIGHT1);
	labelcolor(FL_WHITE);
	add("File/Quit",   FL_CTRL+'q', NULL /*callback*/);
	add("Edit/Change", FL_CTRL+'c', NULL);
	add("Edit/Submenu/Aaa");
	add("Edit/Submenu/Bbb");
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartToolBar::FlowChartToolBar(int x, int y, int w, int h) :
		Fl_Group(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	_pSymbolBar = new SymbolBar(x+10, y,
			FlowChartToolBar::staticSymbolCallback, (void*)this);
	int filler_x = _pSymbolBar->x() + _pSymbolBar->w();
	int filler_w = w - filler_x;
	Fl_Box* pFiller = new Fl_Box(filler_x, y, filler_w, h);
	end();
	resizable(pFiller);
}

void FlowChartToolBar::registerSymbolCallback(
		SymbolClickCallback* cb, void* data)
{
	_cb = cb;
	_data = data;
}

void FlowChartToolBar::staticSymbolCallback(SymbolId id, void* data) {
	FlowChartToolBar* pTb = (FlowChartToolBar*)data;
	pTb->symbolCallback(id);
}

void FlowChartToolBar::symbolCallback(SymbolId id) const {
	fprintf(stderr, "FlowChartToolBar::symbolCallback: selected: %d\n", id);
	//forward to registered callback:
	if (_cb) {
		(*_cb)(id, _data);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartCanvas::FlowChartCanvas(int x, int y, int w, int h) :
		Canvas(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_WHITE);
}

void FlowChartCanvas::createConnection( SymbolBox* box1, SymbolBox* box2 ) {
	Connection* conn = new Connection( box1, box2 );
	_connections.push_back( conn );
}

void FlowChartCanvas::removeConnections( SymbolBox* box1 ) {
	auto it = _connections.begin();
	while( it != _connections.end() ) {
		if( (*it)->box1() == box1 || (*it)->box2() == box1 ) {
			delete *it;
			it = _connections.erase( it );
		} else {
			it++;
		}
	}

	redraw();
}

void FlowChartCanvas::registerEventCallback(CanvasCallback ccb, void* userdata) {
	_ccb = ccb;
	_callback_userdata = userdata;
}

//void FlowChartCanvas::addSymbol( SymbolBox* pSymbol ) {
//	add(pSymbol);
//	redraw();
//}

int FlowChartCanvas::handle(int e) {
	int rc = Canvas::handle( e );
	switch( e ) {
	case FL_PUSH:
		resetSelections();
		if( Fl::belowmouse() == this ) {
			checkSelectConnectionAt( Fl::event_x(), Fl::event_y() );
		}
		break;
	default:
		break;
	}
	return rc;
}

void FlowChartCanvas::draw_then() {
	//first draw the connections...
	for( auto conn : _connections ) {
		conn->draw();
	}

	//...then the symbol boxes
	for( int i = 0, imax = children(); i < imax; i++ ) {
		SymbolBox* box = dynamic_cast<SymbolBox*>( child(i) );
		if( box ) box->drawSymbol();
	}

	//This order is important hence the connections which are
	//drawn from one box' to the other box' centers cannot be
	//seen within the boxes.
}

void FlowChartCanvas::checkSelectConnectionAt( int x, int y ) {
		for( auto c : _connections ) {
			if( c->isNearby( x, y ) ) {
				c->setSelected( true );
			}
		}
	}

void FlowChartCanvas::resetSelections() {
		for( auto c : _connections ) {
			c->setSelected( false );
		}
	}


//+++++++++++++++++++++++++++++++++++++++

StatusBar::StatusBar(int x, int y, int w, int h) :
		Fl_Group(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	end();
}

void StatusBar::resize(int x, int y, int w, int h) {
//	int H = this->h();
//	fprintf(stderr, "StatusBar::resize: x/y/w/h/H: %d/%d/%d/%d/%d\n ",
//			x, y, w, h, H);

	Fl_Group::resize(x, y, w, h);
}
//+++++++++++++++++++++++++++++++++++++++

