/*
 * testTransparency.cpp
 *
 * Usage: ./testTransparency
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void transparencyFunction(Canvas& can) {
    Rectangle * rec = new Rectangle(0,0,-100,600,50,0,0,0,WHITE );
    Cube * cube = new Cube(0,0,0,100,0,45,45,ColorFloat(0,0,1,0.2));
    Cube * cube2 = new Cube(0,0,150,50,0,45,45,ColorFloat(1,0,0,0.2));
    can.add(cube2);
    can.add(cube);
    can.add(rec);

    float rotation = 0.0f;
    while (can.isOpen()) {
        can.sleep();
        cube2->setCenterZ(150 * cos(rotation));
        rotation += 0.1;
    }
}

int main(int argc, char* argv[]) {
    Canvas c(-1, -1, 1024, 620, "Transparency", BLACK);
    c.run(transparencyFunction);
}