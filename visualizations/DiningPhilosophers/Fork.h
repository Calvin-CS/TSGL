/*!
 * \struct Fork
 * \brief Struct for the forks in the Dining Philosophers' problem
 */

#ifndef FORK_H_
#define FORK_H_

#include <tsgl.h>
using namespace tsgl;

struct Fork {

  int user, id;
  Canvas* can;
  ConcavePolygon * myShape;

  //Constructor
  Fork() {
    user = -1; id = 0;

    const int POINTS = 20; // number of vertices in polygon
    const int HEIGHT = 42; // 42 is preferred, but can be changed
    const int WIDTH = 12;  // 12 is preferred, but can be changed
    int xs[POINTS], ys[POINTS];

    // scales (out of 100) for the dimensions of the fork
    double xscale[POINTS] = {0, 19, 19, 27, 27, 46, 46, 54, 54, 73, 73, 81, 81, 100, 100, 65,  65,  35, 35,  0};
    double yscale[POINTS] = {0,  0, 20, 20,  0,  0, 20, 20,  0,  0, 20, 20,  0,   0,  30, 30, 100, 100, 30, 30};

    // create the fork points from the scale arrays
    for(int i = 0; i < POINTS; ++i) {
      // scale the fork
      xs[i] = WIDTH  * xscale[i];
      ys[i] = HEIGHT * yscale[i];
      xs[i] = xs[i]/100;
      ys[i] = ys[i]/100;
    }

    //Add vertices
    myShape = new ConcavePolygon(POINTS, BLACK);
    for(int i = 0; i < POINTS; i++) {
      myShape->addVertex(xs[i], ys[i]);
    }
    myShape->setLayer(5);
  }

  //Destructor
  ~Fork() {
    delete myShape;
  }

  /**
   * Add Fork to the Canvas
   */
  void setCanvas( Canvas* can ) {
    can->add(myShape);
  }

  /**
   * Update visible shape of Fork
   */
  void draw(int x, int y, double angle, ColorFloat c) {
    angle -= PI/2; // rotate by PI/2 radians or 90 degrees for fork next to philosopher
      // if adding PI/2, then the forks point out, if subtracting PI/2 the forks point in to table
        // without this line, the forks are perpendicular to philosophers

    myShape->setColor(c);
    myShape->setCenter(x, y);
    myShape->centeredRotation(angle*180/PI); //Convert angle to degrees

  }

};

#endif /* FORK_H_ */
