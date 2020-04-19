/*
 * symbolstuff.h
 *
 *  Created on: 25.03.2020
 *      Author: martin
 */

#ifndef SYMBOLSTUFF_H_
#define SYMBOLSTUFF_H_

#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>

#include <fltk_ext/DragBox.h>
#include <fltk_ext/TextMeasure.h>
#include <fltk_ext/Line.h>
#include "dialogs.h"

#include "std.h"

static Size defaultSymbolSizes[] = {
		{70, 30}, //Start
		{70, 70}, //Line
		{70, 70}, //Decision
		{120, 80}, //Process
		{70, 70}, //Input / Output
		{70, 70}, //Manual Input
		{70, 70}, //Document
		{70, 70}, //Manual Op
		{70, 70}, //DataStorage
		{70, 70}, //Connector
		{70, 30}, //End
		{70, 70}  //Unselect symbol
};

typedef void (SymbolClickCallback)(SymbolId, void*);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SymbolButton : public Fl_Toggle_Button {
public:
	SymbolButton(int x, int y, SymbolId symbolId, const char** pXpm);
	SymbolButton(int x, int y, SymbolId symbolId, const char* svg_file);
	static int getButtonWidth() {
		return 30;
	}

	SymbolId getSymbolId() const {
		return _symbolId;
	}

private:
	SymbolId _symbolId;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef std::vector<SymbolId> SymbolIds;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SymbolBar : public Fl_Group {
public:
	SymbolBar(int x, int y, SymbolClickCallback* cb = NULL, void* data = NULL);
	SymbolBar(int x, int y, SymbolIds, SymbolClickCallback* cb = NULL, void* data = NULL);

	/** Get the currently selected Symbol */
	SymbolId getSelectedSymbol() const;

	/** Marks the given Symbol as selected */
	void setSymbolSelected(SymbolId);

	SymbolButton* getSymbolButton(SymbolId) const;

	/** enable/disable the button representing the given SymbolId*/
	void enableSymbol(SymbolId, bool enable);

	/** Remove SymbolButton with the given SymbolId from the bar */
	void hideSymbol(SymbolId);

	static void staticOnSymbolClick(Fl_Widget* p, void *data);

	void onSymbolClick(SymbolButton *b);

	void resize(int x, int y, int w, int h);

private:
	void addButton(int x, int y, SymbolId, SymbolClickCallback*, void *data = NULL);

private:
	SymbolIds _symbolIds;
	SymbolClickCallback* _cb = NULL;
	void* _data = NULL;
	static const int SPACING = 2;
	SymbolId _selectedSymbol = SymbolId::NONE;
};

//++++++++++++++++++++++++++++++++++++++++

class SymbolDefaults {
public:
	static Size getDefaultSize(SymbolId id) {
		return defaultSymbolSizes[id];
	}
};

//+++++++++++++++++++++++++++++++++++++++++

class SymbolBox;

//struct Point {
//	int x = 0;
//	int y = 0;
//};

class Connection {
public:
	Connection( SymbolBox* box1, SymbolBox* box2 );
	virtual ~Connection();

	/** give the connection a label which will be
	 * drawn at appropriate position
	 */
	void setLabel( const char* );

	const char* getLabel() const { return _labelBox->label(); }

	/**
	 * Checks if the given point (x, y) is "nearby"
	 * this Connection. Nearby means a distance less or equal
	 * the width of this Connection.
	 */
	bool isNearby( int x, int y ) const;

	/**
	 * Per default, a straight line is drawn between the
	 * two boxes to connect.
	 * If an angled connection is wanted, an additional
	 * point can be added using this method.
	 */
	void addPoint( int x, int y );
	void setSelected( bool selected );
	/**
	 * One of the two connected objects has moved.
	 * Adjust this connection line.
	 * @ box: the moved box
	 * @ delta_x: horizontal amount of moving
	 * @ delta_y: vertical amount of moving
	 */
	void draw();
	void setColor( Fl_Color );
	void setWidth( int w );

	SymbolBox* box1() { return _box1; }
	SymbolBox* box2() { return _box2; }

private:
//	inline bool isPointOnLine( const Point check,
//			                   const Point*, const Point* ) const;
	inline bool isElementOf( DragBox* box, int x, int y ) const;
	inline bool boxesInTouch() const;
	inline Intersection getIntersection( DragBox* box, Line& line ) const;
	void drawLabel( Line& line ) const;

private:
	Fl_Box* _labelBox;
	SymbolBox* _box1;
	SymbolBox* _box2;
	std::vector<Point*> _points;
	Fl_Color _color = FL_DARK2;
	int _w = 3;
	bool _selected = false;
};

//+++++++++++++++++++++++++++++++++++++++++
class SymbolBox;
typedef void (*MouseCallback)( SymbolBox*, int whichButton, void* );
//++++++++++++++++++++++++++++++++++++++++++

//#include <FL/Fl_SVG_Image.H>
class SymbolBox : public DragBox {
public:
	SymbolBox(int x, int y, int w, int h) :
		DragBox(x, y, w, h)
    {
		//color( FL_WHITE );
		box( FL_NO_BOX );
	}
	virtual ~SymbolBox() {}

	void registerMouseCallback( MouseCallback, void* data );

	/** don't use draw_custom */
	virtual void draw_custom() {}

	/** called by FlowCharCanvas */
	virtual void drawSymbol();

	virtual SymbolId getSymbolId() const = 0;

	void addConnection( Connection* );

	//label
	void setLabel(const char* txt);
	const char* getLabel() const {return _label.c_str();}
	//border color
	void setBorderColor( Fl_Color color );
	Fl_Color getBorderColor() const {return _bordercolor;}
	//border width
	void setBorderWidth(int w);
	int getBorderWidth() const {return _borderwidth;}
	//labelfont
	void setLabelFont(Fl_Font);
	Fl_Font getLabelFont() const {return _labelfont;}
	//labelfontsize
	void setLabelFontsize(Fl_Fontsize);
	Fl_Fontsize getLabelFontsize() const {return _labelfontsize;}

protected:
	/** called by dragbox on PUSH event with right mouse pressed. */
	virtual void onRightMouse();

	virtual void onMovedOrResized( bool resized, int delta_x, int delta_y );

protected:
	//int handle(int e);
	Fl_Color _bordercolor = FL_DARK1;
	int _borderwidth = 3;
	Fl_Font _labelfont = FL_HELVETICA;
	Fl_Fontsize _labelfontsize = 13;
	std::string _label;
	MouseCallback _mouseCallback = NULL;
	void* _mouseCallback_userdata = NULL;
private:
	std::vector<Connection*> _connections;
};

//++++++++++++++++++++++++++++++++++++++++

class Start : public SymbolBox {
public:
	Start(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::START;}

	~Start() {}
	void drawSymbol();

protected:
	virtual const char* getText() const { return "Start"; }

};

//++++++++++++++++++++++++++++++++++++++++

class End : public Start {
public:
	End( int x, int y, int w = 0, int h = 0 ) : Start( x, y, w, h ) {}
	~End() {}
	virtual SymbolId getSymbolId() const {return SymbolId::END;}
protected:
	virtual const char* getText() const { return "End"; }
};

//++++++++++++++++++++++++++++++++++++++++

class Decision : public SymbolBox {
public:
	Decision(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::DECISION;}

	~Decision() {}
protected:
	virtual void drawSymbol();

};

//++++++++++++++++++++++++++++++++++++++++

class Process : public SymbolBox {
public:
	Process(int x, int y, int w = 0, int h = 0 ) : SymbolBox(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::PROCESS;}

	~Process() {}
protected:
	virtual void drawSymbol();

};

//++++++++++++++++++++++++++++++++++++++++

class SymbolFactory {
public:
	static SymbolFactory& inst() {
		static SymbolFactory inst;
		return inst;
	}

	~SymbolFactory() {
		fprintf( stderr, "deleting SymbolFactory\n" );
		//todo: delete all symbols in _pSymbols
//		for (auto s : _symbols) {
//			delete s;
//		}
	}

	SymbolBox* create(SymbolId type, int x, int y, int w = 0, int h = 0, bool xy_is_center = true);

protected:
	SymbolFactory(){fprintf(stderr, "creating SymbolFactory\n");} // Prevent construction
	SymbolFactory(const SymbolFactory&); // Prevent construction by copying
	SymbolFactory& operator=(const SymbolFactory&); // Prevent assignment
	//~SymbolFactory() {} // Prevent unwanted destruction

private:
	void setSizeAndPosition( SymbolBox*, int x, int y, int w, int h, bool xy_is_center);
private:
	std::vector<SymbolBox*> _symbols;
};


#endif /* SYMBOLSTUFF_H_ */
