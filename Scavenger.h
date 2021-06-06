/**
 * @file Scavenger.h
 * @brief The Scavenger class declaration file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include "Board.h"
#include <string>

#ifndef SCAVENGER_H
#define SCAVENGER_H

using namespace std;

/**
 * @class Scavenger
 * @brief The Scavenger class declaration.
 */
class Scavenger : public Human {
    public:
    Scavenger(int initRow, int initCol, bool initInfected, Board* thisBoard);
	virtual ~Scavenger();

    //Redefine how a scavenger moves and is displayed
	void move();
	void draw();

	//Setters and getters

        //There are 4 goal points that need coordinates:
        //City gate, 1st ingredient, 2nd ingredient, research facility.
    void setFirstIngredientRowCol(int row, int col);
    void setSecondIngredientRowCol(int row, int col);
    void setGateRowCol(int row, int col);
    void setResearchFacilityRowCol(int row, int col);

        //Set if scavenger has reached those goal points.
    void setHasReachedGate(bool boolean);
    void setHasFirstIngredient(bool boolean);
    void setHasSecondIngredient(bool boolean);
    void setHasReachedResearchFacility(bool boolean);

        //Get if scavenger has reached those goal points.
    bool getHasFirstIngredient();
    bool getHasSecondIngredient();
    bool getHasReachedResearchFacility();

    //Path-finding function to reach those goals. Used by move().
    void doPathFindingMove(int goalRow, int goalCol);


    protected:
    //Booleans to track if scavenger has reached goal points.
    bool hasReachedGate;
    bool hasFirstIngredient;
    bool hasSecondIngredient;
    bool hasReachedResearchFacility;

    //Coordinates of goal points.
    int firstIngredientRow;
    int firstIngredientCol;
    int secondIngredientRow;
    int secondIngredientCol;
    int gateRow;
    int gateCol;
    int researchFacilityRow;
    int researchFacilityCol;
};

#endif // SCAVENGER_H
