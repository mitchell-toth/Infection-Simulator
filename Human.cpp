/**
 * @file Human.cpp
 * @brief The Human class implementation file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

#include "Human.h"
#include "conio.h"

using namespace std;

/**
 * @brief The Human class constructor.
 * This function initializes the row, col, infected, and board variables.
 * As well as the object type ("Human").
 *
 * @param initRow The initial human row location.
 * @param initCol The initial human column location.
 * @param initInfected Whether the human is initially infected.
 * @param theBoard A pointer to the board (used to ask board whether a proposed move is ok).
 */
Human::Human(int initRow, int initCol, bool initInfected, Board* theBoard) {
    //Initialize to parameters.
    row = initRow;
    col = initCol;
    infected = initInfected;
    board = theBoard;

    //Initialize string of object type.
    objectType = "Human";
}


/**
 * @brief The Human class destructor.
 */
Human::~Human() {
    // Nothing to do
}


/**
 * @brief Have the human try to move.
 * To know whether it is ok to move to some position (r,c), ask the board
 * whether the position is ok. E.g., "if( board->tryMove(r,c) ) ..."
 * If the move is ok, then update the human's row and column to reflect the move.
 */
void Human::move() {
    int rowDelta, colDelta;

    //Generate a +/- 2 row and column delta.
    rowDelta=rand()%5-2;
    colDelta=rand()%5-2;

    //Ask the board whether the move is okay.
    if(board->tryMove(row+rowDelta, col+colDelta)) {
        row+=rowDelta;
        col+=colDelta;
    }
}


/**
 * @brief Get the human's current row/col location.
 * Returns the human's current row/column location via the reference parameters.
 * @param[out] currentRow The human's current row.
 * @param[out] currentCol The human's current column.
 */
void Human::getLocation(int& currentRow, int& currentCol) {
    currentRow = row;
    currentCol = col;
}


/**
 * @brief Set the human's row/col location.
 * Sets the human's current row/column location to the parameter values.
 * @param[in] newRow The human's new row location.
 * @param[in] newCol The human's new column location.
 */
void Human::setLocation(int newRow, int newCol) {
    row=newRow;
    col=newCol;
}


/**
 * @brief Sets this human to be infected.
 * Sets this human object's state to infected.
 */
void Human::setInfected() {
    infected = true;
}


/**
 * @brief Sets this human to be healed and uninfected.
 * Sets this human object's state to uninfected.
 */
void Human::setUnInfected() {
    infected = false;
}


/**
 * @brief Reports whether this human is infected.
 * @return Whether this human object is infected.
 */
bool Human::isInfected() {
    return infected;
}


/**
 * @brief Draws the human.
 * Draws the human at the current row/col location on the screen.
 * Draws light red if infected, green otherwise.
 * Remember that the first conio row=1, and the first conio col=1.
 */
void Human::draw() {
    cout << conio::gotoRowCol(row+1,col+1);

    //Infected
    if(infected) {
        cout << conio::bgColor(conio::LIGHT_RED);
        cout << '@' << conio::resetAll() << flush;
    }
    //Healthy
    else {
        cout << conio::bgColor(conio::LIGHT_GREEN);
        cout << '@' << conio::resetAll() << flush;
    }
}


/**
 * @brief Returns the class name/ data type of the object.
 * Used to distinguish between regular humans, doctors, and scavengers.
 * @return The type (class name) of the object.
 */
string Human::getObjectType() {
    return objectType;
}


/**
 * @brief Sets the row and column of the first vaccine ingredient.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the first ingredient.
 * @param[in] col The column of the first ingredient.
 */
void Human::setFirstIngredientRowCol(int row, int col) {
}


/**
 * @brief Sets the row and column of the second vaccine ingredient.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the second ingredient.
 * @param[in] col The column of the second ingredient.
 */
void Human::setSecondIngredientRowCol(int row, int col) {
}


/**
 * @brief Sets the row and column of the middle of the city gate.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the city gate.
 * @param[in] col The column of the city gate.
 */
void Human::setGateRowCol(int row, int col) {
}


/**
 * @brief Sets the row and column of a point in the research facility.
 * Used exclusively by the derived Scavenger class.
 * Used as a goal point to pathfind to, as well as track if the scavenger is touching this goal point.
 * @param[in] row The row of the research facility.
 * @param[in] col The column of the research facility.
 */
void Human::setResearchFacilityRowCol(int row, int col) {
}


/**
 * @brief Moves the object toward a goal point.
 * Used exclusively by the derived Scavenger class in its move() function.
 * @param[in] goalRow The row of a goal point.
 * @param[in] goalCol The column of a goal point.
 */
void Human::doPathFindingMove(int goalRow, int goalCol) {
}


/**
 * @brief Setter for the Scavenger boolean "hasReachedGate".
 * Sets the value to the value of "boolean".
 * Used exclusively by the derived Scavenger class.
 * @param[in] boolean The value to be assigned to "hasReachedGate".
 */
void Human::setHasReachedGate(bool boolean) {
}


/**
 * @brief Setter for the Scavenger boolean "hasFirstIngredient".
 * Sets the value to the value of "boolean".
 * Used exclusively by the derived Scavenger class.
 * @param[in] boolean The value to be assigned to "hasFirstIngredient".
 */
void Human::setHasFirstIngredient(bool boolean) {
}


/**
 * @brief Setter for the Scavenger boolean "hasSecondIngredient".
 * Sets the value to the value of "boolean".
 * Used exclusively by the derived Scavenger class.
 * @param[in] boolean The value to be assigned to "hasSecondIngredient".
 */
void Human::setHasSecondIngredient(bool boolean) {
}


/**
 * @brief Setter for the Scavenger boolean "hasReachedResearchFacility".
 * Sets the value to the value of "boolean".
 * Used exclusively by the derived Scavenger class.
 * @param[in] boolean The value to be assigned to "hasReachedResearchFacility".
 */
void Human::setHasReachedResearchFacility(bool boolean) {
}


/**
 * @brief Getter for the Scavenger boolean "hasFirstIngredient".
 * Used exclusively by the derived Scavenger class.
 * @return By default returns false.
 */
bool Human::getHasFirstIngredient() {
    return false;
}


/**
 * @brief Getter for the Scavenger boolean "hasSecondIngredient".
 * Used exclusively by the derived Scavenger class.
 * @return By default returns false.
 */
bool Human::getHasSecondIngredient() {
    return false;
}


/**
 * @brief Getter for the Scavenger boolean "hasReachedResearchFacility".
 * Used exclusively by the derived Scavenger class.
 * @return By default returns false.
 */
bool Human::getHasReachedResearchFacility() {
    return false;
}
