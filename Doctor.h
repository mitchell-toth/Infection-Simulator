/**
 * @file Doctor.h
 * @brief The Doctor class declaration file.
 *
 * @author Alec Houseman
 * @author Mitchell Toth
 * @date May 2019
 */

#include "Board.h"

#ifndef DOCTOR_H
#define DOCTOR_H

using namespace std;

/**
 * @class Doctor
 * @brief The Doctor class declaration.
 */
class Doctor : public Human {
    public:
    Doctor(int initRow, int initCol, bool initInfected, Board* thisBoard);
    ~Doctor();

    //Redefine how a doctor is to be displayed
    void draw();
};

#endif // DOCTOR_H
