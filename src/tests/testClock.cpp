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
    Prism * head = new Prism(0,133,101,12,100,59,0,0,90,ColorFloat(.6,.3,0,1));
    can.add(head);

    Cuboid * left = new Cuboid(-45,-50,100,10,300,100,0,0,0,ColorFloat(.6,.3,0,1) );
    can.add(left);

    Cuboid * right = new Cuboid(45,-50,100,10,300,100,0,0,0,ColorFloat(.6,.3,0,1));
    can.add(right);

    Cuboid * back = new Cuboid(0,-50,55,80,300,10,0,0,0,ColorFloat(.6,.3,0,1) );
    can.add(back);

    Cuboid * bottom = new Cuboid(0,-195,110,79,10,80,0,0,0,ColorFloat(.6,.3,0,1) );
    can.add(bottom);

    Circle * face = new Circle(0,130,161,45,0,0,0,ColorFloat(1,1,0.8,1));
    can.add(face);

    Arrow * second = new Arrow(-19,130,163,38,2,0,0,0,ColorFloat(1,.8,.2,1),false);
    can.add(second);
    second->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Arrow * minute = new Arrow(-19,130,162,38,2,0,0,0,BLACK,false);
    can.add(minute);
    minute->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Arrow * hour = new Arrow(-10,130,162,20,2,0,0,0,BLACK,false);
    can.add(hour);
    hour->setRotationPoint(face->getCenterX(), face->getCenterY(), face->getCenterZ());

    Ellipsoid * pendulum = new Ellipsoid(0, -150, 100,10, 10,2,0,0,0,ColorFloat(0.75,0.6,.19, 1) );
    pendulum->setRotationPoint(0,80,10);
    can.add(pendulum);

    Rectangle * cord = new Rectangle(-110,80,100,220,5,90,0,0,BLACK);
    cord->setIsOutlined(false);
    cord->setRotationPoint(0,80,10);
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
    Canvas c(-1, -1, 500, 700, "Grandfather Clock", WHITE);
    c.run(clockFunction);
}