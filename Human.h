/**
 * @file Human.h
 * @brief The Human class declaration file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include "Board.h"
#include <string>

#ifndef HUMAN_H
#define HUMAN_H

using namespace std;

/**
 * @class Human
 * @brief The Human class declaration.
 */
class Human {
    public:
    Human(int initRow, int initCol, bool initInfected, Board* thisBoard);
	virtual ~Human();

    //Basic move and draw functions for Human and derived classes
	virtual void move();
	virtual void draw();

	//Setters and getters for Human class and derived classes
	virtual void getLocation(int& row, int& col);
	virtual void setLocation(int row, int col);
	virtual void setInfected();
    virtual void setUnInfected();
	virtual bool isInfected();
    virtual string getObjectType();

    //Setters and getters to be used exlusively by the Scavenger class
    virtual void setFirstIngredientRowCol(int row, int col);
    virtual void setSecondIngredientRowCol(int row, int col);
    virtual void setGateRowCol(int row, int col);
    virtual void setResearchFacilityRowCol(int row, int col);
    virtual void setHasReachedGate(bool boolean);
    virtual void setHasFirstIngredient(bool boolean);
    virtual void setHasSecondIngredient(bool boolean);
    virtual void setHasReachedResearchFacility(bool boolean);
    virtual bool getHasFirstIngredient();
    virtual bool getHasSecondIngredient();
    virtual bool getHasReachedResearchFacility();

    //Move function used exlusively by the Scavenger class
    virtual void doPathFindingMove(int goalRow, int goalCol);


    protected:
    //Track whether or not this human is infected.
    bool infected;     

    //The row and column where this human is on the board.
	int row, col;      

    //String used to keep track of the class to which an object belongs. Possible values: "Human", "Doctor", "Scavenger". 
    string objectType; 

    //Pointer to the board so the human can ask the board whether the human can move to a given location on the board.
	Board *board;      
};

#endif // HUMAN_H
