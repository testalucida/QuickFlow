/*
 * symbolstuff.cpp
 *
 *  Created on: 25.03.2020
 *      Author: martin
 */

#include "symbolstuff.h"

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/fl_draw.H>

#include <string>
#include <iostream>
using namespace std;

#include "dialogs.h"

//#include "./symbols/decision.xpm"
//#include "./symbols/process.xpm"
//#include "./symbols/inputoutput.xpm"
//#include "./symbols/manual_input.xpm"
//#include "./symbols/document.xpm"
//#include "./symbols/manual_op.xpm"
//#include "./symbols/datastorage.xpm"
//#include "./symbols/connector.xpm"
//#include "./symbols/start.xpm"
//#include "./symbols/end.xpm"

//const char** symbolpixmap[] = {
//		decision_xpm,
//		process_xpm,
//		inputoutput_xpm,
//		manual_input_xpm,
//		document_xpm,
//		manual_op_xpm,
//		datastorage_xpm,
//		connector_xpm,
//		start_xpm,
//		end_xpm
//};

const char *symbol_svg[] = { "./src/symbols/start.svg",
		"./src/symbols/line.svg", "./src/symbols/decision.svg",
		"./src/symbols/process.svg", "./src/symbols/inputoutput.svg",
		"./src/symbols/manualinput.svg", "./src/symbols/document.svg",
		"./src/symbols/manualoperation.svg", "./src/symbols/datastorage.svg",
		"./src/symbols/connector.svg", "./src/symbols/end.svg",
		"./src/symbols/none.svg" };

const char *symboltooltip[] = { "Start",
		"Draw a connection line between 2 symbols", "Decision", "Process",
		"Input / Output", "Manual Input", "Document", "Manual Operation",
		"Data Storage", "Connector", "End",
		"Unselect symbol, activate Select Tool" };

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SymbolButton::SymbolButton( int x, int y, SymbolId symbolId,
		const char *svg_file ) :
		Fl_Toggle_Button( x, y, SymbolButton::getButtonWidth(),
				SymbolButton::getButtonWidth() ) {
	_symbolId = symbolId;
	clear_visible_focus();
	box( FL_FLAT_BOX );
	color( FL_LIGHT2 );
	down_box( FL_DOWN_BOX );
	down_color( FL_YELLOW );
	Fl_SVG_Image *svg = new Fl_SVG_Image( svg_file );
	svg->scale( w() - 2, h() - 2 );
	image( svg );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SymbolBar::SymbolBar( int x, int y, SymbolClickCallback *cb, void *data ) :
		Fl_Group( x, y,
				ALL_SYMBOLS_COUNT * SymbolButton::getButtonWidth()
						+ ALL_SYMBOLS_COUNT * SPACING + SPACING, 34 ) {
	_cb = cb;
	_data = data;
	//type(Fl_Pack::HORIZONTAL);
	box( FL_BORDER_FRAME );
	color( FL_WHITE );
	//spacing(SPACING);
	end();
	int X = x + SymbolBar::SPACING, Y = y + SymbolBar::SPACING;
	for ( int i = 0, imax = ALL_SYMBOLS_COUNT; i < imax; i++ ) {
		SymbolId id = (SymbolId) i;
		_symbolIds.push_back( id );
		addButton( X, Y, id, cb, data );
		X += (SymbolButton::getButtonWidth() + SymbolBar::SPACING);
	}
	_selectedSymbol = _symbolIds.at( 0 );
}

SymbolBar::SymbolBar( int x, int y, SymbolIds ids, SymbolClickCallback *cb,
		void *data ) :
		Fl_Group( x, y,
				ids.size() * SymbolButton::getButtonWidth()
						+ ids.size() * SPACING + SPACING, 34 ) {
	_cb = cb;
	_data = data;
	//type(Fl_Pack::HORIZONTAL);
	box( FL_BORDER_FRAME );
	color( FL_WHITE );
	//spacing(SPACING);
	end();
	int X = x + SymbolBar::SPACING, Y = y + SymbolBar::SPACING;
	for ( int i = 0, imax = ids.size(); i < imax; i++ ) {
		SymbolId id = ids.at( i );
		_symbolIds.push_back( id );
		addButton( X, Y, id, cb, data );
		X += (SymbolButton::getButtonWidth() + SymbolBar::SPACING);
	}
	_selectedSymbol = ids.at( 0 );
}

void SymbolBar::resize( int x, int y, int w, int h ) {
	Fl_Group::resize( x, y, w, h );
}

void SymbolBar::staticOnSymbolClick( Fl_Widget *p, void *data ) {
	SymbolBar *pThis = (SymbolBar*) data;
	pThis->onSymbolClick( (SymbolButton*) p );
}

void SymbolBar::onSymbolClick( SymbolButton *b ) {
	_selectedSymbol = b->getSymbolId();

	for ( int i = 0, imax = children(); i < imax; i++ ) {
		try {
			SymbolButton *btn = (SymbolButton*) child( i );
			if ( btn != b )
				btn->value( 0 );
		} catch ( ... ) {
		}
	}

	fprintf( stderr, "click: %d\n", b->getSymbolId() );
	if ( _cb ) {
		(*_cb)( _selectedSymbol, _data );
	}
}

SymbolButton* SymbolBar::getSymbolButton( SymbolId symbolId ) const {
	for ( int i = 0, imax = children(); i < imax; i++ ) {
		try {
			SymbolButton *btn = (SymbolButton*) child( i );
			SymbolId child_symbolId = btn->getSymbolId();
			if ( child_symbolId == symbolId ) {
				return btn;
			}
		} catch ( ... ) {
		}
	}
	throw runtime_error(
			"SymbolBar::getSymbolButton " + to_string( symbolId )
					+ " not found." );
}

void SymbolBar::setSymbolSelected( SymbolId symbolId ) {
	//unselect previous Symbol:
	SymbolButton *btn = getSymbolButton( _selectedSymbol );
	btn->value( 0 );
	//select desired Symbol:
	btn = getSymbolButton( symbolId );
	btn->value( 1 );
	_selectedSymbol = symbolId;
}

SymbolId SymbolBar::getSelectedSymbol() const {
	return _selectedSymbol;
}

void SymbolBar::enableSymbol( SymbolId id, bool enable ) {
	SymbolButton *btn = getSymbolButton( id );
	if ( enable ) {
		btn->activate();
	} else {
		btn->deactivate();
	}
}

void SymbolBar::hideSymbol( SymbolId id ) {
	SymbolButton *btn = getSymbolButton( id );
	btn->hide();

	for ( int i = id + 1, imax = _symbolIds.size(); i < imax; i++ ) {
		SymbolButton *next = getSymbolButton( _symbolIds.at( i ) );
		int x = next->x();
		x -= (btn->w()); // + SymbolBar::SPACING);
		next->position( x, next->y() );
	}

	int W = w();
	W -= (btn->w() + SymbolBar::SPACING);
	//size(W, h());
}

void SymbolBar::addButton( int x, int y, SymbolId id, SymbolClickCallback *cb,
		void *data ) {
	begin();
	//SymbolButton *b = new SymbolButton( 0, 0, (SymbolId)i, symbolpixmap[i]);
	SymbolButton *b = new SymbolButton( x, y, id, symbol_svg[ id ] );
	const char *t = symboltooltip[ id ];
	b->tooltip( t );
	b->callback( staticOnSymbolClick, this );
	end();
}


//*********************************************************
void onShowSymbolDialog( Fl_Widget* ) {
	DecisionDialog *dlg = new DecisionDialog( 100, 100 );
	//SymbolDialog* dlg = new SymbolDialog( 100, 100 );
	dlg->setSymbolText(
			"this is the ingenious data storage.\nSomewhere in the cloud." );
	//dlg->setSymbolSelected(SymbolId::DATA_STORAGE);
	int rc = dlg->show( false );
	fprintf( stderr, "closed dialog with rc = %d\n", rc );
	if ( rc > 0 ) {
//		fprintf(stderr, "selected symbol: %d\n", dlg->getSelectedSymbol());
//		fprintf(stderr, "connect next symbol with YES: %s\n",
//				dlg->connectNextSymbolWithYesBranche() ? "y" : "n");
	}
	delete dlg;

}

int testSymbolStuff() {
	Fl_Double_Window *pWin = new Fl_Double_Window( 100, 100, 800, 700,
			"Flowchart Tests" );
	Fl_Button *btn = new Fl_Button( 20, 20, 190, 25, "Show SymbolBaseDialog" );
	btn->callback( onShowSymbolDialog );
	pWin->end();
	pWin->show();

	return Fl::run();
}

