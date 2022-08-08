//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "pch.h"
#include "SymTab.h"
   

/**/
/*
SymbolTable::AddSymbol()
NAME
    SymbolTable::AddSymbolAddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    SymbolTable::AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
Returns
    Does not return anything.
*/
/**/

void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{// If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) {
        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record the location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*SymbolTable::AddSymbol( const string &a_symbol, int a_loc );*/



/**/

/*
SymbolTable::DisplaySymbolTable()
NAME
    SymbolTable::DisplaySymbolTable - adds a new symbol to the symbol table.
SYNOPSIS
    void SymbolTable::DisplaySymbolTable();
DESCRIPTION

    This function will output the symbol and location as per the format of the symbol table
*/
/**/
void
SymbolTable::DisplaySymbolTable() {
    std::cout << "Symbol Table:" << std::endl <<std::endl;

    std::cout << std::left << std::setfill(' ') << std::setw(13)
        << "Symbol #"
        << std::left << std::setfill(' ') << std::setw(11)
        << "Symbol"
        << "Location" << std::endl;

    int i = 0;
    for (auto const& x : m_symbolTable)
    {
        std::cout << " "
        << std::left << std::setfill(' ') << std::setw(13)
        << i 
        << std::left << std::setfill(' ') << std::setw(13)
        << x.first  
        <<std::setfill(' ') << std::setw(9)
        << x.second 
        << std::endl;
        i++;
    }
}

/*void SymbolTable::DisplaySymbolTable();*/




/**/
/*
SymbolTable::LookupSymbol()
NAME
    SymbolTable::LookupSymbol - Checks if the symbol exists in the symbol table.
SYNOPSIS
    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);
        a_symbol  ->  name of the symbol to be looked for.
        a_loc     ->  location that will be changed if the symbol is found.
DESCRIPTION
    Checks if the symbol a_symbol exists in the symbol table. If the symbol exists
    the value of a_loc is changed to the location of the symbol.
RETURNS
    Returns true if the symbol is found. Returns false otherwise.

*/
/**/
bool
SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        a_loc = m_symbolTable[a_symbol];
        return true;
    }

    return false;
}

/* bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);*/
