/*
 * testBackground.cpp tests the core functions of TSGL::Background
 *
 * Usage: ./testBackground
 */

#include <tsgl.h>

using namespace tsgl;

void testSuite1(Background * bg) {
    ColorFloat colors[] = { ColorFloat(1,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    bg->clear();
    bg->drawSquare(-400, 225, 0, 100, 0,0,0, RED);
    bg->drawSquare(-400, 100, 0, 100, 0,0,0, colors);
    bg->drawCircle(-400, -100, 0, 50, 0,0,0, RED);
    bg->drawCircle(-400, -225, 0, 50, 0,0,0, colors);
    bg->drawEllipse(-225, 225, 0, 75,50, 0,0,0, RED);
    bg->drawEllipse(-225, 100, 0, 75,50, 0,0,0, colors);
    bg->drawRectangle(-225, -100, 0, 150,100, 0,0,0, RED);
    bg->drawRectangle(-225, -225, 0, 150,100, 0,0,0, colors);
    bg->drawImage(0,225,0,"./assets/pics/launch.bmp",100,100,0,0,0);
    bg->drawText(0,100,0,L"ü^xdog","./assets/freefont/FreeMono.ttf",50,0,0,0,RED);
    bg->drawRegularPolygon(0,-100,0,50,5,0,0,0,RED);
    bg->drawRegularPolygon(0,-225,0,50,7,0,0,0,colors);
    bg->drawTriangle(225,275,0,275,175,0,175,175,0,0,0,0,RED);
    bg->drawTriangle(225,50,0,275,150,0,175,150,0,0,0,0,colors);
    float x1[4] = { 160, 225, 225, 290 };
    float y1[4] = {-100, -50,-150,-100 };
    float z1[4] = {   0,   0,   0,   0 };
    bg->drawTriangleStrip(225,-100,0,4,x1,y1,z1,0,0,0,RED);
    float x2[4] = { 160, 225, 225, 290 };
    float y2[4] = {-225,-175,-275,-225 };
    float z2[4] = {   0,   0,   0,   0 };
    bg->drawTriangleStrip(225,-225,0,4,x2,y2,z2,0,0,0,colors);
    bg->drawArrow(400,225,0,100,10,45,0,0,RED,true);
    bg->drawArrow(400,100,0,100,10,-45,0,0,colors);
    bg->drawLine(400,-50,0,100,20,0,0,RED);
    bg->drawLine(400,-150,0,100,-20,0,0,colors);
    float vertices1[12] = { 350,-175,0,
                            350,-200,0,
                            450,-250,0,
                            450,-275,0 };
    bg->drawPolyline(400,-225,0,4,vertices1,0,0,0,RED);
    float vertices2[12] = { 450,-175,0,
                            450,-200,0,
                            350,-250,0,
                            350,-275,0 };
    bg->drawPolyline(400,-225,0,4,vertices2,0,0,0,colors);
}

void testSuite2(Background * bg) {
    ColorFloat colors[] = { ColorFloat(1,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    bg->clear();
    bg->drawStar(-400,225,0,50,5,0,0,0,RED,false);
    bg->drawStar(-400,100,0,50,7,0,0,0,colors,false);
    float x1[6] = { -450,-400,-375,-350,-400,-450 };
    float y1[6] = {  -50, -50, -60,-150,-150,-100 };
    bg->drawConcavePolygon(-400,-100,0,6,x1,y1,0,0,0,RED);
    float x2[6] = { -450,-400,-375,-350,-400,-450 };
    float y2[6] = { -175,-175,-185,-275,-275,-225 };
    bg->drawConcavePolygon(-400,-225,0,6,x2,y2,0,0,0,colors);
    float x3[6] = { -275,-175,-200,-175,-275,-250 };
    float y3[6] = {  275, 275, 225, 175, 175, 225 };
    bg->drawConvexPolygon(-400,225,0,6,x3,y3,0,0,0,RED);
    float x4[6] = { -250,-275,-175,-200,-175,-275 };
    float y4[6] = {  100, 150, 150, 100,  50,  50 };
    bg->drawConvexPolygon(-400,100,0,6,x4,y4,0,0,0,colors);


}

void proceduralFunction(Canvas& can) {
    Background * bg = can.getBackground();
    float x1[6] = { -275,-175,-200,-175,-275,-250 };
    float y1[6] = {  275, 275, 225, 175, 175, 225 };
    ConcavePolygon * c = new ConcavePolygon(0,0,0,6,x1,y1,0,0,0,RED);
    can.add(c);

    Star * s = new Star(0,0,0,50,7,0,0,0,RED,false);
    can.add(s);

    can.bindToButton(TSGL_RIGHT, TSGL_PRESS, [&c]() {
        c->changeXBy(10);
    });

    bool flip = true;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&bg, &flip]() {
        if (flip) {
            testSuite1(bg);
        } else {
            testSuite2(bg);
        }
        flip = !flip;
    });

    while (can.isOpen()) {
        can.sleep();
    }
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.9*Canvas::getDisplayWidth();
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Procedural Background", BLACK);
    c.run(proceduralFunction);
}