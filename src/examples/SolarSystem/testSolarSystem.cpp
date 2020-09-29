/*
 * testSolarSystem.cpp
 *
 * Usage: ./testSolarSystem
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void ssFunction(Canvas& can) {
    Sphere * sun = new Sphere(0, 0, 0, 75, 15, 0.0, 15.0, YELLOW);
    Sphere * mercury = new Sphere(95, 0, 0, 15, 15, 0.0, 15.0, ColorFloat(.8,.55,0,1));
    Sphere * venus = new Sphere(140, 0, 0, 25, 15, 0.0, 15.0, ColorFloat(1,.8,.5,1));
    Sphere * earth = new Sphere(205, 0, 0, 30, 15, 0.0, 15.0, ColorFloat(0,0.8,0.3,1));
    Sphere * mars =  new Sphere(270, 0, 0, 20, 15, 0.0, 15.0, ColorFloat(1,.4,0,1));
    Sphere * jupiter = new Sphere(345, 0, 0, 50, 15, 0.0, 15.0, ColorFloat(1,0.9,.6,1));
    Sphere * saturn = new Sphere(445, 0, 0, 40, 15, 0.0, 15.0, ColorFloat(.9,.65,.25,1));
    Circle * saturnRings = new Circle(445, 0, 0, 70, 15, 0, 75, ColorFloat(.9,.8,.3,0.5));
    Sphere * uranus = new Sphere(515, 0, 0, 25, 15, 0.0, 15.0, ColorFloat(.2,.6,1,1));
    Sphere * neptune = new Sphere(575, 0, 0, 20, 15, 0.0, 15.0, ColorFloat(.25,.65,1,1));

    mercury->setRotationPoint(0,0,0);
    venus->setRotationPoint(0,0,0);
    earth->setRotationPoint(0,0,0);
    mars->setRotationPoint(0,0,0);
    jupiter->setRotationPoint(0,0,0);
    saturn->setRotationPoint(0,0,0);
    saturnRings->setRotationPoint(0,0,0);
    uranus->setRotationPoint(0,0,0);
    neptune->setRotationPoint(0,0,0);

    can.add(sun);
    can.add(mercury);
    can.add(venus);
    can.add(earth);
    can.add(mars);
    can.add(jupiter);
    can.add(saturn);
    can.add(saturnRings);
    can.add(uranus);
    can.add(neptune);
    float rotation = 0.0f;
    while (can.isOpen()) {
        can.sleep();
        sun->setPitch(rotation);
        mercury->setPitch(rotation * 4);
        venus->setPitch(rotation * 3 / 2);
        earth->setPitch(rotation);
        mars->setPitch(rotation/2);
        jupiter->setPitch(rotation/12);
        saturn->setPitch(rotation/30);
        saturnRings->setPitch(rotation/30);
        uranus->setPitch(rotation/90);
        neptune->setPitch(rotation/180);
        rotation += 1;
    }

    delete sun;
    delete mercury;
    delete venus;
    delete earth;
    delete mars;
    delete jupiter;
    delete saturn;
    delete saturnRings;
    delete uranus;
    delete neptune;
}

int main(int argc, char* argv[]) {
    Canvas c(0, -1, Canvas::getDisplayWidth(), 620, "Solar System", BLACK);
    c.run(ssFunction);
}