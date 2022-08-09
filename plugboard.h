#ifndef PLUGBOARD_H
#define PLUGBOARD_H
#include <fstream>

#include "constants.h"
class plugboard
{
    /* --- MEMBER OBJECT CONTAINING ARRAY OF 26 MAPPINGS --- */
    int pConfig[enigmaConst::ALPHABET_SIZE];

    /* ----- ERROR CHECKING FOR PLUGBOARD, CALLED IN CONSTRUCTOR ------ */
    void checkPlugboard(char const* fileName, std::ifstream& in);

public:
    /* --- CONSTRUCTOR FOR CLASS, DOES ERROR CHECK AND THEN FILLS ARRAY --- */
    /* no default constructor needed as will never be initialised without values */
    plugboard(char const* fileName);    

    /* --- MEMBER FUNCTIONS THAT RETURN MAPPED VALUES FROM PLUGBOARD BASED ON CONFIGURATION --- */
    /* Returns the value mapped to the array in position 'passedValue' */
    int pbEncodeLeft(int const passedValue);

    /* returns the array position that the returning value is found in (backwards mapping) */
    int pbEncodeRight(int const returningValue);
};

#endif