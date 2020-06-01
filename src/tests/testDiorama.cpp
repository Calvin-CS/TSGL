/*
 * testDiorama.cpp
 *
 * Usage: ./testDiorama
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void dioramaFunction(Canvas& can) {
    Square * blankCanvas = new Square(1.8,0,1.35,2.7,0,0,0,ColorGLfloat(1,1,1,1));
    blankCanvas->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    // can.add(blankCanvas);

    Rectangle * emptyDioramaLeft = new Rectangle(-0.45,0,0,2.7,2.7,0,90,0,ColorGLfloat(1,1,1,0));
    emptyDioramaLeft->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaLeft);
    Rectangle * emptyDioramaRight = new Rectangle(-3.15,0,0,2.7,2.7,0,90,0,ColorGLfloat(1,1,1,0));
    emptyDioramaRight->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaRight);
    Rectangle * emptyDioramaTop = new Rectangle(-1.8,1.35,0,2.7,2.7,0,0,90,ColorGLfloat(1,1,1,0));
    emptyDioramaTop->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaTop);
    Rectangle * emptyDioramaBottom = new Rectangle(-1.8,-1.35,0,2.7,2.7,0,0,90,ColorGLfloat(1,1,1,0));
    emptyDioramaBottom->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaBottom);

    Cuboid * trunk = new Cuboid(-2,-.3,0,0.25,2,0.25,0,0,0,ColorGLfloat(.6,.3,0,1));
    trunk->displayOutlineEdges(false);
    // can.add(trunk);

    Ellipsoid * leaves = new Ellipsoid(-2,0.7,0,0.75,0.5,0.4,0,0,0,ColorGLfloat(0,1,0,1));
    // can.add(leaves);

    Rectangle * trunkFlat = new Rectangle(1.6,-.3,1.35,0.25,2,0,0,0,ColorGLfloat(.6,.3,0,1));
    trunkFlat->displayOutlineEdges(false);
    // can.add(trunkFlat);

    Ellipse * leavesFlat = new Ellipse(1.6,0.7,1.35,0.75,0.5,0,0,0,ColorGLfloat(0,0.8,0,1));
    // can.add(leavesFlat);

    float counter = 0;
    while (can.isOpen()) {
        // can.sleepFor(0.5);
        can.sleep();
        if (can.getFrameNumber() > 700 && counter < 1) {
            can.add(blankCanvas);
            counter++;
        }
        if(can.getFrameNumber() > 1700 && counter < 2) {
            can.add(trunkFlat);
            counter++;
        }
        if(can.getFrameNumber() > 1800 && counter < 3) {
            can.add(leavesFlat);
            counter++;
        }
        if(can.getFrameNumber() > 2300 && counter < 4) {
            can.add(emptyDioramaLeft);
            can.add(emptyDioramaRight);
            can.add(emptyDioramaTop);
            can.add(emptyDioramaBottom);
            counter++;
        }
        if(can.getFrameNumber() > 3500 && counter < 5) {
            can.add(trunk);
            counter++;
        }
        if(can.getFrameNumber() > 3600 && counter < 6) {
            can.add(leaves);
            counter++;
        }
        if(can.getFrameNumber() > 4000 && counter < 7) {
            if(emptyDioramaLeft->getWidth() > .15) {
                emptyDioramaLeft->changeWidthBy(-2.7/20);
                emptyDioramaRight->changeWidthBy(-2.7/20);
                emptyDioramaTop->changeHeightBy(-2.7/20);
                emptyDioramaBottom->changeHeightBy(-2.7/20);
                emptyDioramaTop->changeZBy(1.35/20);
                emptyDioramaBottom->changeZBy(1.35/20);
                emptyDioramaLeft->changeZBy(1.35/20);
                emptyDioramaRight->changeZBy(1.35/20);
                trunk->changeLengthBy(-0.25/20);
                trunk->changeZBy(1.35/20);
                leaves->changeZRadiusBy(-0.4/20);
                leaves->changeZBy(1.35/20);
            } else {
                emptyDioramaLeft->setWidth(0.01);
                emptyDioramaRight->setWidth(0.01);
                emptyDioramaTop->setHeight(0.01);
                emptyDioramaBottom->setHeight(0.01);
                emptyDioramaTop->setCenterZ(1.35);
                emptyDioramaBottom->setCenterZ(1.35);
                emptyDioramaLeft->setCenterZ(1.35);
                emptyDioramaRight->setCenterZ(1.35);
                trunk->setLength(0.01);
                trunk->setCenterZ(1.35);
                leaves->setZRadius(0.01);
                leaves->setCenterZ(1.35);
                counter++;
            }
        }
    }

    delete blankCanvas;
    delete emptyDioramaLeft;
    delete emptyDioramaRight;
    delete emptyDioramaTop;
    delete emptyDioramaBottom;
    delete leaves;
    delete trunkFlat;
    delete leavesFlat;

}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Diorama vs. Painting");
    c.setBackgroundColor(WHITE);
    c.run(dioramaFunction);
}