/*
 * testDiorama.cpp
 *
 * Usage: ./testDiorama
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void dioramaFunction(Canvas& can) {
    Square * blankCanvas = new Square(180,0,135,270,0,0,0,WHITE);
    blankCanvas->setEdgeColor(ColorFloat(0.1,0.1,0.1,1));
    // can.add(blankCanvas);

    Rectangle * emptyDioramaLeft = new Rectangle(-45,0,0,270,270,0,90,0,ColorFloat(1,1,1,0));
    emptyDioramaLeft->setEdgeColor(ColorFloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaLeft);
    Rectangle * emptyDioramaRight = new Rectangle(-315,0,0,270,270,0,90,0,ColorFloat(1,1,1,0));
    emptyDioramaRight->setEdgeColor(ColorFloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaRight);
    Rectangle * emptyDioramaTop = new Rectangle(-180,135,0,270,270,0,0,90,ColorFloat(1,1,1,0));
    emptyDioramaTop->setEdgeColor(ColorFloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaTop);
    Rectangle * emptyDioramaBottom = new Rectangle(-180,-135,0,270,270,0,0,90,ColorFloat(1,1,1,0));
    emptyDioramaBottom->setEdgeColor(ColorFloat(0.1,0.1,0.1,1));
    // can.add(emptyDioramaBottom);

    Cuboid * trunk = new Cuboid(-200,-30,0,25,200,25,0,0,0,ColorFloat(.6,.3,0,1));
    trunk->displayOutlineEdges(false);
    // can.add(trunk);

    Ellipsoid * leaves = new Ellipsoid(-200,70,0,75,50,40,0,0,0,GREEN);
    // can.add(leaves);

    Rectangle * trunkFlat = new Rectangle(160,-30,135,25,200,0,0,0,ColorFloat(.6,.3,0,1));
    trunkFlat->displayOutlineEdges(false);
    // can.add(trunkFlat);

    Ellipse * leavesFlat = new Ellipse(160,70,135,75,50,0,0,0,ColorFloat(0,0.8,0,1));
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
            if(emptyDioramaLeft->getWidth() > 15) {
                emptyDioramaLeft->changeWidthBy(-270/20);
                emptyDioramaRight->changeWidthBy(-270/20);
                emptyDioramaTop->changeHeightBy(-270/20);
                emptyDioramaBottom->changeHeightBy(-270/20);
                emptyDioramaTop->changeZBy(135/20);
                emptyDioramaBottom->changeZBy(135/20);
                emptyDioramaLeft->changeZBy(135/20);
                emptyDioramaRight->changeZBy(135/20);
                trunk->changeLengthBy(-25/20);
                trunk->changeZBy(135/20);
                leaves->changeZRadiusBy(-40/20);
                leaves->changeZBy(135/20);
            } else {
                emptyDioramaLeft->setWidth(1);
                emptyDioramaRight->setWidth(1);
                emptyDioramaTop->setHeight(1);
                emptyDioramaBottom->setHeight(1);
                emptyDioramaTop->setCenterZ(135);
                emptyDioramaBottom->setCenterZ(135);
                emptyDioramaLeft->setCenterZ(135);
                emptyDioramaRight->setCenterZ(135);
                trunk->setLength(1);
                trunk->setCenterZ(135);
                leaves->setZRadius(1);
                leaves->setCenterZ(135);
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