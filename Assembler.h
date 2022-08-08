//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII();

    // Processes errors in Machine Instructions and records them 
    void FindErrorsInMachineInstruction(int loc);

    void FindErrorsInAssemblerInstruction(int loc);


    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();



private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object


    //The following vectors share the same index
    // These vectors are used while outputting the translation of the program
    //----------------------------------------
    vector<int> m_Current_Location_Vector;  //stores location of instructions
    vector<string> opCodeContent_Vector;    //stores content/translations of instructions
    vector<string> m_Instructions_Vector;   //stores every line of the instructions from the file.
    //----------------------------------------
};

