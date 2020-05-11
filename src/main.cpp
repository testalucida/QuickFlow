//#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
//#include <FL/Fl_Widget.H>
//#include <FL/Fl_Output.H>
//#include <FL/Enumerations.H>
//#include <FL/fl_draw.H>
//#include <FL/Fl_Input.H>
//#include <flx/appwin/AppWin.h>
//#include <FL/Fl_Tile.H>
//#include <FL/Fl_Box.H>

#include <fltk_ext/Canvas.h>
#include <fltk_ext/DragBox.h>

#include "flowchartui.h"
#include "CanvasController.h"

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

int main() {
	//return test();
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

	Decision* dec1 = new Decision( 100, 100, 70, 50 );
	dec1->setLabel( "Is this a nice label?" );
	ctrl.addSymbol( dec1 );
	Decision* dec2 = new Decision( 200, 150, 80, 60 );
	dec2->setLabel( "Proceed to the left?" );
	ctrl.addSymbol( dec2 );
	Connection* conn = canvas->createConnection( dec1, dec2 );
	conn->setLabel("Yes", dec1);
	pWin->show();

	return Fl::run();
}
