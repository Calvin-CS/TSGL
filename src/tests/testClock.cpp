/*
 * testClock.cpp
 *
 * Usage: ./testClock
 */

#include <tsgl.h>
#include <cmath>
#include <ctime>

using namespace tsgl;

void clockFunction(Canvas& can) {
    Prism * head = new Prism(0,1.33,1.01,12,1,0.59,0,0,90,ColorGLfloat(.6,.3,0,1));
    can.add(head);

    Cuboid * left = new Cuboid(-0.45,-.5,1,0.1,3,1,0,0,0,ColorGLfloat(.6,.3,0,1) );
    can.add(left);

    Cuboid * right = new Cuboid(0.45,-0.5,1,0.1,3,1,0,0,0,ColorGLfloat(.6,.3,0,1));
    can.add(right);

    Cuboid * back = new Cuboid(0,-0.5,0.55,0.8,3,0.1,0,0,0,ColorGLfloat(.6,.3,0,1) );
    can.add(back);

    Cuboid * bottom = new Cuboid(0,-1.95,1.10,0.79,0.1,0.8,0,0,0,ColorGLfloat(.6,.3,0,1) );
    can.add(bottom);

    Circle * face = new Circle(0,1.15,2.01,0.4,0,0,0,ColorGLfloat(1,1,0.8,1));
    can.add(face);

    Arrow * second = new Arrow(-0.19,1.15,2.03,0.38,0.02,0,0,0,ColorGLfloat(1,.8,.2,1),false);
    can.add(second);
    second->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Arrow * minute = new Arrow(-0.19,1.15,2.02,0.38,0.02,0,0,0,ColorGLfloat(0,0,0,1),false);
    can.add(minute);
    minute->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Arrow * hour = new Arrow(-0.1,1.15,2.02,0.2,0.02,0,0,0,ColorGLfloat(0,0,0,1),false);
    can.add(hour);
    hour->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Ellipsoid * pendulum = new Ellipsoid(0, -1.5, 1,0.1, 0.1,0.02,0,0,0,ColorGLfloat(0.75,0.6,.19, 1) );
    pendulum->setRotationPoint(0,0.8,1);
    can.add(pendulum);

    Rectangle * cord = new Rectangle(-1.1,0.8,1,2.2,0.05,90,0,0,ColorGLfloat(0,0,0,1));
    cord->setRotationPoint(0,0.8,1);
    cord->displayOutlineEdges(false);
    can.add(cord);

    time_t t = time(NULL);
    // printf("local:     %s", asctime(localtime(&t)));
    // printf("local: %d:%d:%d\n", localtime(&t)->tm_hour, localtime(&t)->tm_min, localtime(&t)->tm_sec);

    second->setYaw(localtime(&t)->tm_sec * -6 - 90);
    minute->setYaw((localtime(&t)->tm_min * -6) + (localtime(&t)->tm_sec / -10) - 90);
    hour->setYaw((localtime(&t)->tm_hour * -30) + (localtime(&t)->tm_min / -2) + (localtime(&t)->tm_sec / -120) - 90);    
    
    float speed = -0.28;
    bool accelerationPositive = true;
    while (can.isOpen()) {
        can.sleep();
        t = time(NULL);
        second->setYaw(localtime(&t)->tm_sec * -6 - 90);
        minute->setYaw((localtime(&t)->tm_min * -6) + (localtime(&t)->tm_sec / -10) - 90);
        hour->setYaw((localtime(&t)->tm_hour * -30) + (localtime(&t)->tm_min / -2) + (localtime(&t)->tm_sec / -120) - 90); 
        if (cord->getYaw() > 97 || cord->getYaw() < 83) {
            speed = 0;
        }
        if (cord->getYaw() > 90 && accelerationPositive == true) {
            accelerationPositive = false;
        } else if (cord->getYaw() < 90 && accelerationPositive == false) {
            accelerationPositive = true;
        }
        if (accelerationPositive) {
            speed += 0.01;
        } else {
            speed -= 0.01;
        }
        cord->changeYawBy(speed);
        pendulum->changeYawBy(speed);

    }

    delete head;
    delete left;
    delete right;
    delete back;
    delete bottom;
    delete face;
    delete second;
    delete minute;
    delete hour;
    delete pendulum;
    delete cord;
}

int main(int argc, char* argv[]) {
    // int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    // int h = (argc > 2) ? atoi(argv[2]) : w;
    // if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    //   w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 500, 700, "Grandfather Clock");
    c.setBackgroundColor(WHITE);
    c.run(clockFunction);
}