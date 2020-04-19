/*
 * CanvasController.cpp
 *
 *  Created on: 04.04.2020
 *      Author: martin
 */

#include "CanvasController.h"

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_ask.H>
#include "flowchartui.h"
#include "std.h"
#include <fltk_ext/Canvas.h>
#include <fltk_ext/DragBox.h>

using namespace std;

CanvasController::CanvasController( FlowChartCanvas *p ) :
		_pCanvas( p ) {
	_pCanvas->registerPushCallback( onCanvasPush_static, this );
}

CanvasController::~CanvasController() {
	// TODO Auto-generated destructor stub
}

void CanvasController::onSymbolBoxPush_static( SymbolBox *box, int whichButton,
		void *userdata ) {
	CanvasController *pThis = (CanvasController*) userdata;
	pThis->onSymbolBoxPush( box, whichButton );
}

void CanvasController::onSymbolBoxPush( SymbolBox *box, int whichButton ) {
	fprintf( stderr, "CanvasController::onSymbolBoxPush\n" );
	if ( whichButton == FL_RIGHT_MOUSE ) {
		//show popup menu.
		_symbolbox_popup = new Fl_Menu_Button( box->x(), box->y(), 300, 100 );
		_symbolbox_popup->type( Fl_Menu_Button::POPUP3 );

		//if two SymbolBoxes are selected, activate MenuItem "Connect".
		vector<DragBox*> dragboxes = _pCanvas->getSelectedDragBoxes();
		if ( dragboxes.size() == 2 ) {
			_symbolbox_popup->add( "Connect", 0 /*shortcut*/,
					NULL /*Fl_Callback* */, NULL );
		}

		_symbolbox_popup->add( "Delete", "^d", NULL, NULL );
		const Fl_Menu_Item *mi = _symbolbox_popup->popup();
		if ( mi ) {
			fprintf( stderr, "text: %s\n", mi->label() );
			if ( !strcmp( mi->label(), "Connect" ) ) {
				connect( (SymbolBox*) dragboxes[ 0 ],
						(SymbolBox*) dragboxes[ 1 ] );
			} else if ( !strcmp( mi->label(), "Delete" ) ) {
				for ( auto b : dragboxes ) {
					destroy( (SymbolBox*) b );
				}
			}
		}
		delete _symbolbox_popup;
		_symbolbox_popup = NULL;
	}
}

void CanvasController::onCanvasPush_static( Fl_Widget *w, void *pThis ) {
	CanvasController *cc = (CanvasController*) pThis;
	cc->onCanvasPush( w );
}

void CanvasController::onCanvasPush( Fl_Widget *w ) {
	if ( w == _pCanvas ) {
		if ( (!_isSymbolDialogOpen) && _symbolId != SymbolId::NONE ) {
			int x = Fl::event_x();
			int y = Fl::event_y();

			// 1st: insert new symbol "A"

			SymbolBox* pSymbol = addSymbol( _symbolId, x, y );

			x = _pCanvas->parent()->x() + _pCanvas->x();
			y = _pCanvas->parent()->y() + _pCanvas->y();
			SymbolBaseDialog *pDlg;
			if ( _symbolId == SymbolId::DECISION ) {
				pDlg = new DecisionDialog( x, y );
			} else {
				//open default dialog
				pDlg = new SymbolDialog( x, y );
			}
			_isSymbolDialogOpen = true;
			int rc = pDlg->show( false );
			// in the dialog:
			// 2nd: enter new symbol's label
			// 3rd: choose next symbol "B" to insert
			// 4th: enter label for connection between A and B
			// 5th: click OK
			if( rc ) {
				// give symbol "A" its label:
				const char* label = pDlg->getSymbolText();
				if( label ) {
					setLabel( pSymbol, label );
				}
				// create next symbol "B"
				SymbolId next = pDlg->getSelectedSymbol();
				if( next != SymbolId::NONE ) {
					//todo: find free space for next symbol
					addSymbol( next, 100, 100 );
				}

				// create connection and give it its label


			}
			_isSymbolDialogOpen = false;

		}
	}
}

void CanvasController::setLabel( SymbolBox* box, const char* lbl ) {
	box->setLabel( lbl );
	//todo: log
}

SymbolBox* CanvasController::addSymbol( SymbolId id, int x, int y ) {
	SymbolBox *pSymbol = SymbolFactory::inst().create( _symbolId, x, y );
	addSymbol( pSymbol );
	return pSymbol;
}

void CanvasController::addSymbol( SymbolBox* box ) {
	box->registerMouseCallback( onSymbolBoxPush_static, this );
	_pCanvas->add( box );
}

void CanvasController::onSymbolSelected( SymbolId id ) {
	//fprintf(stderr, "CanvasController::onSymbolSelected\n");
	_symbolId = id;
}

void CanvasController::connect( SymbolBox *box1, SymbolBox *box2 ) {
	_pCanvas->createConnection( box1, box2 );
	_pCanvas->redraw();
}

void CanvasController::destroy( SymbolBox *b ) {
	_pCanvas->removeConnections( b );
	_pCanvas->remove( b );
	_pCanvas->redraw();
}

bool CanvasController::openChanges() const {
	return true;
}

void CanvasController::onQuitCallback_static( Fl_Widget*, void* data ) {
	CanvasController* pThis = (CanvasController*) data;
	pThis->onQuitCallback();
}

void CanvasController::onQuitCallback() {
	if( openChanges() ) {
		const char* msg = "There are unsaved changes.\n"
				"Do you want to save before leaving?";

		int rc = fl_choice( msg, "No ", "Yes", NULL );

		if( !rc ) exit( 0 );
	}
}











