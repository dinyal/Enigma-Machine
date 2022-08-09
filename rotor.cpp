#include <iostream>
#include <cstdio>
#include <cassert>
#include "errorcheck.h"
#include "errors.h"
#include "rotor.h"
#include "constants.h"

using namespace std;

/* ------- ERROR CHECKING FUNCTION --------- */
void Rotor::checkRotorFile(char const* fileName, ifstream& in)
{
    /*check if correct file type inputted*/
    checkFileType(fileName, enigmaConst::ROT, in);

    if (!in) {
        cerr << "usage: enigma plugboard-file"
             << " reflector-file (<Rotor-file>)* Rotor-positions" << endl;
        in.close();
        throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    assert(in);

    int value = 0;
    int dupInput = 0;
    int counter = 0;
    
    in >> value;

    /*checks all values for non-numeric and out of range*/
    while(!in.eof()) {
        counter++;
        if (checkCharacter(in)) {
            cerr << "Non-numeric character for mapping in rotor file "
                 << fileName << endl;
            in.close();
            throw NON_NUMERIC_CHARACTER;
        }
        if (checkValidNumber(value)) {
            cerr << "Numeric character out of range in rotor file "
                 << fileName << endl;
            in.close();
            throw INVALID_INDEX;
        }

        /* Assign dupInput the value that the checked value is already mapped to, or -1 if not a duplicate */
        dupInput = checkDuplicate(value, fileName, counter-1, 0); 
        if (!checkOverDigits(counter) && dupInput >= 0) {
            cerr << "Invalid mapping of input " << counter - 1 << " to output "
                 << value << " (output " << value << " is already mapped to from input "
                 << dupInput << ") in rotor file: " << fileName << endl;
            in.close();
            throw INVALID_ROTOR_MAPPING;
        }
        
        /* second check for duplicates specific to nothces, only triggered if counter over 26 */
        /*Pass in constant 26 to correctly index counter, and also
         starting position of loop within checkDup function */
        if (checkOverDigits(counter)
             && checkDuplicate(value, 
                            fileName, 
                            (counter-1) - enigmaConst::ALPHABET_SIZE, 
                            enigmaConst::ALPHABET_SIZE) >= 0) {
            cerr << "Invalid notch value " << value
                 << " (notch value already mapped) in rotor file: "
                 << fileName << endl;
            in.close();
            throw INVALID_ROTOR_MAPPING;
        }

        in >> value;
        checkLastDigit(in);
    }
    
    /*Only check for odd digits if we have read in less than 26 values*/
    if (!checkOverDigits(counter) && checkOdd(counter)) {
        cerr << "Incorrect (odd) number of parameters in rotor file "
             << fileName << endl;
        in.close();
        throw INVALID_ROTOR_MAPPING;
    }

    /* If not odd, but less than 26, output error message */
    if (counter < enigmaConst::ALPHABET_SIZE) {
        cerr << "Not all inputs mapped in rotor file: "
             << fileName << endl;
        in.close();
        throw INVALID_ROTOR_MAPPING;
    }
    return;
}

/* ----- FILL FUNCTIONS THAT LOAD VALUES INTO ROTOR ----- */

void Rotor::fillRotor(char const* fileName, ifstream& in)
{
    int readValue;
    int loopIndex = 0;

    /*No need to check ifstream as it has been checked by previous
    error check function, and passed by reference */
    in >> readValue;

    /*read 26 mapped values into configuration array*/
    for(; loopIndex < enigmaConst::ALPHABET_SIZE; loopIndex++) {
        rConfig[loopIndex] = readValue;
        in >> readValue;
    }

    /*map remaining values to the notches file*/
    loopIndex = 0;
    while (!in.eof()) {
        rNotches[loopIndex] = readValue;
        loopIndex++;
        in >> readValue;
        checkLastDigit(in);
    }
    in.close();
}

void Rotor::fillStart(ifstream& inPosFile)
{
    int startValue;
    inPosFile >> startValue;
    startPos = startValue;
    return;
}

/*---- MEMBER FUNCTIONS THAT OPERATE AND ENCRYPT THROUGH ROTORS ---*/
void Rotor::turnPosition() 
{ 
    /*Modulo to catch when it goes over 25*/
    startPos = (startPos + 1) % enigmaConst::ALPHABET_SIZE;
}

int Rotor::encodeRotorLeft(int const passedValue)
{
    /*Use start position as reference to find actual position in rotor*/
    int actualPosition = (startPos + passedValue) % enigmaConst::ALPHABET_SIZE;

    /* Calculate the 'wiring', or the amount to add/take away from passed value
    based on the mapping in the rotor configuration array */

    int indexToNext = rConfig[actualPosition] - actualPosition;
    /*Add this index to the passed in position, NOT the absolute position 
    calculated prior. Add 26 and modulo to account for negative numbers*/
    return ((passedValue + indexToNext)
             + enigmaConst::ALPHABET_SIZE)
             % enigmaConst::ALPHABET_SIZE;
}

int Rotor::encodeRotorRight(int const passedValue)
{
    /* Using same logic as encoding right to left, convert the passed relative position
    to an absolute position, and loop to find the corresponding mapping */
    int leftValue = (passedValue + startPos) % enigmaConst::ALPHABET_SIZE;
    int rightValue = 0;

    for (;rightValue < enigmaConst::ALPHABET_SIZE; rightValue++) {
        if(leftValue == rConfig[rightValue]) {
            return (rightValue - startPos + enigmaConst::ALPHABET_SIZE) 
                    % enigmaConst::ALPHABET_SIZE;
        }
    }
    return rightValue;
}