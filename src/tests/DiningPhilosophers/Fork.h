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
  ConcavePolygon * myShape;
  Fork() {
    user = -1; id = 0;

    const int POINTS = 20; // number of vertices in polygon
    const float HEIGHT = .42; // 42 is preferred, but can be changed
    const float WIDTH = .12;  // 12 is preferred, but can be changed
    float xs[POINTS], ys[POINTS];

    // scales (out of 100) for the dimensions of the fork
    float xscale[POINTS] = {-.50, -.31, -.31, -.23, -.23, -.04, -.04, .04, .04, .23, .23, .31, .31, .5, .5, .15,  .15,  -.15, -.15, -.50};
    float yscale[POINTS] = {-.50,  -.50, -.30, -.30, -.50,  -.50, -.30, -.30, -.50, -.50, -.30, -.30, -.50, -.50, -.20, -.20, .50, .50, -.20, -.20};

    // create the fork points from the scale arrays
    for(int i = 0; i < POINTS; ++i) {
      // scale the fork
      xs[i] = WIDTH  * xscale[i];
      ys[i] = HEIGHT * yscale[i];
      // xs[i] = xs[i]/100;
      // ys[i] = ys[i]/100;
    }

    //Add vertices
    myShape = new ConcavePolygon(0,0,0,POINTS, xs, ys, 0,0,0,ColorFloat(1,1,1,1));
  }
  void setCanvas( Canvas* can) {
    can->add(myShape);
  }
  void draw(float x, float y, double angle, ColorFloat c) {
    angle -= PI/2;
    myShape->setColor(c);
    myShape->setCenter(x, y, 0);
    myShape->setYaw(angle*180/PI);
  }
  ~Fork() {
    delete myShape;
  }
};

#endif /* FORK_H_ */