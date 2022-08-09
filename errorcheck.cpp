#include <iostream>
#include <cstdio>
#include <cassert>
#include <cctype>
#include "errors.h"
#include "errorcheck.h"
#include "constants.h"
using namespace std;

int checkDuplicate(int const value, string const& fileName, int const count, int const seekValue)
{
    /*Using a new ifstream, loop through up until the value before the one being checked, returning
     the value it has been mapped to if the value has been found at any point, and false if not*/

    ifstream in(fileName);
    if (!in) {
        cerr << "usage: enigma plugboard-file"
             << " reflector-file (<rotor-file>)* rotor-positions" << endl;
        throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    assert(in);

    int checkValue = 0, prevValue = 0;

    /* Start on the correct value */
    for (int i = 0; i < seekValue; i++) {
        in >> checkValue;
    }

    in >> checkValue;
    for (int i = 0; i < count; i++) {
        if (checkValue == value) {
            
            /* IF THE FILE IS A ROTOR, can just return the integer i, 
            as i is what the duplicate value is mapped to*/
            if (fileName.substr( fileName.length() - 4 ) == ".rot") {
                in.close();
                return i;
            }

            /*DIFFERENT EDGE CASE FOR REFLECTORS AND PLUGBOARD: if the duplicate value is found and is the
            first of a pair, get next value and return this as the value the duplicate value is mapped to*/
            if (i % 2 != 0) {
                return prevValue;
            }
            /* If the duplicate value is the second of a pair, return prevValue, as 
            this will be what the duplicate value has been mapped to */
            else {
                in >> checkValue;
                return checkValue;
            }
        }

        /* If no duplicate found, make previous value equal to 
        value just checked, and get next value in file for checkValue */
        prevValue = checkValue;
        in >> checkValue;
    }
    in.close();
    return -1;
}

bool checkCharacter(std::ifstream& in)
{
    if (in.fail()) {
        return true;
    }

    /*I use peek to see if there is a character immediately following the read in 
    integer, and if so returns non numeric over 'out of range' or duplicate' */
    char nextInt = in.peek();

    /*if nothing after the read in integer and eof reached, return false*/
    if (in.eof()) {
        return false;
    }
    else if (!isdigit(nextInt) && !isspace(nextInt)) {
        return true;
    }
    else {
        return false;
    }
}

bool checkValidNumber(int const value)
{
    if (value >= enigmaConst::ALPHABET_SIZE || value < 0) {
        return true;
    }
    return false;
}

bool checkOdd(int const counter)
{
    if (counter % 2 != 0) {
        return true;
    }
    return false;
}

bool checkOverDigits(int const counter)
{
    if (counter > enigmaConst::ALPHABET_SIZE) {
        return true;
    }
    return false;
}

void checkFileType(string const& fileName, int const type, ifstream& in)
{
    /*Switches extension check based on type of file*/
    switch (type)
    {
        case enigmaConst::PB : 
            if(fileName.substr( fileName.length() - 3 ) != ".pb") {
                cerr << "usage: enigma plugboard-file"
                     << " reflector-file (<rotor-file>)* rotor-positions"
                     << endl;
                in.close();
                throw ERROR_OPENING_CONFIGURATION_FILE;
            }
            break;
        case enigmaConst::RF : 
            if(fileName.substr( fileName.length() - 3 ) != ".rf") {
                cerr << "usage: enigma plugboard-file"
                     << " reflector-file (<rotor-file>)* rotor-positions"
                     << endl;
                in.close();
                throw ERROR_OPENING_CONFIGURATION_FILE;
            }
            break;
        case enigmaConst::ROT : 
            if(fileName.substr( fileName.length() - 4 ) != ".rot") {
                cerr << "usage: enigma plugboard-file"
                     << " reflector-file (<rotor-file>)* rotor-positions"
                     << endl;
                in.close();
                throw ERROR_OPENING_CONFIGURATION_FILE;
            }
            break;
        case enigmaConst::POS : 
            if(fileName.substr( fileName.length() - 4 ) != ".pos") {
                cerr << "usage: enigma plugboard-file"
                     << " reflector-file (<rotor-file>)* rotor-positions"
                     << endl;
                in.close();
                throw ERROR_OPENING_CONFIGURATION_FILE;
            }
            break;
    }
    return;
}

void checkLastDigit(ifstream& in)
{
        if (in.eof() && !in.fail()) {
            in.clear();
        }
        return;
}