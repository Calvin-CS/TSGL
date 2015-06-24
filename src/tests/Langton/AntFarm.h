/*
 * AntFarm.h
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <omp.h>

#include <tsgl.h>
#include "LangtonAnt.h"

using namespace tsgl;

class LangtonAnt;  //Forward Declaration

/*!
 *
 */
class AntFarm {
private:
    bool* filled;
    bool shading;
    bool inParallel;
    void moveAnt(int j);
public:
    LangtonAnt** ants;
    int width, height, size, cap;
    Canvas* can;

    /*!
     * \brief Explicitly constructs an AntFarm object.
     * \details Explicit constructor for the AntFarm class.
     * \param w The width of the AntFarm object.
     * \param h The height of the AntFarm object.
     * \param s The size of the AntFarm object (how many ants are going to be in the AntFarm object).
     * \param can Pointer to the Canvas to draw to.
     */
    AntFarm(int w, int h, int s, Canvas* can);

    /*!
     * \brief Destroy an AntFarm object.
     * \details Destructor for the AntFarm class.
     * \return Frees up any allocated memory to an AntFarm object.
     */
    ~AntFarm();

    /*!
     * \brief Add an ant.
     * \details Adds a LangtonAnt to the AntFarm with the specified arguments.
     * \param x The x coordinate of the LangtonAnt.
     * \param y The y coordinate of the LangtonAnt.
     * \param r Red component of the color of the LangtonAnt.
     * \param g Green component of the color of the LangtonAnt.
     * \param b Blue component of the color of the LangtonAnt.
     * \param d The direction of the LangtonAnt.
     */
    void addAnt(int x, int y, int r, int g, int b, int d);

    /*!
     *
     */
    void moveAnts();

    /*!
     *
     */
    void setShading(bool b);

    /*!
     *
     */
    void setParallel(bool b);
};

#endif /* ANTFARM_H_ */
