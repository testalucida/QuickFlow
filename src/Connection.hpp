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
#include "SymbolBox.hpp"
#include <stdexcept>

enum Compass {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

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
	 * drawn at an appropriate position near SymbolBox nearer.
	 */
	void setLabel( const char* lbl, SymbolBox* nearer ) {
		_label = lbl;
		_nearerBox = nearer;
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

//		fprintf( stderr, "line created from %d/%d to %d/%d. m=%f, b=%f\n", c1x, c1y,
//				c2x, c2y, line.m(), line.b() );
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
		//get the glue point for this line:
		_box1->getGluePoint( line, x1, y1 );
		_box2->getGluePoint( line, x2, y2 );

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

	/** draws this connection's label */
	void drawLabel( const Line& conn ) const {
		if( _label.empty() ) return;
		fprintf( stderr, "m = %f\n", conn.m() );
		fl_color( _labelcolor );
		fl_font( _font, _fontsize );
		double angle = conn.getGradientAngle() * (-1);
		if( conn.isVertical() ) {
			angle = 0.0;
		}

		if( !_nearerBox ) {
			//draw somewhere in the middle between the two connected boxes
			Point c = conn.getCenter();
			//fprintf( stderr, "gradient: %f, angle: %f\n", line.m(), angle );
			fl_draw( angle, _label.c_str(), c.x, c.y );
		} else {
			IntersectionPtr X;
			//calculate intersection between
			//connection and the nearerBox' border
			//and draw a little bit outside of nearerBox.

			//Where does this connection intersects nearerBox?
			SymbolBox* other = (_nearerBox == _box1 ) ? _box2 : _box1;
			int xc, yc, xoc, yoc;
			_nearerBox->getCenter( xc, yc );
			other->getCenter( xoc, yoc );
			Compass cmp = Compass::NORTH;

			if( xoc == xc ) {
				//conn is vertical
				if( yoc > yc ) {
					cmp = Compass::SOUTH;
				}
			} else {
				if( xoc > xc ) { //east or south
					if( isBetween( conn.m(), -1, 1 ) ) {
						//connection intersects nearerBox in the east
						cmp = Compass::EAST;
					} else if( conn.m() > 0 ) {
						cmp = Compass::SOUTH;
					}
				} else { //west or south
					//xoc <= xc
					if( isBetween( conn.m(), -1, 1 ) ) {
						cmp = Compass::WEST;
					} else if( conn.m() < -1 ) {
						cmp = Compass::SOUTH;
					}
				}
			}
			X = getIntersection( conn, _nearerBox, cmp );
//			fl_draw( angle, _label.c_str(), X->x, X->y );
			fl_draw( _label.c_str(), X->x, X->y );
		}
	}

	inline bool isBetween( float q, float a, float b ) const {
		return ( q >= a && q <= b ) or ( q >= b && q <= a );
	}

	/** get the intersection between this connection and the given box */
	inline IntersectionPtr getIntersection( const Line& line, const SymbolBox* box,
			                      Compass cmp ) const
	{
		LinePtr border;
		switch( cmp ) {
		case Compass::NORTH:
			border = box->getNorthBorder();
			break;
		case Compass::EAST:
			border = box->getEastBorder();
			break;
		case Compass::SOUTH:
			border = box->getSouthBorder();
			break;
		case Compass::WEST:
			border = box->getWestBorder();
			break;
		default:
			throw std::runtime_error( "Connection::getIntersection(): unknown Compass" );
		}
		IntersectionPtr x = line.getIntersection( *border );
		return x;
	}

private:
	//Fl_Box* _labelBox;
	SymbolBox* _box1;
	SymbolBox* _box2;
	std::vector<Point*> _points;
	std::string _label;
	SymbolBox* _nearerBox = NULL;
	Fl_Color _labelcolor = FL_BLUE;
	Fl_Font _font = FL_HELVETICA_ITALIC;
	Fl_Fontsize _fontsize = 10;
	Fl_Color _color = FL_DARK2;
	int _w = 3;
	bool _selected = false;
};


#endif /* CONNECTION_HPP_ */
