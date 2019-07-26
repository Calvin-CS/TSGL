/**
 * testRotation.cpp tests various Drawable rotation functionalities.
 */
#include "tsgl.h"

using namespace tsgl;

void rotationFunction(Canvas& can) {
  TextureHandler loader;

  Line * l = new Line(0,0,50,50, PURPLE);
  l->setCenter(can.getWindowWidth() / 4, can.getWindowHeight() / 2);

  Square * sq = new Square(50, 50, 50, BLUE);
  sq->setCenter(can.getWindowWidth() / 2, 3 * can.getWindowHeight() / 4);

  Image * im = new Image("../assets/pics/test.png", loader, 0, 0, 50, 50);
  im->setCenter(can.getWindowWidth() / 2, can.getWindowHeight() / 4);

  Text * text = new Text(L"dog", 100, 100, 32, GREEN);
  text->setCenter(3* can.getWindowWidth() / 4, can.getWindowHeight() / 2);

  Arrow * a = new Arrow(0,0,0,50,ORANGE, true);
  a->setCenter(can.getWindowWidth() / 2, can.getWindowHeight()/2);

  l->setRotationPoint(can.getWindowWidth() / 2, can.getWindowHeight() / 2);
  im->setRotationPoint(can.getWindowWidth() / 2, can.getWindowHeight() / 2);
  text->setRotationPoint(can.getWindowWidth() / 2, can.getWindowHeight() / 2);
  sq->setRotationPoint(can.getWindowWidth() / 2, can.getWindowHeight() / 2);
  a->setRotationPoint(can.getWindowWidth() / 2, can.getWindowHeight() / 2);

  can.add(sq);
  can.add(im);
  can.add(text);
  can.add(l);
  can.add(a);

  float rotation = 0;
  while(can.isOpen()) {
    sq->setRotation(rotation);
    im->setRotation(rotation);
    text->setRotation(rotation);
    l->setRotation(rotation);
    a->setRotation(rotation);
    rotation += PI/365;
    can.sleepFor(.02);
  }

  delete l;
  delete sq;
  delete im;
  delete text;
  delete a;
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  Canvas c(-1, -1, w, h, "Click the Target!");    //Can change the size of the window
  c.run(rotationFunction);
}