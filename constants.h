#ifndef CONSTANTS_H
#define CONSTANTS_H

/* So that these constants have a type, I have 
used namespace as opposed to #define for constants */
namespace enigmaConst
{
    /*--- CONSTANT TO REPRESENT 26 --- */
    int const ALPHABET_SIZE = 26;

    /*--- CONSTANTS FOR DIFFERENT FILES, USED FOR CHOOSING RIGHT FILE,
    AND FOR ERROR CHECK HELPER FUNCTION --- */
    int const PB = 1;
    int const RF = 2;
    int const ROT = 3;
    int const POS = 4;

    /*--- MINIMUM NUMBER OF FILES ---*/
    int const MIN_ARGS = 4;
}

#endif