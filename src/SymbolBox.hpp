/*
 * SymbolBox.hpp
 *
 *  Created on: 12.05.2020
 *      Author: martin
 */

#ifndef SYMBOLBOX_HPP_
#define SYMBOLBOX_HPP_

#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <fltk_ext/DragBox.h>
#include <fltk_ext/Line.h>
#include <fltk_ext/TextSplitter.h>
#include <fltk_ext/TextMeasure.h>
#include <fltk_ext/FlxRect.h>
#include "enums.h"
#include "std.h"
#include "defaultsymbolsizes.h"

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++
class SymbolBox;
typedef void (*MouseCallback)( SymbolBox*, int whichButton, void* );
typedef std::vector<SymbolId> SymbolIds;
//++++++++++++++++++++++++++++++++++++++++++

class SymbolBox : public DragBox {
public:
	SymbolBox( int x, int y, int w, int h ) : DragBox( x, y, w, h ) {
		box( FL_NO_BOX );
	}
	virtual ~SymbolBox() {}

	void registerMouseCallback( MouseCallback cb, void* data ) {
		_mouseCallback = cb;
			_mouseCallback_userdata = data;
	}

	/** called by FlowCharCanvas */
	virtual void drawSymbol() {}

	virtual SymbolId getSymbolId() const = 0;

	/** get border of symbol box as line object */
	LinePtr getNorthBorder() const {
		int Y = y();
		LinePtr line( new Line( x(), Y, x() + w(), Y ) );
		return line;
	}
	LinePtr getEastBorder() const {
		int X = x() + w();
		LinePtr line( new Line( X, y(), X, y() + h() ) );
		return line;
	}
	LinePtr getSouthBorder() const {
		int Y = y() + h();
		LinePtr line( new Line( x(), Y, x() + w(), Y ) );
		return line;
	}
	LinePtr getWestBorder() const {
		int X = x();
		LinePtr line( new Line( X, y(), X, y() + h() ) );
		return line;
	}

	/**
	 * Gets the glue point of this symbol for a given line.
	 * Default is the center of the surrounding box.
	 */
	virtual Point getGluePoint( const Line& line ) const {
		Point p;
		getCenter( p.x, p.y );
		return p;
	}

	/**
	 * Gets the glue point of this symbol for a given line.
	 * Default is the center of the surrounding box.
	 */
	virtual void getGluePoint( const Line& line, int& x, int& y ) const {
		getCenter( x, y );
	}

	//label
	void setLabel(const char* txt) {
		_label = txt;
	}

	const char* getLabel() const {return _label.c_str();}

	//border color
	void setBorderColor( Fl_Color color ) {
		_bordercolor = color;
	}

	Fl_Color getBorderColor() const {return _bordercolor;}

	//border width
	void setBorderWidth(int w) {
		_borderwidth = w;
	}

	int getBorderWidth() const {return _borderwidth;}

	//labelfont
	void setLabelFont( Fl_Font font ) {
		_labelfont = font;
	}

	Fl_Font getLabelFont() const {return _labelfont;}

	//labelfontsize
	void setLabelFontsize( Fl_Fontsize h ) {
		_labelfontsize = h;
	}

	Fl_Fontsize getLabelFontsize() const {return _labelfontsize;}

	virtual Size getMinSize() const {
		return {20, 20};
	}

	void resize( int x, int y, int w, int h ) {
		//float xyratio = getXYratio();

		Size sz = getMinSize();
		w = w < sz.w ? sz.w : w;
		h = h < sz.h ? sz.h : h;
		DragBox::resize( x, y, w, h );
	}

protected:
	struct IntersectionBorder {
		Compass border = Compass::NORTH;
		int x = 0;
		int y = 0;
		bool intersect = true;
	};

	typedef std::unique_ptr<IntersectionBorder> IntersectionBorderPtr;

	/**
	 * Checks which border the given line is intersecting and returns
	 * the concerned border and the intersection point.
	 * If line doesn't intersect this symbolbox IntersectionBorder::intersect is set
	 * to false.
	 */
	IntersectionBorderPtr getIntersectionBorder( const Line& line ) const {
		IntersectionBorderPtr intersectborder( new IntersectionBorder );
		LinePtr border = getNorthBorder();
		IntersectionPtr X = border->getIntersection( line );
		if( X->intersects && X->withinSegments ) {
			intersectborder->border = Compass::NORTH;
			intersectborder->x = X->x;
			intersectborder->y = X->y;
			return intersectborder;
		}

		border = getEastBorder();
		X = border->getIntersection( line );
		if( X->intersects && X->withinSegments ) {
			intersectborder->border = Compass::EAST;
			intersectborder->x = X->x;
			intersectborder->y = X->y;
			return intersectborder;
		}

		border = getSouthBorder();
		X = border->getIntersection( line );
		if( X->intersects && X->withinSegments ) {
			intersectborder->border = Compass::SOUTH;
			intersectborder->x = X->x;
			intersectborder->y = X->y;
			return intersectborder;
		}

		border = getWestBorder();
		X = border->getIntersection( line );
		if( X->intersects && X->withinSegments ) {
			intersectborder->border = Compass::WEST;
			intersectborder->x = X->x;
			intersectborder->y = X->y;
			return intersectborder;
		}

		intersectborder->intersect = false;
		return intersectborder;
	}

	/** get the Rectangle to draw the label text in. */
	virtual FlxRect getLabelRect() const = 0;

	/** get the one and only instance of TextSplitter */
	static inline TextSplitter& getTextSplitter() {
		static TextSplitter* _textSplitter = NULL;
		if( _textSplitter == NULL ) _textSplitter = new TextSplitter(1, 10);
		return *_textSplitter;
	}

	/** draw the label */
	virtual void drawLabel() const = 0;

	 /* called by dragbox on PUSH event with right mouse pressed. */
	virtual void onRightMouse( bool isBoxSelected ) {
		if ( _mouseCallback ) {
				(_mouseCallback)( this, FL_RIGHT_MOUSE, _mouseCallback_userdata );
			}
	}

	virtual void onMovedOrResized( bool resized, int delta_x, int delta_y ) {
	}

	inline float glY( int fltkY ) const {
		return this->top_window()->h() - fltkY;
	}

	virtual inline float getXYratio() {
		if( _xyratio == 0 ) {
			Size sz = defaultSymbolSizes[getSymbolId()];
			_xyratio = (float)sz.w / (float)sz.h;
		}
		return _xyratio;
	}

protected:
	//int handle(int e);
	Fl_Color _bordercolor = FL_DARK1;
	int _borderwidth = 3;
	Fl_Font _labelfont = FL_HELVETICA;
	Fl_Fontsize _labelfontsize = 13;
	std::string _label;
	Fl_Font _font = FL_HELVETICA;
	Fl_Fontsize _fontsize = 10;
	Fl_Color _color = FL_BLACK;
	MouseCallback _mouseCallback = NULL;
	void* _mouseCallback_userdata = NULL;
private:
	//std::vector<Connection*> _connections;
	float _xyratio = 0;
};

#endif /* SYMBOLBOX_HPP_ */
