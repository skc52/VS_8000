//
//      Implementation of the Assembler class.
//
#include "pch.h"
#include "Assembler.h"
#include "Errors.h"
#include <sstream>

/**/
/*
Assembler::Assembler()
NAME
    Assembler::Assembler - the constructor of the assembler class.
SYNOPSIS
    Assembler::Assembler(int argc, char *argv[]);
        argc	->	total number of command-line arguments.
        argv	->	An array consisting of strings of program and file name.
DESCRIPTION
    It is the constructor for the assembler class.
    argc and argv are passed to the file access constructor.
RETURNS
    No return value for a constructor.
*/
/**/
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}

/*Assembler::Assembler(int argc, char *argv[]);*/


// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler()
{
}

/**/
/*
Assembler::PassI()
NAME
    Assembler::PassI - Helps in filling the symbol table.
SYNOPSIS
    Assembler::PassI();
DESCRIPTION
    It establishes the location of the labels.
RETURNS
    void.
*/
/**/

void Assembler::PassI()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (; ; ) {


        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, and we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);


        //------------------------------------------------------------
        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) {
            return;
        }

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments. 

        //-----------------------------------
        //and store the location for the current instruction
        //only if it is not a comment
        //-----------------------------------

        if (st == Instruction::InstructionType::ST_Comment)
        {
            continue;
        }

        // Handle the case where there is an error.

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}
/*Assembler::PassI();*/

/**/
/*
Assembler::PassII()
NAME
    Assembler::PassII - Translates the code and records errors.
SYNOPSIS
    Assembler::PassII();
DESCRIPTION
    Translates the code in the each line and stores them in the emulator. Also records errors.
RETURNS
    void.

*/
/**/
void Assembler::PassII() {
   
    cout << "Press ENTER to continue...";
    cin.ignore();

    cout << "Translation of Program:" << endl << endl;
    cout << std::left << std::setfill(' ') << std::setw(13);
    cout << "Location";
    cout << std::left << std::setfill(' ') << std::setw(11);
    cout << "Contents";
    cout << "Original Statement"
        << endl;

    // Clean all file flags and go back to the beginning of the file.
    m_facc.rewind();

    // Tracks the location of the instructions to be generated.
    int loc = 0;

    // Successively process each line of source code.
    string line;
    string error; //stores the error message
    Errors::InitErrorReporting();


    for (; ; ) {

        error = "";//reset the error to empty string
        if (!m_facc.GetNextLine(line)) {
            
            // If there are no more lines, and we are missing an end statement.
            // We will let this error be reported by Pass II.
            error = "There is no END statement!";
            Errors::RecordError(error);

            break;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);
        m_Instructions_Vector.push_back(line);
        opCodeContent_Vector.push_back(m_inst.DetermineContent(m_symtab));

        


        //------------------------------------------------------------
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) {
            m_Current_Location_Vector.push_back(-2);
            if (m_facc.GetNextLine(line) == true) {
                error = "END is present at " + to_string(loc) + " but not the last statement!";
                Errors::RecordError(error);
            }
            break;
        }

        

        if (st == Instruction::InstructionType::ST_Comment)
        {
            m_Current_Location_Vector.push_back(-1);
            continue;
        }
        else {
            m_Current_Location_Vector.push_back(loc);
        }

        //check whethere there are errors in ST_MAchinetype
        if (st == Instruction::InstructionType::ST_MachineLanguage) {
            FindErrorsInMachineInstruction(loc);
        }

        if (st == Instruction::InstructionType::ST_AssemblerInstr) {

            FindErrorsInAssemblerInstruction(loc);
            
        }

        if (st == Instruction::InstructionType::ST_Error) {
            error = "Operation Code is Invalid at " + to_string(loc);
            Errors::RecordError(error);

        }

        if (m_inst.LocationNextInstruction(loc) > 999999) {
            error = "Insufficient memory.";
            Errors::RecordError(error);
            return;//return from the 
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);


    }

    //-------------------------------------------------------------------------------
    //this is my output code for the translation
    for (int i = 0; i < m_Instructions_Vector.size(); i++) {
        cout << "  ";
        cout << std::left << std::setfill(' ') << std::setw(10);
        if (m_Current_Location_Vector[i] < 0) {
            cout << " ";
        }
        else {
            cout << m_Current_Location_Vector[i] << " ";
        }
        cout << std::left << std::setfill(' ') << std::setw(11);
        cout << opCodeContent_Vector[i] << "    ";


        cout << m_Instructions_Vector[i] << endl;

        if (Errors::getErrorCount() == 0) {
            long long content;
            if (opCodeContent_Vector[i].length() == 0) {
                content = 0;

            }
            else {
                content = stoll(opCodeContent_Vector[i]);
            }

            //do not include comments
            if (m_Current_Location_Vector[i] >= 0) {
                m_emul.insertMemory(m_Current_Location_Vector[i], content);

            }
        }
    }
}
/*Assembler::PassII();*/


/**/
/*
Assembler::RunProgramInEmulator()
NAME
    Assembler::RunProgramInEmulator - Runs the emulator
SYNOPSIS
    void Assembler::RunProgramInEmulator();
DESCRIPTION
    Checks if there are any errors and if there are, displays them.
    If there are no errors, it runs the emulation.
RETURNS
    void.

*/
/**/


void Assembler::RunProgramInEmulator(){
    cout << "Press Enter to continue...\n";
    cin.ignore();
    
    if (Errors::getErrorCount() > 0) {
        cout << "Emulator cannot run because there are errors present!\n";
        cout << Errors::getErrorCount() << endl;
        Errors::DisplayErrors();
    }
    else {
        cout << "Results from emulating program:\n";
        m_emul.runProgram();
    }
}
/*void Assembler::RunProgramInEmulator(); */


/**/
/*
Assembler::FindErrorsInMachineInstruction()
NAME
    Assembler::FindErrorsInMachineInstruction - Finds and Records errors relaated to Machine Instruction.
SYNOPSIS
    void Assembler::FindErrorsInMachineInstruction(int loc);
    loc -> location of the current instruction

DESCRIPTION
    Considers different scenarios for errors in Machine Instructions,
    and if found, records them.
RETURNS
    void.

*/
/**/

void Assembler::FindErrorsInMachineInstruction(int loc) {
 
    string error = "";

    //check if is has a label
    //if it has then check its validity
    //check whether it has been defined already
    //in that case error will be Multiply Defined Symbol
    if (m_inst.isLabel()) {
        if (!m_inst.isValidLabel()) {
            error = "Invalid Label at " + to_string(loc);
            Errors::RecordError(error);
        }
        int tempLoc = 0;
        m_symtab.LookupSymbol(m_inst.GetLabel(), tempLoc);
        if (tempLoc == m_symtab.multiplyDefinedSymbol) {
            error = "Symbol aready defined at " + to_string(loc);
            Errors::RecordError(error);
        }
    }

    //for the ones which require their operand1 to be register
    //from opcode 1 to 10 @

    if (m_inst.GetNumericOpCode() >= 1 && m_inst.GetNumericOpCode() <= 10) {//except store operation
        //store operation also has register as its first operand and a symbol as its 2nd    

        if (!m_inst.isOperand1Numeric()) {

            error = "Operand 1 is supposed to be a register, But it is not at " + to_string(loc);
            Errors::RecordError(error);
        }

        //from op code 1 to 6 operand 2 is symbolic 
        if (m_inst.GetNumericOpCode() >= 1 && m_inst.GetNumericOpCode() <= 6) {
            if (m_inst.isOperand2Numeric()) {
                error = "Operand2 is supposed to be symbolic, but it is numeric at " + to_string(loc);
                Errors::RecordError(error);
            }
        }

        //from 7 to 10 operand2 is numeric 
        if (m_inst.GetNumericOpCode() >= 7 && m_inst.GetNumericOpCode() <= 10) {
            if (!m_inst.isOperand2Numeric()) {
                error = "Operand2 is supposed to be numeric, but it is not at " + to_string(loc);
                Errors::RecordError(error);
            }
            
        }

        //if there are no operands or one missing operand 
        if (m_inst.GetStringOperand1().empty() || m_inst.GetStringOperand2().empty()) {
            error = "Either one or both operands is empty. Requires two operands at " + to_string(loc);
            Errors::RecordError(error);

        }
        


    }

    //for opcode 11 and 12 operand1 is ignored and operand 2 is symbolic
    if (m_inst.GetNumericOpCode() == 11 || m_inst.GetNumericOpCode() == 12) {
        //if operand1 is register
        if (m_inst.isOperand1Numeric()) {
            //if the operand2 is numeric @
            if (m_inst.isOperand2Numeric()) {
                error = "Operand2 is supposed to be symbolic. Instead it is numeric at " + to_string(loc);
                Errors::RecordError(error);
            }
            //if there is no operand2 @
            if (m_inst.GetStringOperand2().empty()) {
                error = "Symbolic operand not found at " + to_string(loc);
                Errors::RecordError(error);
            }
        }
        //if operand1 is symbolic there should not be any other operand @
        if (!m_inst.isOperand1Numeric()) {
            if (!m_inst.GetStringOperand2().empty()) {
                cout << m_inst.GetStringOperand2() << endl;
                error = "Since operand1 is symbolic, there should not be an operand2 at " + to_string(loc);
                Errors::RecordError(error);
            }
        }

        //if there are no operands @
        if (m_inst.GetStringOperand1().empty()) {
            error = "Operands cannot be empty. Requires at least a symbolic operand at " + to_string(loc);
            Errors::RecordError(error);

        }
    }



    //checking for branching
    if (m_inst.GetNumericOpCode() == 13) {
        //when 13 register value can be ignored and symbolic addr is mandatory
        if (m_inst.GetStringOperand2().empty()) {
            if (m_inst.isOperand1Numeric()) {
                error = "The only operand is a register while it should be an address at " + to_string(loc);
                Errors::RecordError(error);
            }
        }
        //if both are registers
        if (m_inst.isOperand1Numeric() && m_inst.isOperand2Numeric()) {
            error = "Both operands are numeric. No symbolic address found at " + to_string(loc);
            Errors::RecordError(error);

        }
        //if the first one is symbol and the next is register
        if (!m_inst.isOperand1Numeric() && m_inst.isOperand2Numeric()) {
            error = "First operand cannot be a symbol. Second operand cannot be the register. At " + to_string(loc);
            Errors::RecordError(error);
        }

        //if there are no operands
        if (m_inst.GetStringOperand1().empty()) {//if operand1 is empty operand2 is implicitly empty
            error = "Branching requires the memory address. Operands cannot be empty at " + to_string(loc);
            Errors::RecordError(error);

        }
    }

    //from op code 14 to 16, the first operand is a register and the second operand is an address
    if (m_inst.GetNumericOpCode() >= 14 && m_inst.GetNumericOpCode() <= 16) {

        if (!m_inst.isOperand1Numeric()) {
            error = "Operand 1 is supposed to be a register, But it is not at " + to_string(loc);
            Errors::RecordError(error);
        }

        if (m_inst.isOperand2Numeric()) {
            error = "Operand2 is supposed to be symbolic, but it is numeric at " + to_string(loc);
            Errors::RecordError(error);
        }
        //if there are no operands or one missing operand
        if (m_inst.GetStringOperand1().empty() || m_inst.GetStringOperand2().empty()) {
            error = "Either one or both operands are empty. Requires one numeric and one symbolic operand at " + to_string(loc);
            Errors::RecordError(error);

        }
    }

    //throw error if symbolic operands are not defined
    if (m_inst.isOperand2Numeric() == false && !m_inst.GetStringOperand2().empty()) {
        int tempLoc = 0;
        m_symtab.LookupSymbol(m_inst.GetStringOperand2(), tempLoc);
        if (tempLoc == 0) {

            error = "Symbolic Operand2 not defined at " + to_string(loc);
            Errors::RecordError(error);
        }

    }
    if (m_inst.isOperand1Numeric() == false && !m_inst.GetStringOperand1().empty()) {
        int tempLoc = 0;
        m_symtab.LookupSymbol(m_inst.GetStringOperand1(), tempLoc);
        if (tempLoc == 0) {
            error = "Symbolic Operand1 not defined at " + to_string(loc);
            Errors::RecordError(error);
        }

    }

    //registers cannot be greater than 9
    if (m_inst.isOperand1Numeric()) {
        if (stoi(m_inst.GetStringOperand1()) > 9) {
            error = "Numeric operand cannot be greater than 9 at " + to_string(loc);
            Errors::RecordError(error);
        }
        else if (stoi(m_inst.GetStringOperand1()) < 0) {
            error = "Numeric operand cannot be lesser than 0 at " + to_string(loc);
            Errors::RecordError(error);
        }

    }
    if (m_inst.isOperand2Numeric()) {
        if (stoi(m_inst.GetStringOperand2()) > 9) {
            error = "Numeric operand cannot be greater than 9 at " + to_string(loc);
            Errors::RecordError(error);
        }
        else if (stoi(m_inst.GetStringOperand2()) < 0) {
            error = "Numeric operand cannot be lesser than 0 at " + to_string(loc);
            Errors::RecordError(error);
        }
    }
}

/*void Assembler::FindErrorsInMachineInstruction(int loc);*/



/**/
/*
Assembler::FindErrorsInAssemblerInstruction()
NAME
    Assembler::FindErrorsInAssemblerInstruction - Finds and Records errors relaated to Assembler Instruction.
SYNOPSIS
    void Assembler::FindErrorsInAssemblerInstruction(int loc);
    loc -> location of the current instruction

DESCRIPTION
    Considers different scenarios for errors in Assembler Instructions,
    and if found, records them.
RETURNS
    void.

*/
/**/
void Assembler::FindErrorsInAssemblerInstruction(int loc) {
    string error = "";

    //operand1 is numeric and there should not be an operand2
    if (m_inst.GetStringOpCode() == "ORG" || m_inst.GetStringOpCode() == "DS" || m_inst.GetStringOpCode() == "DC") {

        //if no operand is provided
        if (m_inst.GetStringOperand1().empty()) {
            error = "There is no operand at " + to_string(loc);
                Errors::RecordError(error);
        }
        else {
            if (!m_inst.isOperand1Numeric()) {
                error = "Operand1 is not numeric " + to_string(loc);
                    Errors::RecordError(error);
            }

            if (!m_inst.GetStringOperand2().empty()) {
                error = "Operand2 is not empty at " + to_string(loc);
                Errors::RecordError(error);
            }
        }



        //DC and DS must have a label ORG does not
        if (m_inst.GetStringOpCode() == "DS" || m_inst.GetStringOpCode() == "DC") {

            if (!m_inst.isLabel()) {
                error = "Does not have a label at " + to_string(loc);
                Errors::RecordError(error);
            }
            else {
                //check for their validity and occurence
                if (!m_inst.isValidLabel()) {
                    error = "Invalid Label at " + to_string(loc);
                    Errors::RecordError(error);
                }
                int tempLoc = 0;
                m_symtab.LookupSymbol(m_inst.GetLabel(), tempLoc);
                if (tempLoc == m_symtab.multiplyDefinedSymbol) {
                    error = "Symbol at " + to_string(loc) + " multiple times.";
                    Errors::RecordError(error);
                }
            }
        }
        if (m_inst.GetStringOpCode() == "DC") {
            if (m_inst.isOperand1Numeric()) {
                //check if the constant is too large
                if (stoi(m_inst.GetStringOperand1()) > 999999) {
                    error = "Constant is too large at " + to_string(loc);
                    Errors::RecordError(error);

                }
            }
        }
    }

    //end does not have any operands
    else if (m_inst.GetStringOpCode() == "END") {
        if (!m_inst.GetStringOperand1().empty()) {
            error = "There are operands in END! There should not be any!";
            Errors::RecordError(error);

        }
    }
}

/*void Assembler::FindErrorsInAssemblerInstruction(int loc);*/
