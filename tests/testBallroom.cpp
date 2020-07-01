/*
 * testBallroom.cpp
 *
 * Usage: ./testBallroom <width> <height>
 */

#include <list>
#include <cmath>
#include <tsgl.h>

using namespace tsgl;

//Vector struct for the movement of BouncingBalls
struct Vector {
  float x,y;
  Vector() {
    x = y = 0;
  }
  Vector(float xx,float yy) {
    x = xx; y = yy;
  }
  Vector operator+(const Vector &o) {
    return Vector(x+o.x,y+o.y);
  }
  Vector& operator+=(const Vector &o) {
    x += o.x; y += o.y;
    return (*this);
  }
  Vector operator-(const Vector &o) {
    return Vector(x-o.x,y-o.y);
  }
  Vector operator-() {
    return Vector(-x,-y);
  }
  Vector& operator-=(const Vector &o) {
    x -= o.x; y -= o.y;
    return (*this);
  }
  Vector operator*(const float f) {
    return Vector(x*f,y*f);
  }
  Vector& operator*=(const float f) {
    x *= f; y *= f;
    return (*this);
  }
  float dot(const Vector &o) const {
    return x*o.x + y*o.y;
  }
  float length() const {
    return sqrt(x*x+y*y);
  }
  float angle() const {
    return atan2(y,x);
  }
  float angle(const Vector &o) {
    return acos(angle(o));
  }
  float cosangle(const Vector &o) {
    return dot(o)/( length() * o.length() );
  }
};
Vector operator+(const Vector &v1, const Vector &v2) {
  return Vector(v1.x+v2.x,v1.y+v2.y);
}
Vector operator-(const Vector &v1, const Vector &v2) {
  return Vector(v1.x-v2.x,v1.y-v2.y);
}
Vector operator*(const Vector &v, const float f) {
  return Vector(v.x*f,v.y*f);
}

//BouncingBall class for adding to the BallRoom
class BouncingBall {
private:
  float mySpeed, myDir;
  int rw, rh;
  Circle * circle;
  Canvas * can;
public:
  Vector pos, vel, acc;
  ColorFloat color;
  int rad;
  bool bounced;
  BouncingBall(int x, int y, int r, int w, int h, ColorFloat c, Canvas * can) {
    pos = Vector(x,y);
    vel = Vector(0,0);
    acc = Vector(0,0);
    mySpeed = myDir = 0;
    rad = r;
    rw = w;
    rh = h;
    color = c;
    bounced = false;
    circle = new Circle(x,y,r,c);
    can->add(circle);
  }
  BouncingBall(int x, int y, float vx, float vy, int r, int w, int h, ColorFloat c, Canvas * canvas) {
    pos = Vector(x,y);
    vel = Vector(vx,vy);
    acc = Vector(0,0.1f);
    calcSpeed();
    calcDir();
    rad = r;
    rw = w;
    rh = h;
    color = c;
    bounced = false;
    can = canvas;
    circle = new Circle(x,y,r,c);
    can->add(circle);
  }
  ~BouncingBall() {
    can->remove(circle);
    delete circle;
  }
  void calcSpeed() {
    mySpeed = vel.length();
  }
  void setSpeed(float s) {
    mySpeed = s;
    vel.x = s*cos(myDir);
    vel.y = s*sin(myDir);
  }
  void calcDir() {
    myDir = vel.angle();
  }
  void setDir(float d) {
    myDir = d;
    vel.x = mySpeed*cos(d);
    vel.y = mySpeed*sin(d);
  }
  void step() {
    bounced = false;
    vel += acc;
    pos += vel;
    if (pos.x <= rad) {
      pos.x = rad;
      vel.x *= -0.5f;
    } else if (pos.x >= rw-rad) {
      pos.x = rw-rad;
      vel.x *= -0.5f;
    }
    if (pos.y <= rad) {
      pos.y = rad;
      vel.y *= -1.0f;
    } else if (pos.y >= rh-rad) {
      pos.y = rh-rad;
      vel.y *= -1.0f;
    }
    calcSpeed();
    calcDir();
    circle->setCenter(pos.x, pos.y);
  }
  void setRoomSize(int w, int h) {
    rw = w;
    rh = h;
  }
  void bounce(BouncingBall *o) {
    if (bounced)
      return;
    Vector dist = pos-o->pos;
    float radii = rad+o->rad;
    if (dist.length() > radii)
      return;
    Vector vdelta = vel-o->vel;
    pos -= vel;
    Vector pdelta = pos-o->pos;
    float ps = pdelta.length()*pdelta.length();
    vel -= pdelta*(vdelta.dot(pdelta)/ps);
    o->vel += pdelta*((-vdelta).dot(-pdelta)/ps);
    calcSpeed();
    calcDir();
    o->calcSpeed();
    o->calcDir();
    bounced = true;
    circle->setCenter(pos.x, pos.y);
  }
  bool collides(BouncingBall *o) {
    return ((pos-o->pos).length() <= (rad+o->rad));
  }
};

//BallRoom class manages all balls on the screen and their motions
class BallRoom {
private:
  int width, height;
  float friction, gravity;
  bool attract;
  std::list<BouncingBall*> balls;
  std::list<BouncingBall*>::const_iterator it, jt;
  Circle * mouseCircle;
  Canvas * can;
public:
  BallRoom(int w, int h, Canvas * canvas) {
    width = w;
    height = h;
    friction = 0.99f;
    gravity = 0.1f;
    attract = true;
    can = canvas;
    mouseCircle = new Circle(0,0,25,ColorFloat(1.0f,0.5f,0.5f,0.5f));
    can->add(mouseCircle);
  }
  ~BallRoom() {
    while (!balls.empty()) {
      BouncingBall* b = balls.front();
      balls.pop_front();
      delete b;
    }
    can->remove(mouseCircle);
    delete mouseCircle;
  }
  void addBall(int x, int y, int r, ColorFloat c = WHITE) {
    addBall(x,y,0,0,r,c);
  }
  void addBall(int x, int y, int vx, int vy, int r, ColorFloat c = WHITE) {
    BouncingBall* b = new BouncingBall(x,y,vx,vy,r,width,height,c,can);
    const int MAXFAIL = 1000;
    int fails = 0;
    for (it = balls.begin(); it != balls.end(); ++it) {
      while (b->collides((*it))) {
        ++fails;
        if (fails == MAXFAIL) {
          delete b;
          return;
        }
        b->pos.x = 25 + safe_rand() % (width-50);
        b->pos.y = 25 + safe_rand() % (height-50);
      }
    }
    balls.push_back(b);
  }
  void step(Canvas* c) {
    int mx = c->getMouseX(), my = c->getMouseY();
    Vector mvec(mx,my);
    mouseCircle->setCenter(mx, my);
    if (attract) {
      mouseCircle->setColor( ColorFloat(1.0f,0.5f,0.5f,0.5f) );
    } else {
      mouseCircle->setColor( ColorFloat(0.5f,1.0f,1.0f,0.5f) );
    }
    for (it = balls.begin(); it != balls.end(); ++it) {
      BouncingBall *b = (*it);

      float mdir;
      if (attract)
        mdir = (mvec - b->pos).angle();
      else
        mdir = (b->pos-mvec).angle();
      b->acc = Vector(cos(mdir),sin(mdir))*gravity;
      b->vel *= friction;

      b->step();
      for (jt = balls.begin(); jt != balls.end(); ++jt) {
        if (jt == it)
          continue;
        b->bounce(*jt);
        if (b->bounced)
          break;
      }
    }
  }
  inline void toggleAttract() {
    attract ^= true;
  }
};

/*!
 * \brief Lots and lots of multicolored balls!
 * \details Draws a lot of multicolored balls onto the Canvas that follow the mouse around.
 * - It is drawn in this way:
 * - Get the window width and height for convenience of use.
 * - Create the area for the balls based off of the window width and height.
 * - For each ball:
 *   - Set its speed to 5.
 *   - Randomize its initial direction.
 *   - Add it to the area created with the calculated speed and direction as well as with a random color.
 *   .
 * - Bind the left mouse button so that when you click the screen the attraction of the balls will change.
 *   (If they are attracted to the mouse then they will be repelled. If they are repelled, then they will be attracted
 *   to the mouse).
 * - While the Canvas is open:
 *   - Sleep the internal timer until the next draw cycle.
 *   - Animate the balls and have them bounce off of one another.
 *   .
 * .
 * \param can Reference to the Canvas to draw on.
 */
void ballroomFunction(Canvas& can) {
    const int WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight();   // Window height
    BallRoom b(WW,WH,&can);
    srand(time(NULL)); // seed the random number generator
    for (int i = 0; i < 100; ++ i) {
      float speed = 5.0f;
      float dir = 2 * 3.14159f * (safe_rand() % 100) / 100.0f;
      b.addBall(25 + safe_rand() % (WW-50),25 + safe_rand() % (WH-50),speed*cos(dir),speed*sin(dir),10,
        ColorInt(64 + safe_rand() % 192,64 + safe_rand() % 192,64 + safe_rand() % 192,255));
    }

    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&b]() {
        b.toggleAttract();
    });

    while (can.isOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        b.step(&can);
    }
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "The Ballroom");
    c.run(ballroomFunction);
}
