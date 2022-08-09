#ifndef ROTOR_H
#define ROTOR_H
#include <fstream>
#include "constants.h"
class Rotor
{
    /* ----- MEMBER OBJECTS TO STORE 26 ARRAY MAPPING, MAXIMUM 26 NOTHCES, AND A START POSITION ---- */
    int rConfig[enigmaConst::ALPHABET_SIZE];
    int rNotches[enigmaConst::ALPHABET_SIZE];
    int startPos;
 
public:
    /*------- DEFAULT CONSTRUCTOR THAT FILLS NOTHCES ARRAY WITH -1 TO SIGNIFY EMPTY NOTCH ----- */
    Rotor(){
        for (int i = 0; i < enigmaConst::ALPHABET_SIZE; i++) {
            rNotches[i] = -1;
        }
    };

    /*-------- ERROR CHECKING FUNCTION ------*/
    void checkRotorFile(char const* fileName, std::ifstream& in);

    /* ------ MEMBER FUNCTIONS THAT FILL ROTOR OBJECT WITH FILE VALUES ------- */
    void fillStart(std::ifstream& in);
    void fillRotor(char const* fileName, std::ifstream& in);

    /* ----- MEMBER FUNCTIONS TO OPERATE ROTORS ----- */
    /* Mimics the turning of the rotor, incrementing starting position by one */
    void turnPosition();
    int encodeRotorLeft(int const passedValue);
    int encodeRotorRight(int const passedValue);

    /* --- GETTER FUNCTIONS USED IN ENIGMA CLASS SETSTART FUNCTION --- */
    int getPosition() {return startPos;};
    int getNotch(int index) {return rNotches[index]; };
};

#endif