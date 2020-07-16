/*
 * testDice.cpp
 *
 * Usage: ./testDice
 * Note: currently has some interesting rotation issues that mean that the
 * API will probably have to be updated. Not compiling since it won't
 * really work, alter the Makefile to understand the issue. Has to do with composite
 * Euler angles.
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void diceFunction(Canvas& can) {
    Cube * die = new Cube(0.0, 0.0, 0.0, 2, 0.0, 0.0, 0.0, WHITE);
    can.add(die);

    RegularPolygon * spot1 =  new RegularPolygon(0,0,1.01,0.5,6,0,0,0,BLACK);
    can.add(spot1);
    spot1->setRotationPoint(0,0,0);


    RegularPolygon * spot2_1 = new RegularPolygon(-0.6,0.6,1.01,0.3,6,0,0,-90,BLACK);
    spot2_1->setRotationPoint(0,0,0);
    can.add(spot2_1);
    RegularPolygon * spot2_2 = new RegularPolygon(0.6,-0.6,1.01,0.3,6,0,0,-90,BLACK);
    spot2_2->setRotationPoint(0,0,0);
    can.add(spot2_2);

    RegularPolygon * spot3_1 = new RegularPolygon(0,1.01,0,0.3,6,90,0,0,BLACK);
    spot3_1->setRotationPoint(0,0,0);
    can.add(spot3_1);
    RegularPolygon * spot3_2 = new RegularPolygon(0.6,1.01,0.6,0.3,6,90,0,0,BLACK);
    spot3_2->setRotationPoint(0,0,0);
    can.add(spot3_2);
    RegularPolygon * spot3_3 = new RegularPolygon(-0.6,1.01,-0.6,0.3,6,90,0,0,BLACK);
    spot3_3->setRotationPoint(0,0,0);
    can.add(spot3_3);

    // RegularPolygon * spot4_1 = new RegularPolygon(0.6,0.6,1.01,0.3,6,0,90,0,BLACK);
    // spot4_1->setRotationPoint(0,0,0);
    // can.add(spot4_1);
    // RegularPolygon * spot4_2 = new RegularPolygon(-0.6,0.6,1.01,0.3,6,0,90,0,BLACK);
    // spot4_2->setRotationPoint(0,0,0);
    // can.add(spot4_2);
    // RegularPolygon * spot4_3 = new RegularPolygon(0.6,-0.6,1.01,0.3,6,0,90,0,BLACK);
    // spot4_3->setRotationPoint(0,0,0);
    // can.add(spot4_3);
    // RegularPolygon * spot4_4 = new RegularPolygon(-0.6,-0.6,1.01,0.3,6,0,90,0,BLACK);
    // spot4_4->setRotationPoint(0,0,0);
    // can.add(spot4_4);

    RegularPolygon * spot5_1 = new RegularPolygon(0,0,1.01,0.3,6,0,0,90,BLACK);
    spot5_1->setRotationPoint(0,0,0);
    can.add(spot5_1);
    RegularPolygon * spot5_2 = new RegularPolygon(0.6,0.6,1.01,0.3,6,0,0,90,BLACK);
    spot5_2->setRotationPoint(0,0,0);
    can.add(spot5_2);
    RegularPolygon * spot5_3 = new RegularPolygon(-0.6,0.6,1.01,0.3,6,0,0,90,BLACK);
    spot5_3->setRotationPoint(0,0,0);
    can.add(spot5_3);
    RegularPolygon * spot5_4 = new RegularPolygon(0.6,-0.6,1.01,0.3,6,0,0,90,BLACK);
    spot5_4->setRotationPoint(0,0,0);
    can.add(spot5_4);
    RegularPolygon * spot5_5 = new RegularPolygon(-0.6,-0.6,1.01,0.3,6,0,0,90,BLACK);
    spot5_5->setRotationPoint(0,0,0);
    can.add(spot5_5);

    RegularPolygon * spot6_1 =  new RegularPolygon(-0.5,0.65,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_1);
    RegularPolygon * spot6_2 =  new RegularPolygon(-0.5,0,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_2);
    RegularPolygon * spot6_3 =  new RegularPolygon(-0.5,-0.65,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_3);
    RegularPolygon * spot6_4 =  new RegularPolygon(0.5,0.65,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_4);
    RegularPolygon * spot6_5 =  new RegularPolygon(0.5,0,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_5);
    RegularPolygon * spot6_6 =  new RegularPolygon(0.5,-0.65,1.01,0.3,6,0,0,180,BLACK);
    can.add(spot6_6);
    spot6_1->setRotationPoint(0,0,0);
    spot6_2->setRotationPoint(0,0,0);
    spot6_3->setRotationPoint(0,0,0);
    spot6_4->setRotationPoint(0,0,0);
    spot6_5->setRotationPoint(0,0,0);
    spot6_6->setRotationPoint(0,0,0);

    // die->setRoll(45);
    // spot1->setRoll(45);
    // spot2_1->setRoll(45-90);
    // spot2_2->setRoll(45-90);
    // spot3_1->setRoll(180);
    // spot3_2->setRoll(180);
    // spot3_3->setRoll(180);
    // spot3_3->setRoll(180);
    // spot3_3->setRoll(180);
    // spot3_3->setRoll(180);
    // spot3_3->setRoll(180);
    // spot3_3->setRoll(180);
    // spot4_1->setRoll(45);
    // spot4_2->setRoll(45);
    // spot4_3->setRoll(45);
    // spot4_4->setRoll(45);
    // spot5_1->setRoll(45+90);
    // spot5_2->setRoll(45+90);
    // spot5_3->setRoll(45+90);
    // spot5_4->setRoll(45+90);
    // spot5_5->setRoll(45+90);
    // spot6_1->setRoll(45+180);
    // spot6_2->setRoll(45+180);
    // spot6_3->setRoll(45+180);
    // spot6_4->setRoll(45+180);
    // spot6_5->setRoll(45+180);
    // spot6_6->setRoll(45+180);

    float rotation = 0;
    while (can.isOpen()) {
        can.sleep();
        // die->setRoll(rotation);
        // spot1->setRoll(rotation);
        // spot2_1->setRoll(rotation-90);
        // spot2_2->setRoll(rotation-90);
        // spot3_1->setPitch(rotation);
        // spot3_2->setPitch(rotation);
        // spot3_3->setPitch(rotation);
        // spot4_1->setRoll(rotation);
        // spot4_2->setRoll(rotation);
        // spot4_3->setRoll(rotation);
        // spot4_4->setRoll(rotation);
        // spot5_1->setRoll(rotation+90);
        // spot5_2->setRoll(rotation+90);
        // spot5_3->setRoll(rotation+90);
        // spot5_4->setRoll(rotation+90);
        // spot5_5->setRoll(rotation+90);
        // spot6_1->setRoll(rotation+180);
        // spot6_2->setRoll(rotation+180);
        // spot6_3->setRoll(rotation+180);
        // spot6_4->setRoll(rotation+180);
        // spot6_5->setRoll(rotation+180);
        // spot6_6->setRoll(rotation+180);

        die->setPitch(rotation);
        spot1->setPitch(rotation);
        spot2_1->setPitch(rotation);
        spot2_2->setPitch(rotation);
        spot3_1->setPitch(rotation-90);
        spot3_2->setPitch(rotation-90);
        spot3_3->setPitch(rotation-90);
        // spot4_1->setPitch(rotation+90);
        // spot4_2->setPitch(rotation+90);
        // spot4_3->setPitch(rotation+90);
        // spot4_4->setPitch(rotation+90);
        spot5_1->setPitch(rotation);
        spot5_2->setPitch(rotation);
        spot5_3->setPitch(rotation);
        spot5_4->setPitch(rotation);
        spot5_5->setPitch(rotation);
        spot6_1->setPitch(rotation);
        spot6_2->setPitch(rotation);
        spot6_3->setPitch(rotation);
        spot6_4->setPitch(rotation);
        spot6_5->setPitch(rotation);
        spot6_6->setPitch(rotation);
        rotation += 1;
    }

    delete die;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Dice Rolling");
    c.run(diceFunction);
}