#ifndef ERRORCHECK_H
#define ERRORCHECK_H
#include <string>
#include <fstream>

/* ----ERROR HELPERS USED ACROSS THE FILE --- */

/*Checks for duplicates by checking all values prior to input value to see if it 
has already appeared*/
int checkDuplicate(int const value, 
                    std::string const& fileName, 
                    int const count, 
                    int const seekValue);

/*Checks to see if there is a non-numeric value being read in*/
bool checkCharacter(std::ifstream& in);

/*Checks to see if the value is between 0 and 25 */
bool checkValidNumber(int const value);

/* checks if there are an odd number of values */
bool checkOdd(int const count);

/* Checks if over 26 values */
bool checkOverDigits(int const counter);

/*check if correct file type inputted, by looking at the last characters of the file name.
This makes sure that files have been passed in the right order. */	
void checkFileType(std::string const& fileName, 
                    int const type, 
                    std::ifstream& in);

/* Edge case: if there is no whitespace char after the last value, clear eof flag so that
you can error check/load the final value, instead of exiting the loop. Will exit loop after */
void checkLastDigit(std::ifstream& in);

#endif