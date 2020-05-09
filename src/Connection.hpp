/*
 * Connection.hpp
 *
 *  Created on: 09.05.2020
 *      Author: martin
 */

#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include "symbolstuff.h"

class Connection {
public:
	Connection( SymbolBox *box1, SymbolBox *box2 ) :
			_box1( box1 ), _box2( box2 )
	{
	}

	virtual ~Connection() {
		for ( auto p : _points ) {
			delete p;
		}
	}

	/** give the connection a label which will be
	 * drawn at appropriate position
	 */
	void setLabel( const char* lbl ) {
		_label = lbl;
		//todo: log
	}

	const char* getLabel() const { return _label.c_str(); }

	/**
	 * Checks if the given point (x, y) is "nearby"
	 * this Connection. Nearby means a distance less or equal
	 * the width of this Connection.
	 */
	bool isNearby( int x, int y ) const {
		fprintf( stderr, "checking point %d/%d\n", x, y );
		int c1x, c1y, c2x, c2y;
		_box1->getCenter( c1x, c1y );
		_box2->getCenter( c2x, c2y );
		Line line( c1x, c1y, c2x, c2y );

		fprintf( stderr, "line created from %d/%d to %d/%d. m=%f, b=%f\n", c1x, c1y,
				c2x, c2y, line.m(), line.b() );
		return (line.getDistance( x, y ) <= _w);
	}


	/**
	 * Per default, a straight line is drawn between the
	 * two boxes to connect.
	 * If an angled connection is wanted, an additional
	 * point can be added using this method.
	 */
	void addPoint( int x, int y ) {
		Point *p = new Point();
		p->x = x;
		p->y = y;
		_points.push_back( p );
	}

	void setSelected( bool selected ) {
		_selected = selected;
	}

	/**
	 * One of the two connected objects has moved.
	 * Adjust this connection line.
	 * @ box: the moved box
	 * @ delta_x: horizontal amount of moving
	 * @ delta_y: vertical amount of moving
	 */
	void draw() {
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

		drawLabel( line, x1, y1, x2, y2 );
	}

	void setColor( Fl_Color color ) {
	}

	void setWidth( int w ) {
		_w = w;
	}

	SymbolBox* box1() { return _box1; }
	SymbolBox* box2() { return _box2; }

private:

	inline bool isElementOf( DragBox *box, int x, int y ) const {
		if ( x < box->x() || x > box->x() + box->w() )
			return false;
		if ( y < box->y() || y > box->y() + box->h() )
			return false;
		return true;
	}

	inline bool boxesInTouch() const {
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

	inline Intersection getIntersection( DragBox *box,
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

	/** draws this connection's label */
	void drawLabel( Line& line, int x1, int y1, int x2, int y2 ) const {
		//calculate center of line
		int cx = (x1+x2)/2;
		int cy = (y1+y2)/2;
		//keep it simple: make it 50 px wide
		fl_color( FL_BLACK );
		fl_draw( _label.c_str(), cx, cy, 50, 50, FL_ALIGN_LEFT, NULL, 0 );
	}

private:
	//Fl_Box* _labelBox;
	SymbolBox* _box1;
	SymbolBox* _box2;
	std::vector<Point*> _points;
	std::string _label;
	Fl_Color _color = FL_DARK2;
	int _w = 3;
	bool _selected = false;
};


#endif /* CONNECTION_HPP_ */
