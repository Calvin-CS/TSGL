/*!
 * \struct Fork3D
 * \brief Struct for the forks in the 3D visualization of the Dining Philosophers' problem
 */

#ifndef FORK3D_H_
#define FORK3D_H_

#include <tsgl.h>
using namespace tsgl;

struct Fork3D {
  int user, id;
  ConcavePolygon * myShape;
  Fork3D() {
    user = -1; id = 0;

    const int POINTS = 20; // number of vertices in polygon
    const float HEIGHT = .42; // 42 is preferred, but can be changed
    const float WIDTH = .12;  // 12 is preferred, but can be changed
    float xs[POINTS], ys[POINTS];

    // scales (out of 100) for the dimensions of the 3D fork
    float xscale[POINTS] = {-.50, -.31, -.31, -.23, -.23, -.04, -.04, .04, .04, .23, .23, .31, .31, .5, .5, .15,  .15,  -.15, -.15, -.50};
    float yscale[POINTS] = {-.50,  -.50, -.30, -.30, -.50,  -.50, -.30, -.30, -.50, -.50, -.30, -.30, -.50, -.50, -.20, -.20, .50, .50, -.20, -.20};

    // create the 3D fork points from the scale arrays
    for(int i = 0; i < POINTS; ++i) {
      // scale the 3D fork
      xs[i] = WIDTH  * xscale[i];
      ys[i] = HEIGHT * yscale[i];
      // xs[i] = xs[i]/100;
      // ys[i] = ys[i]/100;
    }

    //Add vertices
    myShape = new ConcavePolygon(0,0,0,POINTS, xs, ys, 0,0,0,WHITE);
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
  ~Fork3D() {
    delete myShape;
  }
};

#endif /* FORK3D_H_ */