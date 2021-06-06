/**
 * @file Scavenger.cpp
 * @brief The Scavenger class implementation file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include <cstdlib>
#include <iostream>
#include <string>
//Needed for distance formula ( sqrt(), pow() )
#include <cmath>

#include "Human.h"
#include "conio.h"
#include "Scavenger.h"

using namespace std;

/**
 * @brief The Scavenger class constructor.
 * This function initializes the row, col, infected, and board variables.
 * As well as the object type ("Scavenger") and various progress booleans:
 * hasReachedGate, hasFirstIngredient, hasSecondIngredient, hasReachedResearchFacility.
 *
 * @param initRow the initial scavenger row location.
 * @param initCol the initial scavenger column location.
 * @param initInfected whether the scavenger is initially infected.
 * @param theBoard a pointer to the board (used to ask board whether a proposed move is ok).
 */
Scavenger::Scavenger(int initRow, int initCol, bool initInfected, Board* theBoard) : Human(initRow, initCol, initInfected, theBoard) {

    //Intialize string of object type.
    objectType = "Scavenger";

    //Initialize boolean progress variables (in terms of the goal points).
    hasReachedGate = false;
    hasFirstIngredient = false;
    hasSecondIngredient = false;
    hasReachedResearchFacility = false;
}

/**
 * @brief The Scavenger class destructor.
 */
Scavenger::~Scavenger() {
    // Nothing to do
}

/**
 * @brief Have the scavenger try to move.
 * Based on various progress booleans, have the scavenger pathfind to a specified goal point.
 * Uses the "doPathFindingMove(goalRow, goalCol)" method.
 * To know whether it is ok to move to some position (r,c), ask the board
 * whether the position is ok. E.g., "if( board->tryMove(r,c) ) ..."
 * If the move is ok, then update the scavenger's row and column to reflect the move.
 */
void Scavenger::move() {
    //Pathfind to gate.
    if (! hasReachedGate) {
        doPathFindingMove(gateRow, gateCol);
    }

    //Pathfind to first ingredient.
    else if (! hasFirstIngredient) {
        doPathFindingMove(firstIngredientRow, firstIngredientCol);
    }

    //Pathfind to second ingredient.
    else if (! hasSecondIngredient) {
        doPathFindingMove(secondIngredientRow, secondIngredientCol);
    }

    //Pathfind to research facility.
    else if (! hasReachedResearchFacility) {
        doPathFindingMove(researchFacilityRow, researchFacilityCol);
    }

    //If reached research facility, then the scavenger is done.
    else if (hasReachedResearchFacility) {
        //Don't move.
    }

    //None of the other moves worked, do a random move.
    else {
        int rowDelta, colDelta;

        //Generate a +/- 2 row and column delta.
        rowDelta=rand()%5-2;
        colDelta=rand()%5-2;

        //Ask the board whether the move is valid.
        if(board->tryMove(row+rowDelta, col+colDelta)) {
            row+=rowDelta;
            col+=colDelta;
        }
    }
}


/**
 * @brief Draws the Scavenger.
 * Draws the Scavenger at the current row/col location on the screen.
 * Draws the scavenger yellow and marks the scavenger with an 'S'.
 * Remember that the first conio row=1, and first conio col=1.
 */
void Scavenger::draw() {
    cout << conio::gotoRowCol(row+1,col+1);
    cout << conio::bgColor(conio::YELLOW);
    cout << 'S' << conio::resetAll() << flush;
}


/**
 * @brief Sets the row and column of the first vaccine ingredient.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the first ingredient.
 * @param[in] col The column of the first ingredient.
 */
void Scavenger::setFirstIngredientRowCol(int row, int col) {
    firstIngredientRow = row;
    firstIngredientCol = col;
}


/**
 * @brief Sets the row and column of the second vaccine ingredient.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the second ingredient.
 * @param[in] col The column of the second ingredient.
 */
void Scavenger::setSecondIngredientRowCol(int row, int col) {
    secondIngredientRow = row;
    secondIngredientCol = col;
}


/**
 * @brief Sets the row and column of the middle of the city gate.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the city gate.
 * @param[in] col The column of the city gate.
 */
void Scavenger::setGateRowCol(int row, int col) {
    gateRow = row;
    gateCol = col;
}


/**
 * @brief Sets the row and column of a point in the research facility.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the research facility.
 * @param[in] col The column of the research facility.
 */
void Scavenger::setResearchFacilityRowCol(int row, int col) {
    researchFacilityRow = row;
    researchFacilityCol = col;
}


/**
 * @brief Moves the scavenger toward a goal point.
 * @param[in] goalRow The row of a goal point.
 * @param[in] goalCol The column of a goal point.
 */
void Scavenger::doPathFindingMove(int goalRow, int goalCol) {
    int rowDelta, colDelta;

    //Calculate current distance to goal point.
    float oldDistanceToGoal = sqrt((pow(row-goalRow,2)) + (pow(col-goalCol,2)));

    //Declare variable for storing calculation of new distance to goal point.
    float newDistanceToGoal;


    //Try left
    rowDelta=0;
    colDelta=(rand()%2)-2;
    newDistanceToGoal = sqrt((pow((row+rowDelta)-goalRow,2)) + (pow((col+colDelta)-goalCol,2)));
    
        //Is move valid and effective?
    if (board->tryMove(row+rowDelta, col+colDelta) && (newDistanceToGoal <= oldDistanceToGoal)) {
        row+=rowDelta;
        col+=colDelta;
    }


    //Try down
    rowDelta=(rand()%2)+1;
    colDelta=0;
    newDistanceToGoal = sqrt((pow((row+rowDelta)-goalRow,2)) + (pow((col+colDelta)-goalCol,2)));

        //Is move valid and effective?
    if (board->tryMove(row+rowDelta, col+colDelta) && (newDistanceToGoal <= oldDistanceToGoal)) {
        row+=rowDelta;
        col+=colDelta;
        return;
    }

    //Try up
    rowDelta=(rand()%2)-2;
    colDelta=0;
    newDistanceToGoal = sqrt((pow((row+rowDelta)-goalRow,2)) + (pow((col+colDelta)-goalCol,2)));

        //Is move valid and effective?
    if (board->tryMove(row+rowDelta, col+colDelta) && (newDistanceToGoal <= oldDistanceToGoal)) {
        row+=rowDelta;
        col+=colDelta;
        return;
    }

    //Try right
    rowDelta=0;
    colDelta=(rand()%2)+1;
    newDistanceToGoal = sqrt((pow((row+rowDelta)-goalRow,2)) + (pow((col+colDelta)-goalCol,2)));

        //Is move valid and effective?
    if (board->tryMove(row+rowDelta, col+colDelta) && (newDistanceToGoal <= oldDistanceToGoal)) {
        row+=rowDelta;
        col+=colDelta;
        return;
    }

    //If none of those worked, do a random move.
    rowDelta=rand()%5-2;
    colDelta=rand()%5-2;

    //Ask the board if move is allowed.
    if(board->tryMove(row+rowDelta, col+colDelta)) {
        row+=rowDelta;
        col+=colDelta;
    }
}


/**
 * @brief Setter for the Scavenger boolean "hasReachedGate".
 * Sets the value to the value of "boolean".
 * @param[in] boolean The value to be assigned to "hasReachedGate".
 */
void Scavenger::setHasReachedGate(bool boolean) {
    hasReachedGate = boolean;
}


/**
 * @brief Setter for the Scavenger boolean "hasFirstIngredient".
 * Sets the value to the value of "boolean".
 * @param[in] boolean The value to be assigned to "hasFirstIngredient".
 */
void Scavenger::setHasFirstIngredient(bool boolean) {
    hasFirstIngredient = boolean;
}


/**
 * @brief Setter for the Scavenger boolean "hasSecondIngredient".
 * Sets the value to the value of "boolean".
 * @param[in] boolean The value to be assigned to "hasSecondIngredient".
 */
void Scavenger::setHasSecondIngredient(bool boolean) {
    hasSecondIngredient = boolean;
}


/**
 * @brief Setter for the Scavenger boolean "hasReachedResearchFacility".
 * Sets the value to the value of "boolean".
 * @param[in] boolean The value to be assigned to "hasReachedResearchFacility".
 */
void Scavenger::setHasReachedResearchFacility(bool boolean) {
    hasReachedResearchFacility = boolean;
}


/**
 * @brief Getter for the Scavenger boolean "hasFirstIngredient".
 * @return The boolean value of "hasFirstIngredient".
 */
bool Scavenger::getHasFirstIngredient() {
    return hasFirstIngredient;
}


/**
 * @brief Getter for the Scavenger boolean "hasSecondIngredient".
 * @return The boolean value of "hasSecondIngredient".
 */
bool Scavenger::getHasSecondIngredient() {
    return hasSecondIngredient;
}


/**
 * @brief Getter for the Scavenger boolean "hasReachedResearchFacility".
 * @return The boolean value of "hasReachedResearchFacility".
 */
bool Scavenger::getHasReachedResearchFacility() {
    return hasReachedResearchFacility;
}
