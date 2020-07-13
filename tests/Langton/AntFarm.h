/*
 * AntFarm.h
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <omp.h>

#include <tsgl.h>
#include "LangtonAnt.h"

using namespace tsgl;

class LangtonAnt;  //Forward Declaration

/*!
 * \class AntFarm
 * \brief Display one or more of Langton's Ant!
 * \details Contains the data and method needed in order to hold and move around one or more LangtonAnt objects.
 * \details You can have one LangtonAnt, or multiple.
 * \details You can move them in parallel, or in sequence.
 * \details You can also shade the colors of each LangtonAnt.
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
    RasterCanvas* can;

    /*!
     * \brief Explicitly constructs an AntFarm object.
     * \details Explicit constructor for the AntFarm class.
     * \param w The width of the AntFarm object.
     * \param h The height of the AntFarm object.
     * \param s The size of the AntFarm object (how many ants are going to be in the AntFarm object).
     * \param can Pointer to the RasterCanvas to draw to.
     */
    AntFarm(int w, int h, int s, RasterCanvas* can);

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
     * \brief Move the ants.
     * \details Move the LangtonAnts that are currently in the AntFarm object.
     */
    void moveAnts();

    /*!
     * \brief Shade the ants.
     * \details Determines if we should shade the LangtonAnt(s) so that they have a darker color or not.
     * \param b A boolean determining if we should shade the LangtonAnt(s) or not.
     */
    void setShading(bool b);

    /*!
     * \brief Moves the ants in parallel.
     * \details Determines if we should move the LangtonAnt(s) in parallel or not.
     * \param b A boolean determining if we should move the LangtonAnt(s) in parallel or not.
     */
    void setParallel(bool b);
};

#endif /* ANTFARM_H_ */
