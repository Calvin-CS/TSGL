/*
 * testImage.cpp
 *
 * Usage: ./testImage <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws various images on a Canvas.
 * \details Very basic test function showcasing image drawing capabilities.
 * - The first 6 images are drawn opaque.
 * - The 7th image is drawn across the entire Canvas with alpha transparency.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void imageFunction(Canvas& can) {
    int ww = can.getWindowWidth()/3, hh = can.getWindowHeight()/2;
    // Square * s = new Square(5,0,0,50,0,0,0,RED);
    // can.add(s);

    // Pyramid * p = new Pyramid(-5,0,0,5,100,50,0,0,0,BLUE);
    // can.add(p);

    // Image * image = new Image(0,0,0,"./assets/pics/Messier51.jpg", 4,3, 0,0,0);
    // can.add(image);

    Image * image = new Image(-ww,0.5 * hh,0,"pics/ball.png", ww,hh, 0,0,0);
    can.add(image);
    Image * image2 = new Image(0,0.5 * hh,0,"pics/ship.bmp", ww,hh, 0,0,0);
    can.add(image2);
    Image * image3 = new Image(ww,0.5 * hh,0,"pics/shiprgb.bmp", ww,hh, 0,0,0);
    can.add(image3);
    Image * image4 = new Image(-ww,-0.5 * hh,0,"pics/sky_main.jpg", ww,hh, 0,0,0);
    can.add(image4);
    Image * image5 = new Image(0,-0.5 * hh,0,"pics/colorfulKeyboard.jpg", ww,hh, 0,0,0);
    can.add(image5);
    Image * image6 = new Image(ww,-0.5 * hh,0,"pics/cow.jpg", ww,hh, 0,0,0);
    can.add(image6);

    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&image6]() {
        image6->changeFile("pics/colorfulKeyboard.jpg");
    });

    // image->setHeight((GLfloat)image->getPixelHeight());
    // image->setWidth((GLfloat)image->getPixelWidth());

    float floatVal = 0.0f;
    GLfloat delta = 5;
    bool ss = false;
    while (can.isOpen()) {
        can.sleep();
        image->setAlpha((sin(floatVal) + 1) / 2);
        // image->setCenterX(sin(floatVal/90) * 100);
        // image->setCenterY(sin(floatVal/90) * 100);
        // image->setCenterZ(sin(floatVal/90) * 100);
        // image->setYaw(floatVal);
        image->setPitch(floatVal * 100);
        // image->setRoll(floatVal);
        // image->setWidth(sin(floatVal/90) * 100 + 400);
        // image->setHeight(sin(floatVal/90) * 100 + 400);
        // if (image->getWidth() > 500 || image->getWidth() < 300) {
        //     delta *= -1;
        // }
        // image->changeWidthBy(delta);
        // if (image->getHeight() > 500 || image->getHeight() < 300) {
        //     delta *= -1;
        // }
        // image->changeHeightBy(delta);
        // printf("%d\n", can.getFrameNumber());
        // if (can.getFrameNumber() > 50 && !ss) {
        //     can.takeScreenShot();
        //     ss = true;
        // }
        // ColorInt point = can.getPoint(can.getWindowWidth()/2,can.getWindowHeight()/2);
        // printf("%d, %d, %d, %d\n", point.R, point.G, point.B, point.A);
        floatVal += 0.01;
    }

    // can.drawImage("../assets/pics/background.jpg", ww/2, 0, ww*2, hh*2, 0.25f); //Overlay

    delete image;
    delete image2;
    delete image3;
    delete image4;
    delete image5;
    delete image6;
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.5*w;
    if(w <= 0 || h <= 0) { //Check width and height validity
      w = 1.2*Canvas::getDisplayHeight(); h = 0.5*w;
    }
    Canvas c(-1, -1, w, h ,"Images", WHITE);
    c.run(imageFunction);
}
