/*
 * testAlphaLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#ifndef LANGTONANT_H_
#define LANGTONANT_H_

#include "AntFarm.h"

using namespace tsgl;

class AntFarm;  //Forward Declaration

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

/*!
 *
 */
class LangtonAnt {
public:
    int myX, myY, myRed, myGreen, myBlue, myAlpha, myDir;
    AntFarm* myFarm;
public:
    /*!
     * \brief Explicitly constructs a LangtonAnt object.
     * \details Explicit constructor for the LangtonAnt class.
     * \param x The x coordinate of the LangtonAnt object.
     * \param y The y coordinate of the LangtonAnt object.
     * \param r Red component for the color of the LangtonAnt object.
     * \param g Green component for the color of the LangtonAnt object.
     * \param b Blue component for the color of the LangtonAnt object.
     * \param d The direction of the LangtonAnt object.
     * \param p Pointer to the AntFarm object that the LangtonAnt object belongs to.
     */
    LangtonAnt(int x, int y, int r, int g, int b, int d, AntFarm* p);

    /*!
     *
     */
    void move();

    /*!
     * \brief Change the color of a LangtonAnt object.
     * \details Specifying a new red, green, and blue component allows you to change the
     * color of the LangtonAnt object.
     * \param r Red component of the new color for the LangtonAnt object.
     * \param g Green component of the new color for the LangtonAnt object.
     * \param b Blue component of the new color for the LantonAnt object.
     */
    void changeColor(int r, int g, int b);

    /*!
     * \brief Change the color of a LangtonAnt object.
     * \details Same as changeColor(r, g, b) but instead of passing the red, green, and
     * blue components seperately you are passing a ColorFloat struct that contains all three.
     * \param c A ColorFloat struct containing the new red, green, and blue components for the new
     * color of the LangtonAnt object.
     */
    void changeColor(ColorFloat c);

    /*!
     * \brief Set the alpha component of the color.
     * \details Set the alpha component of the color for a LangtonAnt object.
     * \param a Alpha component to give to the LangtonAnt object.
     */
    void setAlpha(int a);
};

#endif /* LANGTONANT_H_ */
