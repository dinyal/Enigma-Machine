#ifndef ENIGMA_H
#define ENIGMA_H
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include <fstream>


class enigma {
    /* --------MEMBER OBJECTS TO BE INITIALISED --------*/
    plugboard pbFilled;
    reflector refFilled;
    int totalRotors;
    Rotor* rotorGroup = nullptr;

    /* ---- ERROR CHECKING FOR POSITION FILE TO BE DONE WITHIN ENIGMA CLASS ---- */

    void checkPositionFile(char const* fileName, int const totalRotors, std::ifstream& in);

    /* -------PRIVATE HELPER ENCRYPTION FUNCTIONS ------*/

    /*Recursive function: rotor will call 'turnposition' function to turn once, 
    and if it hits a notch, recursively runs again for next rotor*/
    void setStart(int const rotorIndex);

    /* Passes value and encrypts through each rotor recursively from right 
    rotor to reflector */
    int scrambleLeft(int const passedValue, int const rotorIndex);

    /* Passes the returned value and encrypts from 
    reflector from left rotor to plugboard */
    int scrambleRight(int const passedValue, int const rotorRight, int const rotorLeft);

    /* Helper functions to convert the input character from char to int 
    and back reducing use of 'A' in encrypt function */
    char charConversion(int const value);
    int intConversion(char const character);

public:
    /*-----CONSTRUCTOR TO INITIALISE PLUGBOARD, REFLECTOR, AND TOTAL ROTORS-----*/
    enigma(int const argc, char** argv);

    /* ---- LOAD ROTOR FUNCTION THAT CHECKS FOR ERRORS FIRST, AND THEN LOADS INTO ARRAY ---- */
    void loadRotors(int const argc, char** argv);

    /* ----- FUNCTION THAT STARTS ENCRYPTION PROCESS ------ */
    void runEncrypt();

    /* ---- DESTRUCTOR  ---- */
    ~enigma();
};

#endif