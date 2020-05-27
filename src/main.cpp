//#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

#include <fltk_ext/Canvas.h>
#include <fltk_ext/DragBox.h>

#include "flowchartui.h"
#include "CanvasController.h"
#include "symbolbox_derivates.hpp"

#include <iostream>
using namespace std;

int test() {
	double d = 45.0;
	double m = tan( d );
	fprintf( stderr, "tangens of %f degrees makes a gradient of %f\n", d, m );

	double m2 = 1.732050808;
	double d2 = atan( m2 ) * 180 / PI;
	fprintf( stderr, "a gradient of %f corresponds to %f degrees\n", m2, d2 );
	//Line line( 0, 0, 0, 4);
	//fprintf( stderr, "gradient angle: %f\n", line.getGradientAngle() );
	return 1;
}

class TestBox : public Fl_Box {
public:
	TestBox( int x, int y, int w, int h ) : Fl_Box( x, y, w, h ) {
		box( FL_FLAT_BOX );
		color( FL_RED );
	}
protected:
	void draw() {
		Fl_Box::draw();
		gl_start();

		gl_color( FL_BLACK );
		float cx = x() + w()/2;
		float cy = 450; //y() + h()/2;
		float r = h()/2;
		float startAngle = _glDrawing.getRadiantFromDegree( 0 );
		float angle = _glDrawing.getRadiantFromDegree( 180 );
		int numSeg = _glDrawing.getNumCircleSegments( r );
		_glDrawing.drawArcFast( cx, cy, r, startAngle, angle, numSeg );

		gl_finish();
	}
private:
	glDrawing& _glDrawing = glDrawing::inst();
};

int main_() {
	Fl_Window* win = new Fl_Window( 200, 200, 500, 500 );
	win->box( FL_FLAT_BOX );
	win->color( FL_LIGHT3 );

	TestBox* ds = new TestBox( 0, 0, 100, 100 );

	win->end();

	Fl::gl_visual( FL_RGB );
	win->show();

	return Fl::run();
}


int intersect() {
	float cx = 5, cy = 5, r = 6;
	float _b = 8.22;
	float _m = -0.22;
	//circle equation (let x,y be the coords of the intersections and be aware that
	// there might be 2 intersections (x1,y1 and x2,y2))

	/*
	 * Circle:  (x - cx)² + (y - cy)² = r²
	 * Line  :  y = _m * x + _b
	 *
	 * Line in circle:
	 * (x - cx)² + (_m * x + _b - cy)² = r² */
	 float A = _b - cy;
	 /* Solve for x:
	 * x² - 2*cx*x + cx² + _m² * x² + 2*_m*x*A + A² = r²
	 * x² + _m²*x² - 2*cx*x + 2*_m*x*A + cx² + A² - r² = 0
	 * (1 + _m²)*x² - (2*cx + 2*_m*A)*x + (cx² + A² - r²) = 0
	 * use abc formula: */
	 float a = 1 + pow( _m, 2 );
	 float b = 2 * cx + 2 * _m * A;
	 float c = pow( cx, 2 ) + pow( A, 2 ) - pow( r, 2 );
	 //insert in formula
	 float powb = pow( b, 2 );
	 /*
	  * numerator: (-b + sqrt( powb - 4*a*c )
	  * denominator: 2*a */
	 float root = sqrt( powb - 4*a*c );
	 float denom = 2 * a;
	 b *= (-1);
	 float x1 = (b + root) / denom;
	 float x2 = (b - root) / denom;
}


int test2() {
	intersect( );
}

int main() {
	return test2();
	FlowChartMainWindow *pWin =
			new FlowChartMainWindow(200, 200, 500, 500 );

	pWin->box(FL_FLAT_BOX);
	pWin->color(FL_WHITE);
	FlowChartCanvas* canvas = pWin->getCanvas();
	canvas->begin();
	canvas->end();
	pWin->end();

	CanvasController ctrl( canvas );
	pWin->registerSymbolSelectedCallback(
			CanvasController::symbolSelectedCallback_static, &ctrl );
	pWin->selectSymbol( SymbolId::NONE );

	//ACTIVATE THE FOLLOWING LINE!
	//pWin->callback( CanvasController::onQuitCallback_static, &ctrl );

//	Decision* dec1 = new Decision( 100, 100, 70, 50 );
//	dec1->setLabel( "Is this a nice label?" );
//	ctrl.addSymbol( dec1 );
//	Decision* dec2 = new Decision( 200, 150, 80, 60 );
//	dec2->setLabel( "Proceed to the left?" );
//	ctrl.addSymbol( dec2 );
//	Connection* conn = canvas->createConnection( dec1, dec2 );
//	conn->setLabel("Yes", dec1);

//	DataStorage* ds = new DataStorage( 300, 200, 60, 60 );
//	ctrl.addSymbol( ds );

//	Process* proc1 = new Process( 250, 250, 80, 70 );
//	proc1->setLabel( "Executing process 1" );
//	ctrl.addSymbol( proc1 );
//	conn = canvas->createConnection( dec2, proc1 );
//	conn->setLabel( "Proceed to process 1", dec2 );

	Fl::gl_visual( FL_RGB );
	pWin->show();

	return Fl::run();
}
