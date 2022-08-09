#include <iostream>
#include <cstdio>
#include "errors.h"
#include "enigma.h"
#include "constants.h"
using namespace std;

int main(int argc, char ** argv)
{
    /* First checks that there are the right number of parameters inputted in the command line */
    if (argc < enigmaConst::MIN_ARGS) {
        cerr << "usage: enigma plugboard-file"
             << " reflector-file (<rotor-file>)* rotor-positions" << endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    try {
        /*initialise machine with a plugboard, reflector, and a total rotor constant,
        with relevant error checking being performed for plugboard and reflector*/
        enigma enigmaMachine(argc, argv);

        /*Call load rotor function after initialising, to make sure memory is correctly
        freed if error code in thrown during error checking of position and rotor files */
        enigmaMachine.loadRotors(argc, argv);

        enigmaMachine.runEncrypt();
    }

    /*Returns the thrown integer if there has been an error */
    catch(int errorcode) {
        return errorcode;
    }

    return NO_ERROR;
}