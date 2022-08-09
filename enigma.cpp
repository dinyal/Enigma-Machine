#include <iostream>
#include <cstdio>
#include <cctype>
#include <cassert>
#include "enigma.h"
#include "errors.h"
#include "errorcheck.h"
#include "constants.h"

using namespace std;

/* ----------CONSTRUCTOR AND DESTRUCTORS ------------ */
/*Enigma constructor will call the constructor of the plugboard, and the reflector. These will 
handle both the error checking, and the subsequent loading of data into its member objects. */

enigma::enigma(int const argc, char** argv) : pbFilled(argv[enigmaConst::PB]), 
                                              refFilled(argv[enigmaConst::RF]), 
                                              totalRotors(argc - 4) {};

enigma::~enigma() {
    delete [] rotorGroup;
}

/* ------- LOAD FUNCTION FOR ROTORS THAT ERROR CHECKS ROTOR AND POSITION FILES ------ */
/* These are called after construction for memory management purposes. When error thrown,
this insures destructor is still called and memory is appropriately freed */

void enigma::loadRotors(int const argc, char** argv)
{  
    /* Only creates rotors if there are rotor files in command line*/
    if(totalRotors > 0) {
        rotorGroup = new Rotor[totalRotors];
        if (rotorGroup == nullptr) {
            cerr << "No more memory in heap for dynamic allocation for rotors" << endl;
            throw MEMORY_ERROR;
        }
        /* Loops through every rotor file, checking for errors, and 
        fills the rotor if none found */
        for(int file = enigmaConst::ROT, array = 0; file < (argc - 1); file++) {
            ifstream in(argv[file]);
            rotorGroup[array].checkRotorFile(argv[file], in);
            in.clear();
            in.seekg(0);
            rotorGroup[array].fillRotor(argv[file], in);
            array++;
            in.close();
        }
    }

    /* New ifstream to check the position file, and populate starting positions if no errors */
    ifstream inPosFile(argv[argc - 1]);
    checkPositionFile(argv[argc - 1], totalRotors, inPosFile);
    inPosFile.clear();
    inPosFile.seekg(0);

    /* Reads in as many starting positions as rotors */
    for (int array = 0; array < totalRotors; array++) {
        rotorGroup[array].fillStart(inPosFile);
    }
    inPosFile.close();
}

/* --------- POSITION FILE ERROR CHECKING ------------ */

void enigma::checkPositionFile(char const* fileName, int const totalRotors, ifstream& in)
{
    /*check if correct file type inputted*/	
    checkFileType(fileName, enigmaConst::POS, in);

    if (!in) {
        cerr << "usage: enigma plugboard-file "
             << "reflector-file (<Rotor-file>)* rotor-positions" << endl;
        in.close();
        throw ERROR_OPENING_CONFIGURATION_FILE;
    }
    assert(in);

    int value;
    int counter = 0;
    /* First checks for non numeric using in.fail, then checks 
    to make sure position is in range */
    in >> value;
    while (!in.eof()) {
        counter++;
        if (checkCharacter(in)) {
            cerr << "Non-numeric character in rotor positions file "
                 << fileName << endl;
            in.close();
            throw NON_NUMERIC_CHARACTER;
        }
        else if (checkValidNumber(value)) {
            cerr << "Numeric character out of range in rotor positions file "
                 << fileName << endl;
            in.close();
            throw INVALID_INDEX;
        }
        in >> value;
        checkLastDigit(in);
    }

    /*if not enough positions for Rotors, return an error code*/
    if (counter < (totalRotors))
    {
        cerr << "No starting position for rotor "
             << counter << " in rotor position file: "
             << fileName << endl;
        in.close();
        throw NO_ROTOR_STARTING_POSITION;
    }
    return;
}

/* ------- ENCRYPTION FUNCTIONS --------- */

void enigma::runEncrypt()
{
    char input;
    cin >> ws >> input;

    while(!cin.eof()) {
        /* Returns error code the moment a non-alphabetic or lower case value is found */
        if (!isalpha(input)) {
            cerr << input
                 <<  " is not a valid input character "
                 << "(input characters must be upper case letters A-Z)!" << endl;
            throw INVALID_INPUT_CHARACTER;
        }
        else if (!isupper(input)) {
            cerr << input
                 << " is not a valid input character " 
                 << "(input characters must be upper case letters A-Z)!" << endl;
            throw INVALID_INPUT_CHARACTER;
        } 
        else {
            int rotorRight = totalRotors - 1;
            int rotorLeft = 0;
            int pbOutput;

            /*set rotor starting positions first*/
            setStart(rotorRight);

            /*figure out the first passed value from plugboard*/
            pbOutput = pbFilled.pbEncodeLeft(intConversion(input));

            /*First pass right to left, returning a reflected value. Then pass left to right,
            returning a final encoded value to be passed into the plugboard*/
            int rightLeftOutput = refFilled.reflectBack(scrambleLeft(pbOutput, rotorRight));
            int leftRightOutput = scrambleRight(rightLeftOutput, rotorRight, rotorLeft);
            cout << charConversion(pbFilled.pbEncodeRight(leftRightOutput));
            cin >> ws >> input;
        }
    }
    return;
}

void enigma::setStart(int const rotorRight)
{
    /* Exit function if there are no rotors or if no rotors left to turn */
    if (rotorRight < 0) {
        return;
    }

    rotorGroup[rotorRight].turnPosition();
    int index = 0;

    /* If a notch is encountered, recursively call this function to turn next rotor */
    for(; index < enigmaConst::ALPHABET_SIZE; index++) {
        if (rotorGroup[rotorRight].getNotch(index) == -1) {
            return;
        }
        else if (rotorGroup[rotorRight].getPosition() 
                == rotorGroup[rotorRight].getNotch(index)) {
            setStart(rotorRight - 1);
            return;
        }
    }
}

int enigma::scrambleLeft(int const passedValue, int const rotorRight)
{
    /* Base case : if no rotors, just return passed value unchanged */
    if (rotorRight < 0) {
        return passedValue;
    }

    int nextPassedValue = rotorGroup[rotorRight].encodeRotorLeft(passedValue);

    /*base case: if this is the last rotor, return the value to rflector*/
    if(rotorRight == 0) {
        return nextPassedValue;
    }
    else {
        return scrambleLeft(nextPassedValue, rotorRight - 1);
    }
}

int enigma::scrambleRight(int const passedValue, int const rotorRight, int const rotorLeft)
{
    /* Base case: if no rotors, just return passed value unchanged */
    if(rotorRight < 0) {
        return passedValue;
    }

    int nextPassedValue = rotorGroup[rotorLeft].encodeRotorRight(passedValue);

    /* Base case, if we have reached rightmost rotor, return encoded value */
    if (rotorLeft == rotorRight)
    {
        return nextPassedValue;
    }
    else {
        return scrambleRight(nextPassedValue, rotorRight, rotorLeft + 1);
    }
}

char enigma::charConversion(int const value)
{
    return value + 'A';
}

int enigma::intConversion(char const character)
{
    return character - 'A';
}