//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once
#include "SymTab.h"

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) {};
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum class InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // end instruction.
        ST_Error                // Statement has an error.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);
    //{
    //    cout << "Must implement: ParseInstruction( )" << endl;
    //    return InstructionType::ST_MachineLanguage;
    //}

    // Compute the location of the next instruction.
    int LocationNextInstruction( int a_loc ) {
        if (m_type == InstructionType::ST_AssemblerInstr) {
            if (m_OpCode == "DS" || m_OpCode == "ORG") {
                //cout << m_Operand1NumericValue << endl;
                return a_loc + m_Operand1NumericValue;
            }
            
        }
        return a_loc + 1;
    }

    // To access the label
    inline string &GetLabel( ) {

        return m_Label;
    };
    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };


    //I added the following functions 
    // --------------------------------------------------------------------
    //To access the original instruction
    inline string& GetInstruction() {
        return m_instruction;
    }

    inline string GetStringOpCode() {
        return m_OpCode;
    }
    //To access the original instruction
    inline int& GetNumericOpCode() {
        return m_NumOpCode;
    }
    inline string GetStringOperand1() {
        return m_Operand1;
    }
    inline string GetStringOperand2() {
        return m_Operand2;
    }

    inline bool isValidLabel() {
        if (m_Label.size() <= 20 && isalpha(m_Label[0])) {
            return true;
        }
        return false;
    }

    inline bool isOperand1Numeric() {
        return m_IsNumericOperand1;
    }

    inline bool isOperand2Numeric() {
        return m_IsNumericOperand2;
    }

    Instruction::InstructionType GetInstructionType() {
        return m_type;
    }
  
    string DetermineContent(SymbolTable& m_symtab);
    void DetermineContentFromML(string& content, SymbolTable& m_symtab);
    void DetermineContentFromAI(string& content);
    string DetermineOpCode();



   

private:


    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand. 
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode = 0;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type = InstructionType::ST_Error; // The type of instruction.

    bool m_IsNumericOperand1 = false;// == true if the operand 1 is numeric.
    int m_Operand1NumericValue = 0;   // The value of the operand 1 if it is numeric.
    
    bool m_IsNumericOperand2 = false;// == true if the operand 2 is numeric.
    int m_Operand2NumericValue = 0;   // The value of the operand 2 if it is numeric.

    // Delete any comments from the statement.
    void DeleteComment(string &a_line)
    {
        size_t isemi1 = a_line.find(';');
        if (isemi1 != string::npos)
        {
            a_line.erase(isemi1);
        }
    }
    // Record the fields of the instructions.
    bool RecordFields( const string &a_line );

    // Get the fields that make up the statement.  This function returns false if there
    // are extra fields.
    bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
        string& a_Operand1, string& a_Operand2);

    // Check if a string contains a number. 
    bool isStrNumber(const string& a_str);
 

};


