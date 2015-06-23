/*
 * AntFarm.h
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <tsgl.h>
#include <omp.h>

using namespace tsgl;

/*!
 *
 */
class LifeFarm {
private:
    void initGun();

    unsigned framecount;
    bool **alive;
    int **neighbors;
    int *livelist;
    int liststart, listend;
    bool drawdead;
    ColorFloat bgcolor;

    int *currentstate, *nextstate;
    int *currentstart, *nextstart;
public:
    int width, height, size;
    Canvas* can;
    /*!
     * \brief Explicitly constructs a LifeFarm object.
     * \details Explicit constructor for the LifeFarm class.
     * \param w
     * \param h
     * \param can
     * \param randomize
     */
    LifeFarm(int w, int h, Canvas* can, bool randomize);

    /*!
     * \brief Destroy a LifeFarm object.
     * \details Destructor for the LifeFarm class.
     * \return Frees up any allocated memory to a LifeFarm object.
     */
    ~LifeFarm();

    /*!
     *
     */
    void addAnt(int x, int y);

    /*!
     *
     */
    void moveAnts();

    /*!
     *
     */
    void moveAntsOld();

    /*!
     *
     */
    void moveAntsNew();

    /*!
     *
     */
    void setDrawdead(bool b);

    /*!
     *
     */
    void life(int *current, int *fresh);
};

#endif /* ANTFARM_H_ */
