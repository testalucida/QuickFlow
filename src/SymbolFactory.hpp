/*
 * SymbolFactory.hpp
 *
 *  Created on: 12.05.2020
 *      Author: martin
 */

#ifndef SYMBOLFACTORY_HPP_
#define SYMBOLFACTORY_HPP_

#include "symbolbox_derivates.hpp"
#include <fltk_ext/TextMeasure.h>

class SymbolFactory {
public:
	static SymbolFactory& inst() {
		static SymbolFactory inst;
		return inst;
	}

	~SymbolFactory() {
		fprintf( stderr, "deleting SymbolFactory\n" );
	}

	SymbolBox* create( SymbolId id, int x, int y, int w = 0, int h = 0,
			bool xy_is_center = true )
	{
		//xy_is_center = true: create Symbol not at x, y but in such a way
		//that x, y is the middle  of the created Symbol.
		SymbolBox *pSymbol;
		switch ( id ) {
		case SymbolId::START:
			pSymbol = new Start( 0, 0, 0, 0 );
			break;
		case SymbolId::DECISION:
			pSymbol = new Decision( 0, 0, 0, 0 );
			break;
		case SymbolId::PROCESS:
			pSymbol = new Process( 0, 0, 0, 0 );
			break;
		case SymbolId::DATA_STORAGE:
			pSymbol = new DataStorage( 0, 0, 0, 0 );
			break;
		case SymbolId::END:
			pSymbol = new End( 0, 0, 0, 0 );
			break;
		default:
			string msg = "SymbolFactory::create(): " + std::to_string( id );
			msg += ": unknown SymbolType.";
			throw runtime_error( msg );
		}

		setSizeAndPosition( pSymbol, x, y, w, h, xy_is_center );
		_symbols.push_back( pSymbol );

		return pSymbol;

	}

protected:
	SymbolFactory() {
		fprintf( stderr, "creating SymbolFactory\n" );
	} // Prevent construction

	SymbolFactory( const SymbolFactory& ); // Prevent construction by copying

	SymbolFactory& operator=( const SymbolFactory& ); // Prevent assignment

private:
	void setSizeAndPosition( SymbolBox *symbol, int x, int y, int w, int h,
			bool xy_is_center )
	{
		if ( w == 0 || h == 0 ) {
			Size size = SymbolDefaults::getDefaultSize( symbol->getSymbolId() );
			w = w == 0 ? size.w : w;
			h = h == 0 ? size.h : h;
		}

		if ( xy_is_center ) {
			x -= w / 2;
			y -= h / 2;
		}

		symbol->position( x, y );
		symbol->size( w, h );
	}
private:
	std::vector<SymbolBox*> _symbols;
};

#endif /* SYMBOLFACTORY_HPP_ */
