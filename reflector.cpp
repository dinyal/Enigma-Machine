#include <iostream>
#include <cstdio>
#include <cassert>
#include "errors.h"
#include "errorcheck.h"
#include "reflector.h"
#include "constants.h"
using namespace std;

/* ----- CONSTRUCTOR THAT CHECKS FOR ERRORS AND LOADS MAPPING --- */
reflector::reflector(char const* fileName) {
    ifstream in(fileName);
    checkReflectorFile(fileName, in);
    in.clear();
    in.seekg(0);

    /* reads in 2 values at a time, mapping one to the other in the array */
    int firstValue, secondValue;
    in >> firstValue;
    while (!in.eof()) {
        in >> secondValue;
        refConfig[firstValue] = secondValue;
        refConfig[secondValue] = firstValue;
        in >> firstValue;
    }
    in.close();
}

/* ----- ERROR CHECKING FUNCTION ------ */
void reflector::checkReflectorFile(char const* fileName, ifstream& in)
{
    /*check if correct file type inputted*/
    checkFileType(fileName, enigmaConst::RF, in);

    /*check file opens*/
    if (!in) {
        cerr << "usage: enigma plugboard-file "
             << "reflector-file (<rotor-file>)* rotor-positions" << endl;
        in.close();
        throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    assert(in);

    int value;
    int dupInput = -1;
    int counter = 0;

    in >> value;
    while (!in.eof()) {
        counter++;
        /*Throws the error code if too many parameters */
        if (checkOverDigits(counter)) {
            cerr << "Incorrect (odd) number of parameters in reflector file "
                 << fileName << endl;
            in.close();
            throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
        }

        /* In order, checks for integer, then checks if in range, then checks duplicate.
        NOTE: for more detailed explanation of these helper functions, see errorcheck.cpp, and 
        plugboard.cpp */
        if (checkCharacter(in)) {
            cerr << "Non-numeric character in reflector file "
                 << fileName << endl;
            in.close();
            throw NON_NUMERIC_CHARACTER;
        }
        if (checkValidNumber(value)) {
            cerr << "Numeric character out of range in reflector file "
                 << fileName << endl;
            in.close();
            throw INVALID_INDEX;
        }

        if ((dupInput = checkDuplicate(value, fileName, counter-1, 0)) >= 0) {
            cerr << "Invalid mapping of the value " << value << " (value " << value
                 << " is already mapped to from input " << dupInput
                 << ") in reflector file: " << fileName << endl;
            in.close();
            throw INVALID_REFLECTOR_MAPPING;
        }

        in >> value;
        checkLastDigit(in);
    }
    
    /* Checks if it's odd or too few parameters*/
    if (checkOdd(counter)) {
        cerr << "Incorrect (odd) number of parameters in reflector file "
             << fileName << endl;
        in.close();
        throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }

    /*Catches case where there are less than 13 pairs*/
    if (counter != enigmaConst::ALPHABET_SIZE) {
        cerr << "Insufficient number of mappings in reflector file: "
             << fileName << endl;
        in.close();
        throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }

    return;        
}

/* ------ REFLECTS BACK THE PAIRED VALUE ------ */
int reflector::reflectBack(int const passedValue)
{
    return refConfig[passedValue];
}