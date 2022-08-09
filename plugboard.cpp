#include <iostream>
#include <cstdio>
#include <cassert>
#include "errors.h"
#include "errorcheck.h"
#include "plugboard.h"
#include "constants.h"

using namespace std;

/* ----- CONSTRUCTOR ----- */
plugboard::plugboard(char const* fileName){
    ifstream in(fileName);

    checkPlugboard(fileName, in);
    in.clear();
    in.seekg(0);

    /* First fill with default values if no mapping, to be replaced below */
    for(int i = 0; i < enigmaConst::ALPHABET_SIZE; i++) {
        pConfig[i] = i;
    }

    /* Read in 2 values at a time (knowing there are no odd numbers 
    due to error checking), and map one to the other */
    int firstValue, secondValue;
    in >> firstValue;
    while (!in.eof()) {
        in >> secondValue;
        pConfig[firstValue] = secondValue;
        pConfig[secondValue] = firstValue;
        in >> firstValue;
    }

    in.close();
}

void plugboard::checkPlugboard(char const* fileName, ifstream& in)
{
    checkFileType(fileName, enigmaConst::PB, in);

    if (!in) {
        cerr << "usage: enigma plugboard-file "
             << "reflector-file (<rotor-file>)* rotor-positions" << endl;
        in.close();
        throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    assert(in);

    /* Read in values from file into 'value'. Return error codes
     when corresponding check shows an error.*/
    int value;
    int dupInput = -1;
    int counter = 0;

    in >> value;
    while (!in.eof()) {
        counter++;
        /* Checks to see if file ever exceeds 26, the maximum number of mappings that are possible */
        if (checkOverDigits(counter)) {
            cerr << "Incorrect number of parameters in plugboard file "
                 << fileName << endl;
            in.close();
            throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
        }
        /*In the specified order, first checks if values gotten are are non digit (in which case fail 
        flag triggers), or not in range using helper function found in errorcheck.h*/
        if (checkCharacter(in)) {
            cerr << "Non-numeric character in plugboard file "
                 << fileName << endl;
            in.close();
            throw NON_NUMERIC_CHARACTER;
        }

        if (checkValidNumber(value)) {
            cerr << "Numeric character out of range in plugboard file "
                 << fileName << endl;
            in.close();
            throw INVALID_INDEX;
        }

        /* Different error message to be outputted for notch duplication*/
        if ((dupInput = checkDuplicate(value, fileName, counter-1, 0)) >= 0) {
            cerr << "Invalid mapping of the value " << value << " (value " << value
                 << " is already mapped to from input " << dupInput
                 << ") in plugboard file: " << fileName << endl;
            in.close();
            throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
        }
        in >> value;
        checkLastDigit(in);
    }

    /*Final error case being checked is if the number of parameters is odd */
    if (checkOdd(counter)) {
        cerr << "Incorrect number of parameters in plugboard file "
             << fileName << endl;
        in.close();
        throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }
    return;
}

int plugboard::pbEncodeLeft(int const passedValue)
{
    return pConfig[passedValue];
}

int plugboard::pbEncodeRight(int const returningValue)
{
    for (int outputValue = 0; outputValue < enigmaConst::ALPHABET_SIZE; outputValue++) {
        if (pConfig[outputValue] == returningValue) {
            return outputValue;
        }
    }
    return -1;
}