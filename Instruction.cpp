#include "pch.h"

#include "Instruction.h"
#include "Errors.h"


/**/
/*
Instruction::InstructionType Instruction::ParseInstruction()
NAME
    Instruction::InstructionType Instruction::ParseInstruction - Determining the type of instruction.
SYNOPSIS
    Instruction::InstructionType Instruction::ParseInstruction(string a_line);
        a_line -> a string that consists of a line in the file to be parsed.
DESCRIPTION
    
    It determines the type of instruction.
Returns
    Returns the instruction type (Comment, Assembler, Error, Machine)
*/
/**/

Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the sceond pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment( a_line );

    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields( a_line );

    // If there is extra data, return false.
    //------------this is the code that I added - error checking
    if (isFormatError) {
        Errors::RecordError("Extra Operand at " + a_line);
    }
    
    //---------------------------
    
    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }
    // Return the instruction type.
    return m_type;
}

/*Instruction::InstructionType Instruction::ParseInstruction(string a_line);*/


/**/
/*
Instruction::RecordFields()
NAME
    Instruction::RecordFields - Extracts the fields from instruction and determines instruction type.
SYNOPSIS
    bool Instruction::RecordFields( const string &a_line );
    a_line -> a string that consists of a line in the file to be parsed.
DESCRIPTION
    The function calls another function that parse the line into
    label, operation code, and operands of instruction
    from the given line. 
    Then it determines the Instruction Type and
    numeric Op Code.
    It also records an error if there are extra operands provided.
    
Returns
    Returns true if parsing the line into fields(label, opcode, operand1, operand2)
    was successful and false otherwise
*/
/**/

bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
    //---------------isFormatError is true when there are extra data
    bool isFormatError = !ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) {
        m_type = InstructionType::ST_Comment;
        return isFormatError;
    }
    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
    // I leave to the class the tasks:
    // - Determine and record the instruction type from the op code.
    // - Recording the numberic Op code for machine lanuage equivalents.

    if (m_OpCode == "DS" || m_OpCode == "DC" || m_OpCode == "ORG") {
        m_type = InstructionType::ST_AssemblerInstr;
        if (m_OpCode != "ORG") {
            m_NumOpCode = 0;
        }
       
    }
    
    else if (m_OpCode == "END") {
        m_type = InstructionType::ST_End;
    }
 
    else if (m_OpCode == "ADD" || m_OpCode == "SUB" || m_OpCode == "MULT" || m_OpCode == "DIV"
            || m_OpCode == "LOAD" || m_OpCode == "STORE" || m_OpCode == "READ" 
            || m_OpCode == "WRITE" || m_OpCode == "ADDR" || m_OpCode == "MULTR"
            || m_OpCode == "DIVR" || m_OpCode == "B" || m_OpCode == "BM"
            || m_OpCode == "BZ" || m_OpCode == "BP" || m_OpCode == "HALT"
        ) {
        m_type = InstructionType::ST_MachineLanguage;
        if (m_OpCode == "ADD") {
            m_NumOpCode = 1;
        }
        else if (m_OpCode == "SUB") {
            m_NumOpCode = 2;
        }
        else if (m_OpCode == "MULT") {
            m_NumOpCode = 3;
        }
        else if (m_OpCode == "DIV") {
            m_NumOpCode = 4;
        }
        else if (m_OpCode == "LOAD") {
            m_NumOpCode = 5;
        }
        else if (m_OpCode == "STORE") {
            m_NumOpCode = 6;
        }
        else if (m_OpCode == "READ") {
            m_NumOpCode = 11;
        }
        else if (m_OpCode == "WRITE") {
            m_NumOpCode = 12;
        }
        else if (m_OpCode == "ADDR") {
            m_NumOpCode = 7;
        }
        else if (m_OpCode == "SUBR") {
            m_NumOpCode = 8;
        }
        else if (m_OpCode == "MULTR") {
            m_NumOpCode = 9;
        }
        else if (m_OpCode == "DIVR") {
            m_NumOpCode = 10;
        }
        else if (m_OpCode == "B") {
            m_NumOpCode = 13;
        }
        else if (m_OpCode == "BM") {
            m_NumOpCode = 14;
        }
        else if (m_OpCode == "BZ") {
            m_NumOpCode = 15;
        }
        else if (m_OpCode == "BP") {
            m_NumOpCode = 16;
        }
        else if (m_OpCode == "HALT") {
            m_NumOpCode = 17;
            /*m_Operand2 = "000000";*/
        }
    }

    //set the numeric op code for op codes that are not valid to -3
    //-3 means the Op code is invalid
    else {
        m_type = InstructionType::ST_Error;
        m_NumOpCode = -3;
    }

    return isFormatError;
}
/*bool Instruction::RecordFields( const string &a_line );*/


/**/
/*
Instruction::ParseLineIntoFields()
NAME
    Instruction::ParseLineIntoFields - Extracts the fields from instruction.
SYNOPSIS
    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2);
    a_line -> a string that consists of a line in the file to be parsed.
    a_label -> a string that holds the parsed label
    a_OpCode -> a string that holds the parsed OpCode
    a_Operand1 -> a string that holds the parsed operand1
    a_Operand2 -> a string that holds the parsed operand2
DESCRIPTION
    The function parses the line into
    label, operation code, and operands of instruction
    from the given line.
    
Returns
    Returns true if there were no extra operands, returns false otherwise.
*/
/**/

bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    //Here is a comment that sit on its own line.
    
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    
    
    return endStr.empty() ? true : false;
}
/*bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2);*/




/**/
/*
Instruction::isStrNumber()
NAME
     Instruction::isStrNumber - Determines whether a string is a number.
SYNOPSIS
    bool Instruction::isStrNumber(const string& a_str);
    a_str -> a string that contains the string to be checked 

DESCRIPTION
    The function checks whether the string is a number t

Returns
    Returns true if a_str is a number, returns false otherwise.
*/
/**/

bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}
/*    bool Instruction::isStrNumber(const string& a_str);*/



/**/
/*
Instruction::DetermineContent()
NAME
     Instruction::DetermineContent - Determines the translation for a given instruction.
SYNOPSIS
    string Instruction::DetermineContent(SymbolTable& m_symtab);
    m_symtab -> refers to the symbol table built by passI

DESCRIPTION
    The function determines content based on Instruction Type
    if Instruction Type is a comment content will be empty
    if Instruction Type is Machinelanguage or Assembler,
    content will be determined by DetermineContentFromML and
    DetermineContentFromAI respectively.
    

Returns
    Returns the translation/content of an instruction.
*/
/**/
string Instruction::DetermineContent(SymbolTable& m_symtab) {
   
    string content;
    if (m_type == InstructionType::ST_Comment) {
        content = "";
    }
    else if (m_type == InstructionType::ST_MachineLanguage) {
        content = "";
        content += DetermineOpCode();
        DetermineContentFromML(content, m_symtab);
    }
    else if (m_type == InstructionType::ST_AssemblerInstr) {
        content = "";
        content += DetermineOpCode();
        DetermineContentFromAI(content);
    }
    return content;
}
/*    string Instruction::DetermineContent(SymbolTable& m_symtab);*/



/**/
/*
Instruction::DetermineContentFromML()
NAME
     Instruction::DetermineContentFromML - Determines the translation for a Machine instruction.
SYNOPSIS
    void Instruction::DetermineContentFromML(string& content, SymbolTable& m_symtab);
    content -> used for containing the content/translation of the instruction
    m_symtab -> refers to the symbol table built by passI

DESCRIPTION
    The function determines content for Machinelanguage Instructions.
    There are three parts to a content/translation - opCode, registerPart, memoryAddrPart
    It determines the registerPart and memoryPart for the content.

Returns
    Does not return anything.
*/
/**/
void Instruction::DetermineContentFromML(string& content, SymbolTable& m_symtab) {
    //oper1Content is the register part of the content
    //oper2Content is the memory part of the content

    string oper1Content;
    string oper2Content;

    //Determine oper1Content
    //Determine oper2Content
    //Concatenate both with content
    //return content


    

    //if m_Operand1 is not a number then it represents a memory address
    

    //if m_Operand1 is a number then it represents a register
    //set the register part of the content to m_Operand1

    //register values can be ignored for numeric Op code = 11 12 13 17
    //when register value can be ignored, and it is not provided
    //then set the register part of the content to 9
    //9 because it is our default for no register cases in above op codes
    //then set the memory part of the content as m_Operand1

    if (m_NumOpCode == 11 || m_NumOpCode == 12 || m_NumOpCode == 13 || m_NumOpCode == 17) {

        if (m_IsNumericOperand1 == false) {
            oper1Content = "9";
            oper2Content = m_Operand1;

        }
        else {
            oper1Content = m_Operand1;

        }
    }
    else {
        if (m_IsNumericOperand1 == false) {
            oper1Content = "?";
            oper2Content = m_Operand1;

        }
        else {
            oper1Content = m_Operand1;

        }

    }

    //since we are done determining the register part of the content,
    //append oper1content to 
    content += oper1Content;

    //for operations that require two registers - from 7 to 10
    if (m_NumOpCode >= 7 && m_NumOpCode <= 10) {
        oper2Content = m_Operand2 + "00000";
    }
    
    else {
        //if our operand1 was numeric, then oper2content would be empty
        //in that case, it would be equal to m_Operand2
        if (oper2Content.empty()) {
            oper2Content = m_Operand2;
        }

        //determine the location held by oper2content
        int loc = 0;
        m_symtab.LookupSymbol(oper2Content, loc);
        string strLoc;
        //if loc is still equal to 0 then that means the label/symbol
        //represented by oper2content
        //does not exist in the symbol table

        
        if ((loc == 0 && m_NumOpCode != 17) || loc == m_symtab.multiplyDefinedSymbol) {

            strLoc = "??????";

        }
        
        else {
            strLoc = to_string(loc);
            int lenLoc = (int) strLoc.length();
            int remLenLoc = 6 - lenLoc;

            string zeroPrependingString = "";
            for (int i = 0; i < remLenLoc; i++) {
                zeroPrependingString += "0";
            }
            strLoc = zeroPrependingString + strLoc;
        }
        oper2Content = strLoc;
    }

    content += oper2Content;
}
/*void Instruction::DetermineContentFromML(string& content, SymbolTable& m_symtab)*/


/**/
/*
Instruction::DetermineContentFromAI()
NAME
     Instruction::DetermineContentFromML - Determines the translation for a Assembler instruction.
SYNOPSIS
    void Instruction::DetermineContentFromAI(string& content);
    content -> used for containing the content/translation of the instruction

DESCRIPTION
    The function determines content for Assembler Instructions.
    There are three parts to a content/translation - opCode, registerPart, memoryAddrPart
    It determines the registerPart and memoryPart for the content.

Returns
    Does not return anything.
*/
/**/
void Instruction::DetermineContentFromAI(string& content) {
    //For Op Code other than DC in Instruction Type Assembler
    //content will be empty
    //For Op Code DC
    //variable oper1Content is the register part of the content
    //variable oper2Content is the memory part of the content
    //oper1Content will be 0 
    //oper2Content will be the number that it is trying to define
    //prepend 0 to oper2content to make its length equal to 6
    string oper1Content;
    string oper2Content;

    if (m_OpCode == "ORG" || m_OpCode == "DS" || m_OpCode == "END") {
        content = "";
        return;
    }
    else {//DC
        oper1Content = "0";
        //if m_Operand1 is empty then replace the memory part with "??????"
        if (m_Operand1.empty()) {
            oper2Content = "??????";
        }
        else {
            int lenLoc = (int)m_Operand1.length();
            int remLenLoc = 6 - lenLoc;

            string zeroPrependingString = "";
            for (int i = 0; i < remLenLoc; i++) {
                zeroPrependingString += "0";
            }
            oper2Content = zeroPrependingString + m_Operand1;
        }
        
    }
    content = content + oper1Content + oper2Content;
}
/* void Instruction::DetermineContentFromAI(string& content);*/



/**/
/*
Instruction::DetermineOpCode
NAME
     Instruction::DetermineOpCode() - Determines the operation code for an instruction.
SYNOPSIS
    string Instruction::DetermineOpCode();

DESCRIPTION
    The function determines opCodes for the instruction.
   
Returns
    Returns the opCode as string.
*/
/**/

string Instruction::DetermineOpCode() {
    string numOpCodeStr;
    //if the length of numeric op code is not 2, we need to 
    //make it to 2 by prepending 0 
    if (to_string(m_NumOpCode).length() != 2) {
        numOpCodeStr = "0" + to_string(m_NumOpCode);
    }
    else {
        numOpCodeStr = to_string(m_NumOpCode);
    }

    //if Op Code is invalid
    if (m_NumOpCode == -3) {
        numOpCodeStr = "??";
    }
    return numOpCodeStr;
}
/*string Instruction::DetermineOpCode();*/



