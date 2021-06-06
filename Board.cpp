/**
 * @file Board.cpp
 * @brief The Board class implementation file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>

// When writing a class implementation file, you must "#include" the class
// declaration file.
#include "Board.h"
#include "Doctor.h"
#include "Scavenger.h"

// We also use the conio namespace contents, so must "#include" the conio declarations.
#include "conio.h"


//Preprocessor macros for filling up "landscapeBoard".
#define WALL '#'
#define EMPTY 'E'
#define FIRST_INGREDIENT '1'
#define SECOND_INGREDIENT '2'
#define RESEARCH_FLOOR ' '



/**
 * @brief The Board class constructor, responsible for intializing a Board object.
 * The Board constructor is responsible for initializing all the Board object variables.
 * It also initializes various game status variables and scavenger-related variables.
 *
 * @param rows The number of rows to make the board.
 * @param cols The number of columns to make the board.
 * @param numberOfHumans The number of humans to place on the board.
 * @param numberOfDoctors The number of doctors to place on the board.
 */
Board::Board(int rows, int cols, int numberOfHumans, int numberOfDoctors) {
    //Initialize from parameters.
    numHumans = numberOfHumans;
    numDoctors = numberOfDoctors;
    numRows = rows;
    numCols = cols;

    //Initialize time variables.
    currentTime = 0;
    uSleepTime = 250000;

    //Initialize number of infected.
    numInfected=0;

    //Initialize scavenger-related variables.
    firstIngredientAttained = false;
    secondIngredientAttained = false;
    scavengerPos = -1;
    scavengerHealth = 100;

    //Initialize end-of-the-game-related variables.
    vaccineApplied = false;
    infectionWorsened = false;

    //Initialize logical board.
    initializeLandscapeBoard();

    //Initialize status of the city.
    cityIsDestroyed = false;
    
    //Initialize numerical progress variables.
    vaccineResearchProgress = 0;
    cityWallHealth = 100;
}

/**
 * @brief The Board class destructor.
 * The Board destructor is responsible for any last-minute cleaning 
 * up that a Board object needs to do before being destroyed. In this case,
 * it needs to return all the memory borrowed for creating the Human objects.
 */
Board::~Board() {
    for(int pos=0; pos<numHumans; ++pos) {
        delete humans[pos];
    }
}

/**
 * @brief The primary function that runs the simulation.
 * Initializes and fills the logical boards, creates human objects, infects some humans, then runs simulation until the game progresses to the end, or until the time exceeds 400.
 * Uses a for loop to control the simulation.
 * With each iteration, the logical boards and screen are updated, and updated statistics are printed out.
 */
void Board::run() {

    //Initialize and fill logical landscapeBoard.
    makeLandscape();
    markResearchFacility();

    //Fill the logical "humans" board with people.
    populateCity();
    populateOutsideOfCity();

    //Used at end of game to "break" out of loop.
    int timeToStopAt = -1;



    //-------The loop that runs the simulation---------
    for(currentTime=0; currentTime<=400; ++currentTime) {

        //Clear screen before every new time unit.
        cout << conio::clrscr() << flush;

        //Tell each human to try moving.
        for(int pos=0; pos<numHumans; ++pos) {
            humans[pos]->move();
        }

        //Deal with infection propagation.
        processInfection();

        //Check status of scavenger.
        checkOnScavenger();

        //Update numerical progress variables for the vaccine and the city wall.
        updateResearchProgress();
        updateCityWallHealth();

        //Place the research facility walls on the logical landscapeBoard.
        makeResearchFacility();

        //Display the logical landscapeBoard.
        drawLandscape();


        //At time 30, open the city gate.
        if (currentTime == 30) {
            openCityGate();
        }
        //At time 35, select a scavenger.
        else if (currentTime == 35) {
            selectScavenger();
        }


        //Endgame events and details.
        if (vaccineResearchProgress == 100) {
            if (! vaccineApplied) {
                //Cure all infected.
                applyVaccine();
            }
            else {
                if (timeToStopAt == -1) {
                    //Set "timeToStopAt".
                    timeToStopAt = currentTime + 15;
                }
            }
        }
        else if (cityWallHealth == 0 && currentTime > 150 && scavengerHealth == 0) {
            if (! infectionWorsened) {
                //Create more infected.
                makeInfectionWorse();
            }
            else {
                if (allInfected() && timeToStopAt == -1) {
                    //Set "timeToStopAt".
                    timeToStopAt = currentTime + 15;
                }
            }
        }


        //Tell each human to draw itself on board with updated infection status.
        for(int pos=0; pos<numHumans; ++pos) {
            humans[pos]->draw();
        }

        //Print statistics.
        printStatistics(currentTime);
        cout << conio::resetAll() << flush;
        
        //Sleep specified microseconds
        usleep(uSleepTime);

        //If end of simulation events, then break.
        if (currentTime == timeToStopAt) {
            break;
        }

    }//-------End of loop---------

    //Position the cursor so prompt shows up on its own line
    cout << endl;
}


/**
 * @brief Determines whether or not all humans are infected.
 * @return If even one human is uninfected, returns false. Otherwise, returns true.
 */
bool Board::allInfected() {
    for(int i=0; i<numHumans; ++i) {
        if(humans[i]->getObjectType() != "Doctor" && humans[i]->isInfected() == false) return false;
    }

    return true;
}


/**
 * @brief The function that handles one infection cycle to determine what new infections are present.
 * For each pair of adjacent humans in the simulation, processInfection() makes sure that if one is infected, the other becomes infected as well.
 * But if one of these people is a doctor, the other person will become healed.
 * And if one of them is a scavenger and comes into contact with an infected, the scavenger loses 1/4 of its health.
 */
void Board::processInfection() {
    for( int i=0; i<numHumans; ++i ) {
        for( int j=i+1; j<numHumans; ++j ) {
            if( isNextTo(humans[i], humans[j]) ){

                //HEAL
                if (humans[i]->getObjectType() == "Doctor" && humans[j]->isInfected()) {
                    //Doctor + Infected = heal.
                    humans[j]->setUnInfected();
                }
                else if (humans[j]->getObjectType() == "Doctor" && humans[i]->isInfected()) {
                    //Infected + Doctor = heal.
                    humans[i]->setUnInfected();
                }

                //INFECT
                else if( humans[i]->isInfected() && humans[j]->isInfected()==false ) {
                    //Deal with scavenger
                    if (humans[j]->getObjectType()=="Scavenger") {
                        //Hurt the scavenger.
                        scavengerHealth -= 25;
                        if (scavengerHealth <= 0) {
                            //If scavenger dead, replace with infected human.
                            int row,col;
                            humans[j]->getLocation(row,col);
                            delete humans[j];
                            humans[j] = new Human(row,col,true,this);
                        }
                    }
                    else {
                        //Infected + Human = infect.
                        humans[j]->setInfected();
                    }
                } 
                else if ( humans[j]->isInfected() && humans[i]->isInfected()==false ) {
                    //Deal with scavenger
                    if (humans[i]->getObjectType()=="Scavenger") {
                        //Hurt the scavenger.
                        scavengerHealth -= 25;
                        if (scavengerHealth <= 0) {
                            //If scavenger dead, replace with infected human.
                            int row,col;
                            humans[i]->getLocation(row,col);
                            delete humans[i];
                            humans[i] = new Human(row,col,true,this);
                        }
                    }
                    else {
                        //Human + Infected = infect.
                        humans[i]->setInfected();
                    }
                }
            }
        }
    }

    // Reset the board 'numInfected' count and recount how many are infected.
    numInfected = 0;
    for( int i=0; i<numHumans; ++i ) {
        if( humans[i]->isInfected() ) {
            numInfected++;
        }
    }
}

/**
 * @brief The function that determines whether a particular move can happen.
 * If the move would go off the board, or land on the same position as another human, the function returns false (do not move). Otherwise, it returns true (ok to proceed).
 * @param[in] row The row the human wishes to move to.
 * @param[in] col The column the human wishes to move to.
 * @return Whether the human calling this function may move to the specified row and column.
 */
bool Board::tryMove(int row, int col) {
    int tryRow, tryCol=-1;

    // If off board, the move is not permitted.
    if( row<0 || row>=numRows || col<0 || col>=numCols ) return false;

    // Else if another human is on the same location, the move is not permitted.
    // Or if trying to go through a landscape object (ex: a wall).
    for(int i=0; i<numHumans; ++i) {
        humans[i]->getLocation(tryRow, tryCol);
        //Trying to move on top of another human is not permitted.
        if( row==tryRow && col==tryCol ) return false;
        //Research floor IS permitted.
        else if (landscapeBoard[row][col] == RESEARCH_FLOOR) return true;
        //But anything else is not permitted.
        else if (landscapeBoard[row][col] != EMPTY) return false;
    }

    // No problems, so the move is permitted
    return true;
}

/**
 * @brief The function that determines whether two humans are on adjacent squares.
 * @param h1 A pointer to first human object.
 * @param h2 A pointer to second human object.
 * @return Whether or not h1 and h2 are on adjacent squares.
 */
bool Board::isNextTo(Human *h1, Human* h2) {
    //Get human location information
    int h1Row, h1Col;
    h1->getLocation(h1Row, h1Col);
    int h2Row, h2Col;
    h2->getLocation(h2Row, h2Col);

    //Return whether h1 and h2 are on adjacent squares in any direction 
    //(horizontally, vertically, diagonally).
    return abs(h1Row-h2Row)<=1 && abs(h1Col-h2Col)<=1;
}


/**
 * @brief Initialize the logical board that governs the background "landscape".
 * Sets all logical board locations to "EMPTY" (see preprocessor macros).
 */
void Board::initializeLandscapeBoard() {
    for (int row=0; row<numRows; row++) {
        for (int col=0; col<numCols; col++) {
            landscapeBoard[row][col] = EMPTY;
        }
    }
}


/**
 * @brief General function that gets called to fill up the landscapeBoard.
 * Calls makeCityWall(), makeResearchFacility(), and makeTwoVaccineIngredients().
 */
void Board::makeLandscape() {
    makeCityWall();
    makeResearchFacility();
    makeTwoVaccineIngredients();
}


/**
 * @brief The primary function for displaying the logical board contents on the screen.
 * Iterates through the landscapeBoard and checks the value at each cell.
 * Depending on that value, display a specified character and color.
 * Afterwards, go to the research facility and label it with "VACCINE LAB".
 */
void Board::drawLandscape() {

    /*
    //Shows where the "gate" and "research facility" goal points are:
    cout << conio::gotoRowCol(gateRow+1, gateCol+1);
    cout << conio::bgColor(conio::WHITE) << 'X' << conio::resetAll();
    cout << conio::gotoRowCol(researchFacilityRow+1, researchFacilityCol+1);
    cout << conio::bgColor(conio::WHITE) << 'X' << conio::resetAll();
    */

    for (int row=0; row<numRows; row++) {
        for (int col=0; col<numCols; col++) {
            //WALL
            if (landscapeBoard[row][col] == WALL) {
                cout << conio::gotoRowCol(row+1,col+1);
                cout << WALL;
            }
            //FIRST INGREDIENT
            else if (landscapeBoard[row][col] == FIRST_INGREDIENT) {
                cout << conio::gotoRowCol(row+1,col+1);
                cout << conio::bgColor(conio::YELLOW) << conio::fgColor(conio::BLACK) <<FIRST_INGREDIENT << conio::resetAll();
            }
            //SECOND INGREDIENT
            else if (landscapeBoard[row][col] == SECOND_INGREDIENT) {
                cout << conio::gotoRowCol(row+1,col+1);
                cout << conio::bgColor(conio::MAGENTA) << conio::fgColor(conio::BLACK) << SECOND_INGREDIENT << conio::resetAll();
            }
            //RESEARCH FLOOR
            else if (landscapeBoard[row][col] == RESEARCH_FLOOR) {
                cout << conio::gotoRowCol(row+1,col+1);
                cout << conio::bgColor(conio::LIGHT_GRAY) << RESEARCH_FLOOR << conio::resetAll();
            }
        }
    }

    //Label the research facility with "VACCINE LAB".
    cout << conio::gotoRowCol(1,numCols-11) << conio::fgColor(conio::BLACK) << conio::bgColor(conio::LIGHT_GRAY);
    cout << "VACCINE LAB";

    //Make sure cursor is reset back to normal.
    cout << conio::resetAll() << flush;
}


/**
 * @brief Destroys the city wall.
 * Iterates through the landscapeBoard and sets to "EMPTY" everything that is a "WALL".
 * This removes the research facility walls as well, but they are redrawn elsewhere.
 * Finally, set "cityIsDestroyed" boolean to true.
 */
void Board::destroyCity() {
    for (int row=0; row<numRows; row++) {
        for (int col=0; col<numCols; col++) {
            //If WALL, then make it EMPTY.
            if (landscapeBoard[row][col] == WALL) {
                landscapeBoard[row][col] = EMPTY;
            }
        }
    }
    cityIsDestroyed = true;
}


/**
 * @brief Places humans within the city limits.
 * Adds about a third of the allowed humans to the "humans" array.
 * Only places them within the city, and none of them are infected.
 * Also places numDoctors doctors in the city, and keeps track of the number of doctors placed.
 */
void Board::populateCity() {
    int row, col;

    //Used to iterate over numDoctors and change it mid-loop.
    int tempNumDoctors = numDoctors;
    numDoctors = 0;

    //Iterate over about 1/3 of numHumans.
    for(int pos=0; pos<int(numHumans/3); ++pos) {
        row = random() % numRows; //row will be in range(0, numRows-1).
        col = (random() % (numCols-cityStartingColumn-2))+cityStartingColumn+2; //col will be in range(cityStartingColumn+2, numCols-1).

        //Make numDoctors doctors (these are the first to be made).
        if (pos < tempNumDoctors) {
            humans[pos] = new Doctor(row,col,false,this);
            numDoctors++;
        }
        else {
            // Creates 'Human' objects and sets the array pointers to point at them.
            // Create and initialize another Human. 
            // Parameters are row on board, col on board, initially infected, and a pointer to this board object ('this').
            humans[pos] = new Human(row, col, false, this); 
        }
    }
}


/**
 * @brief Places humans outside the city limits.
 * Fills up the remainder of the "humans" array according to the numHumans variable.
 * Uses a while(true) loop and "break" statements to determine a random location outside of the city limits.
 * Only places them outside the city, and about a third are set to be infected.
 * The rest are uninfected.
 */
void Board::populateOutsideOfCity() {
    int row, col;

    //Start at 1/3 of numHumans, go to end of numHumans.
    for(int pos=int(numHumans/3); pos<numHumans; ++pos) {
        while (true) {
            row = random() % numRows; //row will be in range(0, numRows-1).
            col = random() % numCols; //col will be in range(0, numCols-1).
           
            //If outside of city, break out of loop.
            if (row > cityEndingRow || col < cityStartingColumn-2) {
                break;
            }
        }

        //Infect first few humans.
        if (pos<=numHumans-int(numHumans/3)) {
            humans[pos] = new Human(row, col, true, this); 
        }
        //Make the rest healthy.
        else {
            humans[pos] = new Human(row, col, false, this); 
        }
    }
}


/** 
 * @brief Makes the city wall, marking it on the logical landscapeBoard.
 * Chooses a spot roughly at the middle of the board (column-wise) and creates a doubly thick wall going down to the bottom of the board.
 * Uses the marker "WALL".
 * When done, it initializes the values of cityStartingColumn, cityEndingRow, gateRow, and gateCol. 
 */
void Board::makeCityWall() {
    //Make outside wall
    for (int row=0; row<numRows; row++) {
        landscapeBoard[row][(int(numCols/2))+8] = WALL;
        landscapeBoard[row][(int(numCols/2))+9] = WALL;
    }

    //Initialize variable values based on placement.
    cityStartingColumn = int(numCols/2)+9;
    cityEndingRow = numRows-1;
    gateRow = (int(numRows/2))-1;
    gateCol = (int(numCols/2))+9;
}


/** 
 * @brief Makes the research facility, marking it on the logical landscapeBoard.
 * Creates portions of doubly thick walls at the upper right corner of the board.
 * Uses the marker "WALL".
 * When done, it initializes the values of researchFacilityEndingRow, researchFacilityStartingCol, researchFacilityRow, and researchFacilityCol. 
 */
void Board::makeResearchFacility() {
    //Make research facility
    for (int row=0; row<3; row++) {
        landscapeBoard[row][numCols-15] = WALL;
        landscapeBoard[row][numCols-14] = WALL;
    }
    for (int col=numCols-5; col<numCols; col++) {
        landscapeBoard[5][col] = WALL;
        landscapeBoard[6][col] = WALL;
    }

    //Initialize variable values based on placement.
    researchFacilityEndingRow = 4;
    researchFacilityStartingCol = numCols-13;
    researchFacilityRow = 3;
    researchFacilityCol = numCols-9;
}


/** 
 * @brief Randomly places the vaccine ingredients, marking them on the logical landscapeBoard.
 * Uses a while(true) loop and "break" statements to control valid placement.
 * Only places the ingredients outside of the city limits.
 * Uses the markers "FIRST_INGREDIENT" and "SECOND_INGREDIENT" respectively.
 * When done, it initializes the values of firstIngredientRow, firstIngredientCol, secondIngredientRow, and secondIngredientCol.
 */
void Board::makeTwoVaccineIngredients() {
    int row, col;

    //FIRST INGREDIENT
    while (true) {
        row = random() % numRows; // row will be in range(0, numRows-1)
        col = random() % numCols; // col will be in range(0, numCols-1)
        if ((row > cityEndingRow+2 || col < cityStartingColumn-2) && landscapeBoard[row][col] == EMPTY) {
            break;
        }
    }

    //Initialize variable values based on placement.
    landscapeBoard[row][col] = FIRST_INGREDIENT;
    firstIngredientRow = row;
    firstIngredientCol = col;


    //SECOND INGREDIENT
    while (true) {
        row = random() % numRows;       // row will be in range(0, numRows-1)
        col = random() % numCols;  // col will be in range(0, numCols-1)
        if ((row > cityEndingRow+2 || col < cityStartingColumn-2) && landscapeBoard[row][col] == EMPTY) {
            break;
        }
    }

    //Initialize variable values based on placement.
    landscapeBoard[row][col] = SECOND_INGREDIENT;
    secondIngredientRow = row;
    secondIngredientCol = col;
}


/**
 * @brief Opens the city gate by creating a gap in the middle of the city wall.
 * Goes to the wall's starting columns and creates a space of "EMPTY" cells halfway down the board.
 */
void Board::openCityGate() {
    for (int row=(int(numRows/2))+1; row>(int(numRows/2))-4; row--) {
        landscapeBoard[row][(int(numCols/2))+8] = EMPTY;
        landscapeBoard[row][(int(numCols/2))+9] = EMPTY;
    }
}


/**
 * @brief Closes the city gate by filling the gap in the middle of the city wall.
 * Goes to the wall's starting columns and fills a space halfway down the board with "WALL".
 */
void Board::closeCityGate() {
    for (int row=(int(numRows/2))+1; row>(int(numRows/2))-4; row--) {
        landscapeBoard[row][(int(numCols/2))+8] = WALL;
        landscapeBoard[row][(int(numCols/2))+9] = WALL;
    }
}


/**
 * @brief Marks the floor of the research facility on the logical landscapeBoard.
 * Uses "researchFacilityEndingRow" and "researchFacilityEndingCol".
 * Marks the board with "RESEARCH_FLOOR".
 */
void Board::markResearchFacility() {
    for (int row=0; row<=researchFacilityEndingRow; row++) {
        for (int col=researchFacilityStartingCol; col<numCols; col++) {
            landscapeBoard[row][col] = RESEARCH_FLOOR;
        }
    }
}


/**
 * @brief Randomly selects a human within the city to be a scavenger.
 * Iterate through the "humans" board and select a human based on certain critera.
 * Criteria: 1) Is not infected, 2) Is a human (not a doctor), 3) Is within the city.
 * Changes selected human to scavenger by deleting humans[pos] and replacing it with a pointer to a Scavenger object.
 * Afterwards, give the scavenger all the necessary information for pathfinding.
 */
void Board::selectScavenger() {
    int pos;
    int row, col;
    while (true) {
        pos = random() % numHumans;
        if (humans[pos]->isInfected()==false) {
            if (humans[pos]->getObjectType()=="Human") {
                humans[pos]->getLocation(row,col);
                if (isWithinCity(row,col)) {
                    scavengerPos = pos;
                    delete humans[scavengerPos];
                    //Make a scavenger.
                    humans[scavengerPos] = new Scavenger(row,col,false,this);
                    //Communicate first ingredient coordinates.
                    humans[scavengerPos]->setFirstIngredientRowCol(firstIngredientRow,firstIngredientCol);
                    //Communicate second ingredient coordinates.
                    humans[scavengerPos]->setSecondIngredientRowCol(secondIngredientRow,secondIngredientCol);
                    //Communicate gate goal point coordinates.
                    humans[scavengerPos]->setGateRowCol(gateRow,gateCol);
                    //Communicate research facility goal point coordinates.
                    humans[scavengerPos]->setResearchFacilityRowCol(researchFacilityRow, researchFacilityCol);
                    break;
                }
            }
        }
    } 
}


/**
 * @brief Checks if a given row and column are within the city limits.
 * @param[in] row The given row, most likely an object's current row.
 * @param[in] col The given column, most likely an object's current column.
 * @return If the row and column are within the city limits or not.
 */
bool Board::isWithinCity(int row, int col) {
    if (row>cityEndingRow) {
        return false;
    }
    if (col<cityStartingColumn) {
        return false;
    }
    return true;
}


/**
 * @brief Checks if a given row and column are within the limits of the research facility.
 * @param[in] row The given row, most likely an object's current row.
 * @param[in] col The given column, most likely an object's current column.
 * @return If the row and column are within the limits of the research facility or not.
 */
bool Board::isWithinResearchFacility(int row, int col) {
    if (row>researchFacilityEndingRow) {
        return false;
    }
    if (col<researchFacilityStartingCol) {
        return false;
    }
    return true;
}


/**
 * @brief Checks the status of the scavenger and updates values accordingly.
 * Uses the member function "isNextToAGoal(goalRow,goalCol)" to see if the scavenger has reached a certain goal point.
 * If so, tell scavenger about this, allowing the scavenger to move on to the next goal point.
 */
void Board::checkOnScavenger() {

    if (scavengerPos != -1) {
        
        //Scavenger reached gate.
        if (isNextToAGoal(gateRow,gateCol)) {
            humans[scavengerPos]->setHasReachedGate(true); 
        }

        //Scavenger reached first ingredient.
        else if (isNextToAGoal(firstIngredientRow,firstIngredientCol)) {
            humans[scavengerPos]->setHasFirstIngredient(true); 
            landscapeBoard[firstIngredientRow][firstIngredientCol] = EMPTY;
        }

        //Scavenger reached second ingredient.
        else if (isNextToAGoal(secondIngredientRow,secondIngredientCol) && humans[scavengerPos]->getHasFirstIngredient()) {
            humans[scavengerPos]->setHasSecondIngredient(true); 
            humans[scavengerPos]->setHasReachedGate(false); 
            landscapeBoard[secondIngredientRow][secondIngredientCol] = EMPTY;
        }

        //Scavenger reached research facility.
        else if (isNextToAGoal(researchFacilityRow,researchFacilityCol) && humans[scavengerPos]->getHasFirstIngredient() && humans[scavengerPos]->getHasSecondIngredient()) {
            humans[scavengerPos]->setHasReachedResearchFacility(true); 
        }
    }
}


/**
 * @brief Checks if scavenger is within 1 space of a specified goal point.
 * @param[in] goalRow The row of a goal point.
 * @param[in] goalCol The column of a goal point.
 * @return If scavenger is within 1 space of the specified goal or not.
 */
bool Board::isNextToAGoal(int goalRow, int goalCol) {
    int scavengerRow, scavengerCol;

    //Define scavengerRow and scavengerCol
    humans[scavengerPos]->getLocation(scavengerRow, scavengerCol);

    return abs(scavengerRow-goalRow)<=1 && abs(scavengerCol-goalCol)<=1;   
}


/**
 * @brief Updates the vaccine research progress numerical value.
 * If the infection has worsened (near end of game), then research stops.
 * If the scavenger has retrieved both the ingredients and has made it back to the research facility, increase research progress by a lot.
 * Otherwise, increase research progress by a random, small amount.
 * If research progress goes beyond 100, set it to 100 (cap it at 100).
 */
void Board::updateResearchProgress() {

    if (! infectionWorsened) {

        //If there's a scavenger.
        if (scavengerPos != -1) {

            int scavengerRow, scavengerCol;
            //Get scavenger's position.
            humans[scavengerPos]->getLocation(scavengerRow, scavengerCol);

            //If scavenger has both ingredients and is in research facility.
            if (humans[scavengerPos]->getHasFirstIngredient() && humans[scavengerPos]->getHasSecondIngredient() && isWithinResearchFacility(scavengerRow, scavengerCol)) {
                vaccineResearchProgress += rand()%10;
            }
        }

        //Based on a random number, increase vaccineResearchProgress.
        int randNum = rand()%5;
        switch (randNum) {
            case 0:
                vaccineResearchProgress += 0.8;
                break;
            case 1:
                vaccineResearchProgress += 0.4;
                break;
            case 2:
                vaccineResearchProgress += 0.3;
                break;
            case 3:
                vaccineResearchProgress += 0.2;
                break;
            case 4:
                vaccineResearchProgress += 0;
                break;
        }

        //Cap research progress at 100%.
        if (vaccineResearchProgress > 100) {
            vaccineResearchProgress = 100;
        }
    }
}


/**
 * @brief Updates the cityWallHealth numerical value.
 * Decrease the wall's health by a random, small amount.
 * If its health goes below 0, call "destroyCity()" and set health to 0 (cap it at 0).
 */
void Board::updateCityWallHealth() {
    int randNum = rand()%4;
    switch (randNum) {
        case 0:
            cityWallHealth -= 2;
            break;
        case 1:
            cityWallHealth -= 1;
            break;
        case 2:
        case 3:
            cityWallHealth -= 0;
            break;
    }

    //Cap cityWallHealth at 0%.
    if (cityWallHealth <= 0) {
        cityWallHealth = 0;

        //Destroy the city wall.
        if (! cityIsDestroyed) {
            destroyCity();
        }
    }
}


/**
 * @brief Print out useful, neat statistics relating to simulation status.
 * Based on a variety of simulation status variables and currentTime, set "gameNote" accordingly.
 * Print out various statistics along with gameNote.
 * @param[in] currentTime The current simulation time tick.
 */
void Board::printStatistics(int currentTime) {
    //Time < 15.
    if (currentTime < 15) {
        gameNote = "Emergency vaccine research has begun!";
    }
    //Time < 30.
    else if (currentTime < 30) {
        gameNote = "Vaccine ingredients are needed!";
    }
    //Time < 45 and there's a scavenger.
    else if (currentTime < 45 && scavengerPos != -1) {
        gameNote = "A scavenger has been selected!";
    }
    //There's a scavenger and it has reached the first ingredient.
    if (scavengerPos != -1 && humans[scavengerPos]->getHasFirstIngredient()) {
        gameNote = "The first ingredient has been grabbed!";
    }
    //There's a scavenger and it has reached the second ingredient.
    if (scavengerPos != -1 && humans[scavengerPos]->getHasSecondIngredient()) {
        gameNote = "Both ingredients have been grabbed!";
    }
    //There's a scavenger and it has reached the research facility.
    if (scavengerPos != -1 && humans[scavengerPos]->getHasReachedResearchFacility()) {
        gameNote = "Ingredients applied! Vaccine research accelerated!";
    }
    //If vaccine reached 100% and was applied.
    if (vaccineApplied) {
        gameNote = "Vaccine created! Humans WIN!";
    }
    //If scavenger died and infectionWorsened == true.
    else if (infectionWorsened) {
        gameNote = "The infection has spread! Infected WIN!";
    }
    //If city wall is dead.
    else if (cityWallHealth == 0) {
        gameNote = "The city wall has crumbled!";
    }
    //If scavenger is dead.
    else if (scavengerHealth == 0) {
        gameNote = "The scavenger has perished!";
    }
    //Time >= 385.
    if (currentTime >= 385) {
        gameNote = "The infected WIN!";
    }

    //Print game note.
    cout << conio::gotoRowCol(numRows+3, 1)
         << "   -- " << gameNote << " --   ";

    //Print vaccine percentage.
    cout << conio::gotoRowCol(numRows+4,1)
         << "Vaccine:" << vaccineResearchProgress << "%";

    //Print time info and various object counts.
    cout << conio::gotoRowCol(numRows+5, 1) 
         << "Time:" << currentTime 
         << " | Humans:" << numHumans
         << " | Doctors:" << numDoctors
         << " | Infected:" << numInfected;

    //Print city wall health.
    cout << conio::gotoRowCol(numRows+6, 1)
         << "CityWallHealth:" << cityWallHealth << "%";

    //If there's a scavenger, print scavenger health.
    if (scavengerPos != -1) {
        cout << conio::gotoRowCol(numRows+7, 1)
        << "ScavengerHealth:" << scavengerHealth << "%";
    }

    //Flush it all out there to the screen.
    cout << flush;
}


/**
 * @brief Implement the vaccine to cure all infected humans.
 * If research progress has reached 100%, cure all humans.
 * Iterate through all humans in "humans" array and call "setUnInfected()".
 * Finally, set boolean value of "vaccineApplied" to true.
 */
void Board::applyVaccine() {
    if (! vaccineApplied) {
        //Set all humans to uninfected
        for (int pos=0; pos<numHumans; pos++) {
            humans[pos]->setUnInfected();
        }
        vaccineApplied = true;
    }
}


/**
 * @brief Creates 2x the amount of infected humans so that all remaining humans will become infected much quicker.
 * Adds infected humans to "humans" array and updates "numHumans" value.
 * Finally, sets boolean value of "infectionWorsened" to true.
 */ 
void Board::makeInfectionWorse() {
    if (! infectionWorsened) {
        int row, col;

        //Remove doctors.
        for (int pos=0; pos<numHumans; pos++) {
            if (humans[pos]->getObjectType() == "Doctor") {
                humans[pos]->getLocation(row,col);
                delete humans[pos];
                humans[pos] = new Human(row,col,false,this);
            }
        }

        //Place more infected.
        for (int pos=numHumans; pos < numHumans+30; pos++) {
            row = random() % numRows; //row will be in range(0, numRows-1)
            col = random() % numCols; //col will be in range(0, numCols-1)
            humans[pos] = new Human(row, col, true, this); 
        }

        //Update numHumans.
        numHumans+=30;

        infectionWorsened = true;
    }
}


/**
 * @brief Helper to return a random float.
 */ 
int Board::random() {
	return rand();
}
