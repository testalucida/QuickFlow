/*
 * symbolbox_derivates.hpp
 *
 *  Created on: 12.05.2020
 *      Author: martin
 */

#ifndef SYMBOLBOX_DERIVATES_HPP_
#define SYMBOLBOX_DERIVATES_HPP_

#include "SymbolBox.hpp"
#include "glDrawing.hpp"
#include <FL/Fl_Group.H>
#include <fltk_ext/TextMeasure.h>
#include "defaultsymbolsizes.h"
#include <memory>
#include <cmath>

class SymbolDefaults {
public:
	static Size getDefaultSize(SymbolId id) {
		return defaultSymbolSizes[id];
	}
};

struct Circle {
	//Points the circle was created with
	double ax = 0; //x value of intersection arc with box' left border
	double ay = 0; //y value of intersection arc with box' left border
	double bx = 0; //x value of the box' center
	double by = 0; //y value of the box' top border
	//center
	double mx = 0;
	double my = 0;
	//radius
	double r = 0;
};

typedef std::unique_ptr<Circle> CirclePtr;

/////////////////////////   Start   ///////////////////////////

class Start : public SymbolBox {
public:
	Start(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::START;}
	virtual FlxRect getLabelRect() const { return {x(), y(), w(), h()};}
	~Start() {}
	void drawSymbol() {
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
	virtual void drawLabel() const {}

protected:
	virtual const char* getText() const { return "Start"; }

};

//////////////////////////   End  ////////////////////////

class End : public Start {
public:
	End( int x, int y, int w = 0, int h = 0 ) : Start( x, y, w, h ) {}
	~End() {}
	virtual SymbolId getSymbolId() const {return SymbolId::END;}
	virtual FlxRect getLabelRect() const { return {x(), y(), w(), h()};}
protected:
	virtual const char* getText() const { return "End"; }
};

///////////////////////   Decision   /////////////////////

class Decision : public SymbolBox {
public:
	Decision(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::DECISION;}

	virtual FlxRect getLabelRect() const {
		FlxRect rect;
		rect.x = x() + w()/3;
		rect.y = y() + h()/3;
		rect.w = w()/3;
		rect.h = h()/3;
		return rect;
	}

	~Decision() {}
protected:
	virtual void drawSymbol() {
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

	virtual void drawLabel() const {
		FlxRect rect = getLabelRect();
			string& split = getTextSplitter().split( _label.c_str(), rect.w, rect.h );
			Fl_Color old = fl_color();
			fl_color( _color );
			fl_font( _font, _fontsize );
			fl_draw( split.c_str(), rect.x, rect.y, rect.w, rect.h, FL_ALIGN_CENTER, NULL, 0 );

			fl_color( old );
	}

};

/////////////////////  Process  ///////////////////////

class Process : public SymbolBox {
public:
	Process(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::PROCESS;}

	virtual FlxRect getLabelRect() const {
		return {x() + 3, y() + 3, w() - 6, h() - 6};
	}

	~Process() {}

protected:
	virtual void drawSymbol() {
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

	virtual void drawLabel() const {

	}

};

/////////////////////  Data Storage  ///////////////////////

class DataStorage : public SymbolBox {
public:
	DataStorage(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h) {
	}

	virtual SymbolId getSymbolId() const {return SymbolId::DATA_STORAGE;}

	virtual Size getMinSize() const {
		//return defaultSymbolSizes[getSymbolId()];
		return {30, 20};
	}

	/**
	 * Gets the glue point of this DataStorage symbol
	 * for a given line (the connection)
	 */
	virtual void getGluePoint( const Line& line, int& x, int& y ) const {
		//first check roughly which part of the the symbolbox is hit by line
		IntersectionBorderPtr intersectborder = getIntersectionBorder( line );
		if( !intersectborder->intersect ) {
			throw runtime_error( "DataStorage::getGluePoint():\n"
								 "given line doesn't intersect.");
		}

		switch( intersectborder->border ) {
		case Compass::NORTH: {
			getCircleIntersection( x, y, line, _topReverseBowl->mx,
											   _topReverseBowl->my,
											   _topReverseBowl->r );
			return;
		}
		case Compass::EAST:
		case Compass::WEST:
			if( intersectborder->y <= _line_top_y ) { //this->y() + _party ) {
				getCircleIntersection( x, y, line, _topReverseBowl->mx,
												   _topReverseBowl->my,
												   _topReverseBowl->r  );
			} else if( intersectborder->y >= _line_bottom_y ) {
				getCircleIntersection( x, y, line, _topReverseBowl->mx,
												   _bottombowl_y,
												   _topReverseBowl->r  );
			} else {
				x = intersectborder->x;
				y = intersectborder->y;
			}
			return;
		case Compass::SOUTH:
			getCircleIntersection( x, y, line, _topReverseBowl->mx,
											   _bottombowl_y,
											   _topReverseBowl->r );
			return;
		default:
			throw runtime_error( "DataStorage::getGluePoint():\n"
								 "No border intersection detected." );
		}
	}

	~DataStorage() {}

protected:
	virtual void drawSymbol() {
		fl_push_clip( x(), y()-1, w(), h()+2 );
		gl_start();
		gl_color( _bordercolor );
		drawDataStorage();
		gl_finish();
		fl_pop_clip();
	}

	inline void drawDataStorage() {
		_topReverseBowl = getCircle();
//		fprintf( stderr, "CIRCLE: ax = %f, ay = %f, "
//				         "mx = %f, my = %f\n",
//						 circle->ax, circle->ay,
//				         circle->mx, circle->my );
		float startAngle = 0;
		float angle = _glDrawing.getRadiantFromDegree( 180 );
		int numSeg = _glDrawing.getNumCircleSegments( _topReverseBowl->r );
		//top semicircle opening southwards (reverse bowl):
//		fprintf( stderr, "drawing black reverse bowl: my(fltk): %f, "
//						                             "my(gl): %f\n",
//													 circle->my,
//													 glY( circle->my ) );
		_glDrawing.drawArcFast( _topReverseBowl->mx,
								glY( _topReverseBowl->my ), _topReverseBowl->r,
								startAngle, angle, numSeg, 2 );

		//top semicircle opening northwards (bowl):
		float ay = h()/_party * 2;
//		fprintf( stderr, "ay = %f\n", ay );
		float my = _topReverseBowl->my - 2*_topReverseBowl->r + ay;
//		fprintf( stderr, "drawing blue bowl: my(fltk): %f, "
//				                            "my(gl): %f\n", my, glY( my ) );
		startAngle = _glDrawing.getRadiantFromDegree( 180 );
//		gl_color( FL_BLUE );
		_glDrawing.drawArcFast( _topReverseBowl->mx,
								glY( my ), _topReverseBowl->r,
								startAngle, angle, numSeg, 2 );

		//bottom semicircle opening northwards
		_bottombowl_y = my;
		_bottombowl_y += ( h() - ay );
//		fprintf( stderr, "drawing yellow bowl: my(fltk): %f, "
//				         "my(gl): %f\n", my, glY( my ) );
//		gl_color( FL_YELLOW );
		_glDrawing.drawArcFast( _topReverseBowl->mx,
								glY( _bottombowl_y ), _topReverseBowl->r,
								startAngle, angle, numSeg, 2 );

//		gl_color( FL_GREEN );
		float x1 = x() + 1; //circle->ax;
		_line_top_y = y() + ay/2;
		float x2 = x1 + w() - 2;
		_line_bottom_y = _line_top_y + h() - ay;
//		fprintf( stderr, "drawing straight from x1/y1 (fltk: %f/%f) resp. (gl: %f/%f) "
//						 "to x1/y2 (fltk: %f/%f) resp. (gl: %f/%f)\n",
//				         x1, y1, x1, glY(y1), x1, y2, x1, glY( y2 ) );
		_glDrawing.drawLineSegment( x1, glY( _line_top_y ), x1, glY( _line_bottom_y ), 2 );
		_glDrawing.drawLineSegment( x2, glY( _line_top_y ), x2, glY( _line_bottom_y ), 2 );

	}

	virtual void drawLabel() const {}

	virtual FlxRect getLabelRect() const {
		FlxRect rect = {x(), y(), 0, 0};
		return rect;
	}

private:
	CirclePtr getCircle() const {
		// construct a circle using 2 points of this box:
		// - center of top border
		// - a point on the left border with a vertical distance from
		// top border of 1/15 height of this box.
		// We call these 2 points A(ax/ay) (left border) and
		// B(bx, by) (top border).
		// We construct line AB and its perpendicular bisector S1M
		// where S1 is the center of AB and M(mx/my) is the center
		// of the circle.
		// Additionally we have to find the circle's radius.
		// Be aware the screen's coordinate system stands upside down,
		// so y is increasing on moving downwords.

		CirclePtr circle( new Circle );

		//1st: create line segment AB
		circle->ax = x();
		circle->ay = y() + h()/_party;
		circle->bx = circle->ax + w()/2;
		circle->by = y(); //+ 2;
		//get gradient m for line segment AB:
		double m = ( circle->by - circle->ay ) / ( circle->bx - circle->ax );

//		fprintf( stderr, "AM: A(%f/%f), B(%f/%f), m=%f\n", ax, ay, bx, by, m );

		//2nd: get center of AB (S1)
		double s1x = (circle->ax + circle->bx) / 2;
		double s1y = (circle->ay + circle->by) / 2;
//		fprintf( stderr, "S1(%f/%f)\n", s1x, s1y );

		//3rd: equation for perpendicular bisector (S1M):
		//     Condition: gradient of S1M * gradient of AB = -1
		//     Gradient of S1M be m2.
		double m2 = -1/m;
		//we know the x coord of M, because in this special case
		//it's the same as bx
		circle->mx = circle->bx;
		//get intersection of S1M with y axis
		//b2 = s1y - m2*s1x
		double b2 = s1y - s1x * m2;
		//get my:
		circle->my = m2 * circle->mx + b2;

//		fprintf( stderr, "S1M: S1(%f/%f), M(%f/%f), m=%f\n", s1x, s1y, circle->mx, circle->my, m2 );
		//get radius:
		circle->r = abs( circle->by - circle->my );

		return circle;
	}

	/**
	 * NOTE: DO NOT USE FOR GENERALLY COMPUTING THE INTERSECTION OF THIS BOX
	 * WITH A GIVEN CIRCLE.
	 * Calculates the intersection between a given line and this DataStorage symbol.
	 * Method assumes that there is at least one intersection and
	 * returns the intersection point between the center of the given circle and
	 * the center of the connected SymbolBox (that's why this method is special).
	 */
	void getCircleIntersection( int& x, int& y,
						        const Line& line,
			       	   	   	    float cx, float cy, float r ) const
	{
		CircleIntersectionsPtr intersect =
				line.getCircleIntersections( cx, cy, r );
		if ( intersect->numberOfIntersections < 1 ) {
			throw runtime_error(
					"DataStorage::getCircleIntersection():\n"
							"no circle intersection." );
		}
		x = round( intersect->x1 );
		y = round( intersect->y1 );
		if ( intersect->numberOfIntersections > 1 ) {
			//find out which intersection is between the center of the circle
			//and the center of the other connected SymbolBox

			if ( !(line.isLinePointInSegment( x, y )) ) {
				x = round( intersect->x2 );
				y = round( intersect->y2 );
			}
		}
		//			cerr << "***********************" << endl;
		//			fprintf( stderr, "connection from %d/%d to %d/%d\n", line.x1(), line.y1(), line.x2(), line.y2() );
		//			fprintf( stderr, "center of top reverse bowl: %f/%f\n", _topReverseBowl->mx, _topReverseBowl->my );
		//			fprintf( stderr, "intersection: %d/%d\n", x, y );
	}

private:
	glDrawing& _glDrawing = glDrawing::inst();
	int _party = 5;
	CirclePtr _topReverseBowl;
	float _bottombowl_y = 0;
	float _line_top_y = 0;
	float _line_bottom_y = 0;
};

#endif /* SYMBOLBOX_DERIVATES_HPP_ */
