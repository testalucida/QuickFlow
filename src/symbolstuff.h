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
#include <fltk_ext/TextSplitter.h>
#include <fltk_ext/Line.h>
#include <fltk_ext/FlxRect.h>
#include "dialogs.h"

#include "std.h"



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




//++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++



#endif /* SYMBOLSTUFF_H_ */
