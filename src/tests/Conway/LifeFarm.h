/*
 * LifeFarm.h
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <tsgl.h>
#include <omp.h>

using namespace tsgl;

/*!
 * \class LifeFarm
 * \brief Simulate Conway's Game of Life!
 * \details Contains the data and methods needed in order to simulate Conway's Game of Life.
 * \details see https://en.wikipedia.org/wiki/Conway's_Game_of_Life for more details on what Conway's Game of Life is.
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
     * \param w The width of the LifeFarm object.
     * \param h The height of the LifeFarm object.
     * \param can Pointer to the Canvas to draw to.
     * \param randomize Determines if we should randomize the motion of the ants that are currently in the LifeFarm object.
     */
    LifeFarm(int w, int h, Canvas* can, bool randomize);

    /*!
     * \brief Destroy a LifeFarm object.
     * \details Destructor for the LifeFarm class.
     * \return Frees up any allocated memory to a LifeFarm object.
     */
    ~LifeFarm();

    /*!
     * \brief Add an ant.
     * \details Adds an ant to the LifeFarm object.
     * \param x The x coordinate of the ant in the LifeFarm object.
     * \param y The y coordinate of the ant in the LifeFarm object.
     */
    void addAnt(int x, int y);

    /*!
     * \brief Move the ants.
     * \details Move the ants that are currently in the LifeFarm object.
     * \note This method class moveAntsOld().
     * \see moveAntsOld()
     */
    void moveAnts();

    /*!
     * \brief Move the ants.
     * \details This method is called first in order to move the ants around in the LifeFarm object.
     * \see moveAntsNew()
     */
    void moveAntsOld();

    /*!
     * \brief Move the ants.
     * \details This method moves the ants around to new positions.
     * \see moveAntsOld()
     */
    void moveAntsNew();

    /*!
     * \brief Draw "dead" ants.
     * \details Determines if we should draw ants that have "died" in the game.
     * \param b A boolean that determines if we should draw the dead ants or not.
     */
    void setDrawdead(bool b);

    /*!
     * \brief Give life to the ants!
     * \details Makes the ants come alive by keeping track of their current positions and next state positions.
     * \param *current A pointer to an array of current states that the ants are in during the game.
     * \param *fresh A pointer to an array of next states that the ants are going to be in during the game.
     */
    void life(int *current, int *fresh);
};

#endif /* ANTFARM_H_ */
