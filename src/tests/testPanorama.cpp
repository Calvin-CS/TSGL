/*
 * testPanorama.cpp
 *
 * Usage: ./testPanorama
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void panoramaFunction(Canvas& can) {
    Square * blankCanvas = new Square(1.8,0,1.35,2.7,0,0,0,ColorGLfloat(1,1,1,1));
    blankCanvas->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    can.add(blankCanvas);

    Rectangle * emptyPanoramaLeft = new Rectangle(-0.45,0,0,2.7,2.7,0,90,0,ColorGLfloat(1,1,1,0));
    emptyPanoramaLeft->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    can.add(emptyPanoramaLeft);
    Rectangle * emptyPanoramaRight = new Rectangle(-3.15,0,0,2.7,2.7,0,90,0,ColorGLfloat(1,1,1,0));
    emptyPanoramaRight->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    can.add(emptyPanoramaRight);
    Rectangle * emptyPanoramaTop = new Rectangle(-1.8,1.35,0,2.7,2.7,0,0,90,ColorGLfloat(1,1,1,0));
    emptyPanoramaTop->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    can.add(emptyPanoramaTop);
    Rectangle * emptyPanoramaBottom = new Rectangle(-1.8,-1.35,0,2.7,2.7,0,0,90,ColorGLfloat(1,1,1,0));
    emptyPanoramaBottom->setEdgeColor(ColorGLfloat(0.1,0.1,0.1,1));
    can.add(emptyPanoramaBottom);

    Cuboid * trunk = new Cuboid(-2,-.3,0,0.25,2,0.25,0,0,0,ColorGLfloat(.6,.3,0,1));
    trunk->displayOutlineEdges(false);
    can.add(trunk);

    Ellipsoid * leaves = new Ellipsoid(-2,0.7,0,0.75,0.5,0.4,0,0,0,ColorGLfloat(0,1,0,1));
    can.add(leaves);

    Rectangle * trunkFlat = new Rectangle(1.6,-.3,1.35,0.25,2,0,0,0,ColorGLfloat(.6,.3,0,1));
    trunkFlat->displayOutlineEdges(false);
    can.add(trunkFlat);

    Ellipse * leavesFlat = new Ellipse(1.6,0.7,1.35,0.75,0.5,0,0,0,ColorGLfloat(0,0.8,0,1));
    can.add(leavesFlat);

    float counter = 1;
    while (can.isOpen()) {
        // can.sleepFor(0.5);
        can.sleep();
        if (counter < 10) {
            emptyPanoramaLeft->setWidth(2.7/(counter*4));
            emptyPanoramaRight->setWidth(2.7/(counter*4));
            emptyPanoramaTop->setHeight(2.7/(counter*4));
            emptyPanoramaBottom->setHeight(2.7/(counter*4));
            emptyPanoramaTop->setCenterZ(1.35-1.35/counter);
            emptyPanoramaBottom->setCenterZ(1.35-1.35/counter);
            emptyPanoramaLeft->setCenterZ(1.35-1.35/counter);
            emptyPanoramaRight->setCenterZ(1.35-1.35/counter);
            trunk->setLength(0.25/counter);
            trunk->setCenterZ(1.35-1.35/counter);
            leaves->setZRadius(0.4/counter);
            leaves->setCenterZ(1.35-1.35/counter);
            counter += 0.05;
        } else {
            emptyPanoramaLeft->setWidth(0.01);
            emptyPanoramaRight->setWidth(0.01);
            emptyPanoramaTop->setHeight(0.01);
            emptyPanoramaBottom->setHeight(0.01);
            emptyPanoramaTop->setCenterZ(1.35);
            emptyPanoramaBottom->setCenterZ(1.35);
            emptyPanoramaLeft->setCenterZ(1.35);
            emptyPanoramaRight->setCenterZ(1.35);
            trunk->setLength(0.25/counter);
            trunk->setCenterZ(1.35-1.35/counter);
            leaves->setZRadius(0.4/counter);
            leaves->setCenterZ(1.35-1.35/counter);
        }
        // printf("%f\n", counter);
    }

    delete blankCanvas;
    delete emptyPanoramaLeft;
    delete emptyPanoramaRight;
    delete emptyPanoramaTop;
    delete emptyPanoramaBottom;

}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Panorama vs. Painting");
    c.setBackgroundColor(WHITE);
    c.run(panoramaFunction);
}