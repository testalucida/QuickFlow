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

//SymbolButton::SymbolButton(int x, int y, SymbolId symbolId, const char** pXpm) :
//		Fl_Toggle_Button( x, y, SymbolButton::getButtonWidth(), SymbolButton::getButtonWidth())
//{
//	_symbolId = symbolId;
//	clear_visible_focus();
//	box(FL_FLAT_BOX);
//	color(FL_LIGHT2);
//	down_box(FL_DOWN_BOX);
//	down_color(FL_YELLOW);
//	Fl_Pixmap* p = new Fl_Pixmap(pXpm);
//	image(p);
//}

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

void SymbolBox::registerMouseCallback( MouseCallback cb, void *data ) {
	_mouseCallback = cb;
	_mouseCallback_userdata = data;
}

void SymbolBox::onRightMouse( bool isBoxSelected ) {
	if ( _mouseCallback ) {
		(_mouseCallback)( this, FL_RIGHT_MOUSE, _mouseCallback_userdata );
	}
}

void SymbolBox::onMovedOrResized( bool resized, int delta_x, int delta_y ) {
	//fprintf( stderr, "onMovedOrResized: %d, %d\n", delta_x, delta_y );
}

void SymbolBox::drawSymbol() {
	/** it is important to fill the inside of the symbol
	 * with canvas' color. Otherwise the connections drawn
	 * from one box' center to the other box' center can be seen
	 * within the symbols.
	 * An example is to be seen below.
	 */
//	Fl_Color memo = fl_color();
//	fl_color( FL_DARK2 );
//	fl_line_style( FL_SOLID, 3 );
//	fl_push_clip(x(),y(),w(),h());
//	fl_push_matrix();
//
//	int x1 = x() + 3;
//	int y1 = y() + 3;
//	int x2 = x() + w() - 3;
//	int y2 = y() + h() - 3;
//
//	fl_begin_complex_polygon();
//
//	//draw filled rectangle
//	fl_vertex( (float)x1, (float)y1 );
//	fl_vertex( (float)x2, (float)y1 );
//	fl_vertex( (float)x2, (float)y2 );
//	fl_vertex( (float)x1, (float)y2 );
//	fl_vertex( (float)x1, (float)y1 );
//
//	fl_end_complex_polygon();
//
//	//draw filled smaller rectangle to see an outline
//	x1 += 3;
//	y1 += 3;
//	x2 -= 3;
//	y2 -= 3;
//
//	fl_color( FL_RED );
//	fl_begin_complex_polygon();
//
//	fl_vertex( (float)x1, (float)y1 );
//	fl_vertex( (float)x2, (float)y1 );
//	fl_vertex( (float)x2, (float)y2 );
//	fl_vertex( (float)x1, (float)y2 );
//	fl_vertex( (float)x1, (float)y1 );
//
//	fl_end_complex_polygon();
//
//	fl_pop_matrix();
//	fl_pop_clip();
//	fl_line_style( 0 );
//	fl_color( memo );
}

void SymbolBox::addConnection( Connection *c ) {
	_connections.push_back( c );
}

void SymbolBox::setLabel( const char *txt ) {
	//_label->value( txt );
	_label = txt;
}

void SymbolBox::setBorderColor( Fl_Color color ) {
	_bordercolor = color;
	//redraw();
}

void SymbolBox::setBorderWidth( int w ) {
	_borderwidth = w;
}

void SymbolBox::setLabelFont( Fl_Font font ) {
	_labelfont = font;
}

void SymbolBox::setLabelFontsize( Fl_Fontsize h ) {
	_labelfontsize = h;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Start::drawSymbol() {
	int margin = 1;
	int x = this->x() + margin;
	int y = this->y() + margin;
	int w = this->w() - 2 * margin;
	int h = this->h() - 2 * margin;

	fl_rectf( x+10, y+2, w-20, h-2, FL_WHITE );

	fl_color( _bordercolor );
	fl_line_style( FL_SOLID, _borderwidth, NULL );



	int pie_w = 30;
	fl_pie( x, y, pie_w, h, 90, 270 );

	//fl_color(FL_YELLOW);
	int x2 = x + w - 15;
	fl_line( x + 15, y, x2, y );

	int y2 = y + h;
	fl_line( x + 15, y2, x2, y2 );

	//fl_color( _bordercolor );
	fl_pie( x + w - pie_w, y, pie_w, h, -90, 90 );

	fl_draw( getText(), x + 30, y, w - 2 * pie_w, h, FL_ALIGN_CENTER );

	fl_line_style( 0 );
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

//void Decision::drawSymbol() {
//	fl_color( _bordercolor );
//	fl_line_style( FL_SOLID, _borderwidth, NULL );
//
//	int x = this->x();
//	int y = this->y();
//	int w = this->w();
//	int h = this->h();
//
//	fl_loop( x + w / 2, y, x + w, y + h / 2, x + w / 2, y + h, x, y + h / 2 );
//
//	fl_line_style( 0 );
//}

void Decision::drawSymbol() {
	Fl_Color memo = fl_color();
	fl_color( _bordercolor );
	fl_line_style( FL_SOLID, _borderwidth, NULL );

	fl_push_clip(x(),y(),w(),h());
	fl_push_matrix();

	//rhombus' vertices:
	int x1 = this->x() + w()/2;
	int y1 = this->y();
	int x2 = x() + w();
	int y2 = y() + h()/2;
	int x3 = x() + w()/2;
	int y3 = y() + h();
	int x4 = x();
	int y4 = y() + h()/2;

	//draw filled rhombus
	fl_begin_polygon();
	fl_vertex( (float)x1, (float)y1 );
	fl_vertex( (float)x2, (float)y2 );
	fl_vertex( (float)x3, (float)y3 );
	fl_vertex( (float)x4, (float)y4 );
	fl_vertex( (float)x1, (float)y1 );
	fl_end_polygon();

	//draw filled smaller rhombus to see an outline
	int w = _borderwidth;
	y1 += w;
	x2 -= w;
	y3 -= w;
	x4 += w;

	fl_color( parent()->color() );
	fl_begin_polygon();
	fl_vertex( (float)x1, (float)y1 );
	fl_vertex( (float)x2, (float)y2 );
	fl_vertex( (float)x3, (float)y3 );
	fl_vertex( (float)x4, (float)y4 );
	fl_vertex( (float)x1, (float)y1 );
	fl_end_polygon();

	fl_pop_matrix();
	fl_pop_clip();
	fl_line_style( 0 );
	fl_color( memo );

	fl_line_style( 0 );

	drawLabel();
}

void Decision::drawLabel() const {
	FlxRect rect = getLabelRect();
	string& split = getTextSplitter().split( _label.c_str(), rect.w, rect.h );
	Fl_Color old = fl_color();
	fl_color( _color );
	fl_font( _font, _fontsize );
	fl_draw( split.c_str(), rect.x, rect.y, rect.w, rect.h, FL_ALIGN_CENTER, NULL, 0 );

	fl_color( old );
}

FlxRect Decision::getLabelRect() const {
	FlxRect rect;
	rect.x = x() + w()/3;
	rect.y = y() + h()/3;
	rect.w = w()/3;
	rect.h = h()/3;
	return rect;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Process::drawSymbol() {
	fl_color( _bordercolor );
	fl_line_style( FL_SOLID, _borderwidth, NULL );

	int x1 = x() + 10;
	int y1 = y() + 10;
	int x2 = x1 + w() - 20;
	int y2 = y1 + h() - 20;

	fl_begin_polygon();
	fl_vertex( (float)x1, (float)y1 );
	fl_vertex( (float)x2, (float)y1 );
	fl_vertex( (float)x2, (float)y2 );
	fl_vertex( (float)x1, (float)y2 );
	fl_vertex( (float)x1, (float)y1 );
	fl_end_polygon();

	//draw a filled smaller rectangle:
	int w = _borderwidth;
	x1 += w;
	y1 += w;
	x2 -= w;
	y2 -= w;
	Fl_Color colr = ( box() == FL_NO_BOX ) ? parent()->color() : color();
	fl_color( colr );
	fl_begin_polygon();
	fl_vertex( (float)x1, (float)y1 );
	fl_vertex( (float)x2, (float)y1 );
	fl_vertex( (float)x2, (float)y2 );
	fl_vertex( (float)x1, (float)y2 );
	fl_vertex( (float)x1, (float)y1 );
	fl_end_polygon();

	fl_line_style( 0 );
}

FlxRect Process::getLabelRect() const {
	return {x() + 3, y() + 3, w() - 6, h() - 6};
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#include <FL/Fl_SVG_Image.H>
SymbolBox* SymbolFactory::create( SymbolId id, int x, int y, int w, int h,
		bool xy_is_center ) {
	//xy_is_center = true: create Symbol not at x, y but in such a way
	//that x, y is the middle  of the created Symbol.
	SymbolBox *pSymbol;
	switch ( id ) {
	case SymbolId::START:
		pSymbol = new Start( 0, 0, 0, 0 );
		break;
	case SymbolId::DECISION:
		pSymbol = new Decision( 0, 0, 0, 0 );
		break;
	case SymbolId::PROCESS:
		pSymbol = new Process( 0, 0, 0, 0 );
		break;
	case SymbolId::END:
		pSymbol = new End( 0, 0, 0, 0 );
		break;
	default:
		string msg = "SymbolFactory::create(): " + std::to_string( id );
		msg += ": unknown SymbolType.";
		throw runtime_error( msg );
	}

	setSizeAndPosition( pSymbol, x, y, w, h, xy_is_center );
	_symbols.push_back( pSymbol );

	return pSymbol;
}

void SymbolFactory::setSizeAndPosition( SymbolBox *pSymbol, int x, int y, int w,
		int h, bool xy_is_center ) {
	if ( w == 0 || h == 0 ) {
		Size size = SymbolDefaults::getDefaultSize( pSymbol->getSymbolId() );
		w = w == 0 ? size.w : w;
		h = h == 0 ? size.h : h;
	}

	if ( xy_is_center ) {
		x -= w / 2;
		y -= h / 2;
	}

	pSymbol->position( x, y );
	pSymbol->size( w, h );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Connection::Connection( SymbolBox *box1, SymbolBox *box2 ) :
		_box1( box1 ), _box2( box2 )
{
	_labelBox = new Fl_Box( 0, 0, 0, 0 );
}

Connection::~Connection() {
	for ( auto p : _points ) {
		delete p;
	}
}

void Connection::setLabel( const char* lbl ) {
	_labelBox->label( lbl );
	//todo: log
}


void Connection::addPoint( int x, int y ) {
	Point *p = new Point();
	p->x = x;
	p->y = y;
	_points.push_back( p );
}

void Connection::draw() {
	//don't try to draw any line when boxes are in contact:
	if ( boxesInTouch() )
		return;
	int x1, y1, x2, y2;
	_box1->getCenter( x1, y1 );
	_box2->getCenter( x2, y2 );
	Line line( x1, y1, x2, y2 ); //the line to draw

	fl_line_style( FL_SOLID, 3 );
	Fl_Color memo = fl_color();
	Fl_Color color = _selected ? FL_YELLOW : FL_DARK2;
	fl_color( color );
	fl_begin_line();
	fl_vertex( x1, y1 );
	fl_vertex( x2, y2 );
	fl_end_line();

	fl_line_style( 0 );
	fl_color( memo );

	drawLabel( line );
}

void Connection::drawLabel( Line& line ) const {

}

inline Intersection Connection::getIntersection( DragBox *box,
		Line &line ) const {
	//which sideline does line intersect?
	//get all 4 vertices of box:
	int x1 = box->x();
	int x2 = x1 + box->w();
	int y1 = box->y();
	int y2 = y1 + box->h();

	//intersects with top line?
	Line topline( x1, y1, x2, y1 );
	Intersection X = topline.getIntersection( line );
	if ( X.intersects && X.withinSegments ) {
		return X;
	} else {
		//right sideline?
		Line rightline( x2, y1, x2, y2 );
		X = rightline.getIntersection( line );
		if ( X.intersects && X.withinSegments ) {
			return X;
		} else {
			//bottom line?
			Line bottomline( x1, y2, x2, y2 );
			X = bottomline.getIntersection( line );
			if ( X.intersects && X.withinSegments ) {
				return X;
			} else {
				//left sideline?
				Line bottomline( x1, y1, x1, y2 );
				X = bottomline.getIntersection( line );
				return X;
			}
		}
	} //!X.intersects
}  //Intersection getIntersection( ...

bool Connection::isNearby( int x, int y ) const {
	fprintf( stderr, "checking point %d/%d\n", x, y );
	int c1x, c1y, c2x, c2y;
	_box1->getCenter( c1x, c1y );
	_box2->getCenter( c2x, c2y );
	Line line( c1x, c1y, c2x, c2y );

	fprintf( stderr, "line created from %d/%d to %d/%d. m=%f, b=%f\n", c1x, c1y,
			c2x, c2y, line.m(), line.b() );
	return (line.getDistance( x, y ) <= _w);
}

void Connection::setColor( Fl_Color color ) {
}

void Connection::setSelected( bool selected ) {
	_selected = selected;
}

void Connection::setWidth( int w ) {
	_w = w;
}

inline bool Connection::isElementOf( DragBox *box, int x, int y ) const {
	if ( x < box->x() || x > box->x() + box->w() )
		return false;
	if ( y < box->y() || y > box->y() + box->h() )
		return false;
	return true;
}

inline bool Connection::boxesInTouch() const {
	int x21 = _box2->x();
	int y21 = _box2->y();
	if ( isElementOf( _box1, x21, y21 ) )
		return true;
	int x22 = x21 + _box2->w();
	if ( isElementOf( _box1, x22, y21 ) )
		return true;
	int y22 = _box2->y() + _box2->h();
	if ( isElementOf( _box1, x22, y22 ) )
		return true;
	if ( isElementOf( _box1, x21, y22 ) )
		return true;
	//still here ==> _box2 is either completey contained by _box1
	//or they don't touch each other at all.
	//One additional check is enough:
	if ( isElementOf( _box2, _box1->x(), _box1->y() ) )
		return true;

	return false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//SymbolSettings::SymbolSettings() {
//	//todo: read properties from a file
//}
//
//SymbolSettings& SymbolSettings::inst() {
//	static SymbolSettings* pThis = NULL;
//	if( !pThis ) {
//		pThis = new SymbolSettings();
//	}
//	return *pThis;
//}

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


