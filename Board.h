#ifndef BOARD_H
#define BOARD_H

/**
 * @file Board.h
 * @brief The Board class declaration file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

//----------------------------------------------------
// "forward" declaration of Board. Tell the compiler that this will be
// defined properly further "forward" in the program. This is needed because
// both classes reference each other. Otherwise, when include "Human.h" and 
// the compiler finds a reference to Board, it will complain.
//----------------------------------------------------
class Board;

#include "Human.h"
#include <string>

using namespace std;

/**
 * @class Board
 * @brief The Board class declaration.
 */
class Board {
    public:
    Board(int numRows, int numCols, int numHumans, int numDoctors); 
    ~Board();                 

    //Main function that runs the simulation.
    void run();

    // Function that lets human objects know whether a move is okay.
    bool tryMove(int row, int col); 

    //Setting maximum human count and board dimensions.
    //Used to initialize the logical boards.
    static const int MAX_HUMAN_COUNT = 100; 
    static const int MAX_NUM_ROWS = 20;
    static const int MAX_NUM_COLS = 80;


    protected:
    //-------------Functions------------------

    //Go through and process infection status
    void processInfection();  

    //Tells whether all humans are infected
    bool allInfected();       

    //Tells whether one human is next to another
    bool isNextTo(Human* h1, Human* h2); 


    //Making the logical boards. Initializing the logical "landscapeBoard".
        //Makes all board positions "EMPTY"
    void initializeLandscapeBoard();

        //Manages various function calls to continue filling up the logical board.
    void makeLandscape();

        //Makes the city wall.
    void makeCityWall();

        //Makes the research facility in top right corner.
    void makeResearchFacility();

        //Randomly places the 2 vaccine ingredients on logical board.
    void makeTwoVaccineIngredients();

        //Mark the floor of the research facility on logical board.
    void markResearchFacility();


    //Drawing the logical board onto the screen.
    void drawLandscape();

    //Placing humans in and outside the city walls.
    void populateCity();
    void populateOutsideOfCity();

    //Remove the city wall.
    void destroyCity();
    
    //Manage opening and closing the city wall gate.
    void openCityGate();
    void closeCityGate();

    //See if a point on the board is within certain boundaries.
    bool isWithinCity(int row, int col);
    bool isWithinResearchFacility(int row, int col);

    //Randomly select a human within the city to become the scavenger.
    void selectScavenger();

    //Check if scavenger has reached any goal points. If so, control scavenger's progress booleans.
    void checkOnScavenger();

    //Determine if scavenger is within 1 space of a goal point.
    bool isNextToAGoal(int goalRow, int goalCol);

    //Update statistic percentages of vaccine research and city wall health.
    void updateResearchProgress();
    void updateCityWallHealth();

    //Based on current time and game status, print out statistics and a game note.
    void printStatistics(int currentTime);


    //End-of-the-game functions:

        //Vaccine research reached 100%, now cure all infected.
    void applyVaccine();
        //Scavenger died and vaccine is hopeless, create more infected and change doctors into regular humans.
    void makeInfectionWorse();



    //-------------Variables------------------

    //The main logical board that keeps track of the city wall, research facility, and ingredients.
    char landscapeBoard[MAX_NUM_ROWS][MAX_NUM_COLS];

    //The main logical board that keeps track of human objects.
    Human* humans[MAX_HUMAN_COUNT];

    //Initial variables to create board and run the simulation.
    int numHumans;            // Num humans
    int numDoctors;           // Num doctors
    int numInfected;          // Num humans infected
    int currentTime;          // Current time in simulation
    int numRows;              // Number of rows in board
    int numCols;              // Number of cols in board
    int uSleepTime;           // Num microseconds to sleep between updates

    //Boolean to keep track of if the city wall should be removed or not.
    bool cityIsDestroyed;

    //Coordinates of various structures (city, research facility) and goal points.
    int cityStartingColumn;
    int cityEndingRow;
    int researchFacilityEndingRow;
    int researchFacilityStartingCol;
    int firstIngredientRow;
    int firstIngredientCol;
    int secondIngredientRow;
    int secondIngredientCol;
    int gateRow;
    int gateCol;
    int researchFacilityRow;
    int researchFacilityCol;

    //Index for scavenger in "humans" logical board.
    int scavengerPos;

    //Numerical values for scavenger and city wall "health".
    int scavengerHealth;
    int cityWallHealth;

    //Numerical value for vaccine research progress.
    float vaccineResearchProgress;

    //Game note to be displayed based on currentTime and game status.
    string gameNote;

    //End-of-the-game booleans:
        //Keeps track of if the vaccine reached 100% and was applied.
    bool vaccineApplied;
        //Keeps track of if more infected were released to kill off the remaining humans.
    bool infectionWorsened;

    //Booleans to keep track of if the scavenger has reached the 1st or 2nd ingredient. 
    bool firstIngredientAttained;
    bool secondIngredientAttained;
	
	private:
	int random();
};

#endif //#ifndef BOARD_H
