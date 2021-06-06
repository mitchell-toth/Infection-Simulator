/**
 * @file Doctor.cpp
 * @brief The Doctor class implementation file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "Human.h"
#include "conio.h"
#include "Doctor.h"

using namespace std;


/**
 * @brief The Doctor class constructor.
 * This function initializes the row, col, infected, and board variables.
 * As well as the object type ("Doctor").
 *
 * @param initRow The initial Doctor row location.
 * @param initCol The initial Doctor column location.
 * @param initInfected The initial state of being infected or not
 * @param theBoard A pointer to the board (used to ask board whether a proposed move is ok).
 */
Doctor::Doctor(int initRow, int initCol, bool initInfected, Board* theBoard) : Human(initRow, initCol, initInfected, theBoard) {

    //Initalize string of object type.
    objectType = "Doctor";
}


/**
 * @brief The Doctor class destructor.
 */
Doctor::~Doctor() {
    // Nothing to do
}


/**
 * @brief Draws the Doctor.
 * Draws the doctor at the current row/col location on the screen.
 * Draws light red if infected, cyan otherwise.
 * Remember that the first conio row=1, and first conio col=1.
 */
void Doctor::draw() {
    cout << conio::gotoRowCol(row+1,col+1);

    //Infected
    if (infected) {
        cout << conio::bgColor(conio::LIGHT_RED);
        cout << '@' << conio::resetAll() << flush;
    }
    //Healthy
    else {
        cout << conio::bgColor(conio::CYAN);
        cout << '+' << conio::resetAll() << flush;
    }
}
