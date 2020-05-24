/*
 * glDrawing.hpp
 *
 *  Created on: 24.05.2020
 *      Author: martin
 */

#ifndef GLDRAWING_HPP_
#define GLDRAWING_HPP_

#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <cmath>

#ifndef PI
# define PI 3.14159265358979323846
#endif

class glDrawing {
	glDrawing() {}
public:
	~glDrawing() {}

	static glDrawing& inst() {
		static glDrawing* _this = NULL;
		if( _this == NULL ) {
			_this = new glDrawing();
		}
		return *_this;
	}

	void drawLineSegment( float ax, float ay, float bx, float by,
			              float linewidth = 3 ) const
	{
		glLineWidth( linewidth );
		glBegin( GL_LINES );
		glVertex2f( ax, ay );
		glVertex2f( bx, by );
		glEnd();
	}

	void drawCircleFast( float cx, float cy, float r, int num_segments ) {
		//http://slabode.exofire.net/circle_draw.shtml
		float theta = 2 * 3.1415926 / float( num_segments );
		float tangential_factor = tanf( theta ); //calculate the tangential factor

		float radial_factor = cosf( theta ); //calculate the radial factor

		float x = r; //we start at angle = 0

		float y = 0;

		glBegin( GL_LINE_LOOP );
		for ( int ii = 0; ii < num_segments; ii++ ) {
			glVertex2f( x + cx, y + cy ); //output vertex

			//calculate the tangential vector
			//remember, the radial vector is (x, y)
			//to get the tangential vector we flip those coordinates and negate one of them

			float tx = -y;
			float ty = x;

			//add the tangential vector

			x += tx * tangential_factor;
			y += ty * tangential_factor;

			//correct using the radial factor

			x *= radial_factor;
			y *= radial_factor;
		}
		glEnd();
	}

	/**
	 * Draws an arc with center at cx/cy and radius r.
	 * start_angle and arc_angle must be passed as radiants.
	 * You can get the radiant of the desired angle by calling
	 * getRadiantFromDegree().
	 * E.g.: if you want to draw a quarter circle starting from north
	 *       ending in the east you may call getRadiantFromDegree( 0 ) and
	 *       getRadiantFromDegree( 90 ) and pass the returned radiants
	 *       to drawArcFast(..).
	 */
	void drawArcFast( float cx, float cy, float r,
					  float start_angle, float arc_angle,
					  int num_segments, float linewidth = 3 )
	{
		//http://slabode.exofire.net/circle_draw.shtml
		float theta = arc_angle / float( num_segments - 1 );//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
		float tangential_factor = tanf( theta );
		float radial_factor = cosf( theta );

		float x = r * cosf( start_angle );	//we now start at the start angle
		float y = r * sinf( start_angle );
		glLineWidth( linewidth );

		glBegin( GL_LINE_STRIP );//since the arc is not a closed curve, this is a strip now
		for ( int ii = 0; ii < num_segments; ii++ ) {
			glVertex2f( x + cx, y + cy );

			float tx = -y;
			float ty = x;

			x += tx * tangential_factor;
			y += ty * tangential_factor;

			x *= radial_factor;
			y *= radial_factor;
		}
		glEnd();
	}

	void drawCircleByRepeatedRotation( float cx, float cy, float r,
			int num_segments ) {
		float theta = 2 * 3.1415926 / float( num_segments );
		float c = cosf( theta );	//precalculate the sine and cosine
		float s = sinf( theta );
		float t;

		float x = r;	//we start at angle = 0
		float y = 0;

		glBegin( GL_LINE_LOOP );
		for ( int ii = 0; ii < num_segments; ii++ ) {
			glVertex2f( x + cx, y + cy );	//output vertex

			//apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
		glEnd();
	}

	inline float getRadiantFromDegree( int degree ) const {
		return degree * PI / 180;
	}

	/*
	 * good guess to get the number of segments based on the radius
	 * in order to receive a smooth circle when calling the circle (arc)
	 * drawing functions.
	 */
	int getNumCircleSegments( float r ) {
		//http://slabode.exofire.net/circle_draw.shtml
		return 10 * sqrtf( r );	//change the 10 to a smaller/bigger number as needed
	}
};

#endif /* GLDRAWING_HPP_ */
