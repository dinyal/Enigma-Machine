#ifndef REFLECTOR_H
#define REFLECTOR_H
#include <fstream>
#include "constants.h"
class reflector
{
    /* --- MEMBER OBJECT CONTAINING PAIRING FOR REFLECTOR ---*/
    int refConfig[enigmaConst::ALPHABET_SIZE];

    /* --- ERROR CHECKING FUNCTION CALLED AS PART OF CONSTRUCTOR. FILE ONLY LOADED IF IT PASSES THIS TEST --- */
    void checkReflectorFile(char const* fileName, std::ifstream& in);

public:
    /* --- CONSTRUCTOR FOR CLASS, DOES ERROR CHECK AND THEN FILLS ARRAY --- */
    /* no default constructor needed as will never be initialised without values */
    reflector(char const* fileName);

    /* --- FUNCTION THAT MIMICS THE REFLECTING BEHAVIOUR OF A REFLECTOR, RETURNING PAIRED VALUE OF PASSED VALUE --- */
    int reflectBack(int const passedValue);
};

#endif