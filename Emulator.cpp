#include "pch.h"     // This must be present if you use precompiled headers which you will use. 
#include "Emulator.h"


/**/
/*
Emulator::insertMemory()
NAME
    Emulator::insertMemory - Stores the translated code in the
                             specified memory location.
SYNOPSIS
    bool Emulator::insertMemory(int a_location, long long a_contents);
    a_location -> The location where the translated code is to be stored.
    a_contents -> The translated code that is to be stored.
DESCRIPTION
    Stores the translated code in the specified memory location.
RETURNS
    Returns true if the a_contents was stored in the location a_location, else returns false.

*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents) {
    if (a_location >= MEMSZ) {
        return false; //location is greater than the memory size
    }

    m_memory[a_location] = a_contents;
    return true;
}
/*bool Emulator::insertMemory(int a_location, long long a_contents);*/



/**/
/*
emulator::runProgram()
void emulator::runProgram()
NAME
    emulator::runProgram() - Runs the emulator.
SYNOPSIS
    void emulator::runProgram();
DESCRIPTION
    This function runs the emulator for the assembler
Returns
    Does not return anything.
*/
/**/

void emulator::runProgram() {
    long long opCode = 0;
    int mem_addr = 0;
    int reg = 0;
    int reg2 = 0;

    for (int location = 0; location < MEMSZ; location++) {
        long long content = m_memory[location];

        //if content is not equal to zero then in that case we ae dealing
        //with executable instructions
        if (content != 0) {
            opCode = content / 10000000;
            if (opCode >= 7 && opCode <= 10) {
                reg2 = (content / 100000) % 10;
            }
            else {
                mem_addr = content % 1000000;
            }
            reg = (content / 1000000) % 10;

            switch (opCode) {
            case 1:
                m_register[reg] += m_memory[mem_addr];
                break;
            case 2:
                m_register[reg] -= m_memory[mem_addr];
                break;
            case 3:
                m_register[reg] *= m_memory[mem_addr];
                break;
            case 4:
                m_register[reg] /= m_memory[mem_addr];
                break;
            case 5:
                m_register[reg] = m_memory[mem_addr];
                break;
            case 6:
                m_memory[mem_addr] = m_register[reg];
                break;
            case 7:
                m_register[reg] += m_register[reg2];
                break;
            case 8:
                m_register[reg] -= m_register[reg2];
                break;
            case 9:
                m_register[reg] *= m_register[reg2];
                break;
            case 10:
                m_register[reg] /= m_register[reg2];
                break;
            case 11:
                cout << "? ";
                int input;
                cin >> input;
                //TODO Check if this is an integer
                if (to_string(input).length() > 9) {
                    cout << "The input size is too big.\n";
                    return;
                }
                m_memory[mem_addr] = input;
                break;
            case 12:
                cout << m_memory[mem_addr] << endl;
                break;
            case 13:
                //- 1 because location gets incremented by 1 the next time 
                //the content is read from a memory address, it will be the next memory
                //address
                location = mem_addr - 1;
                break;
            case 14:
                if (m_register[reg] < 0) {
                    location = mem_addr - 1;
                }
                break;
            case 15:
                if (m_register[reg] == 0) {
                    location = mem_addr - 1;
                }
                break;
            case 16:
                if (m_register[reg] > 0) {
                    location = mem_addr - 1;
                }
                break;
            case 17:
                cout << "End of Emulation\n";
                return;
                /* default:
                     cout << "Operation code is invalid.\n";
                     return false;*/
            }
        }
    }
}

/*void emulator::runProgram();*/