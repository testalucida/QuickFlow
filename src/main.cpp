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


int main() {
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
//	ctrl.addSymbol( dec1 );
//	Decision* dec2 = new Decision( 200, 150, 80, 60 );
//	ctrl.addSymbol( dec2 );
//	canvas->createConnection( dec1, dec2 );
	pWin->show();

	return Fl::run();
}
