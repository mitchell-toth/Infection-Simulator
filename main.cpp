/**
 * @file main.cpp
 * @brief Contains the main function that starts the infection simulation.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 **/

#include <iostream>
#include <cstdlib>
#include <time.h>

#include "Board.h"

using namespace std;

/**
 * @fn main()
 * @brief Main function that starts the simulation running.
 * The main function does the following:
 * (1) seeds the random number generator
 * (2) creates a board object that is 20 rows by 80 columns and contains 18 total humans with 2 of those being doctors.
 * (3) starts the simulation running by calling the board's run function.
 **/
int main() {

    //Seed the random number generator.
    srand( time(NULL) );

    //Parameters: rows, cols, numHumans, numDoctors.
    Board board(20, 80, 18, 2);

    //Run the simulation.
    board.run();

    return 0;
}

