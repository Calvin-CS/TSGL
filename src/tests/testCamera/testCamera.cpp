/*
 * testCamera.cpp
 *
 * Usage: ./testCamera
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cameraFunction(Canvas& can) {
    Camera * camera = can.getCamera();

    ColorFloat colors[8] = { ColorFloat(1,0,0,1), ColorFloat(0,1,0,1),
                             ColorFloat(0,0,1,1), ColorFloat(1,1,0,1),
                             ColorFloat(1,0,1,1), ColorFloat(0,1,1,1),
                             ColorFloat(0.25,1,0.25,1), ColorFloat(0.25,0.25,1,1)};
    Cube * cube = new Cube(0,0,0,200,0,0,0,colors);
    can.add(cube);

    can.bindToButton(TSGL_RIGHT, TSGL_PRESS, [&camera]() {
        camera->moveRight(25);
    });
    can.bindToButton(TSGL_LEFT, TSGL_PRESS, [&camera]() {
        camera->moveLeft(25);
    });
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&camera]() {
        camera->moveUp(25);
    });
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&camera]() {
        camera->moveDown(25);
    });
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&camera]() {
        camera->moveForward(25);
    });
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&camera]() {
        camera->moveBackward(25);
    });

    can.bindToButton(TSGL_W, TSGL_PRESS, [&camera]() {
        camera->changePitchBy(5);
    });
    can.bindToButton(TSGL_S, TSGL_PRESS, [&camera]() {
        camera->changePitchBy(-5);
    });
    can.bindToButton(TSGL_A, TSGL_PRESS, [&camera]() {
        camera->changeYawBy(-5);
    });
    can.bindToButton(TSGL_D, TSGL_PRESS, [&camera]() {
        camera->changeYawBy(5);
    });

    can.bindToButton(TSGL_V, TSGL_PRESS, [&camera]() {
        printf("%f\n", camera->getYaw());
    }); 

    can.bindToButton(TSGL_B, TSGL_PRESS, [&camera]() {
        printf("%f\n", camera->getPitch());
    }); 

    /* making sure lookAt() works properly from all angles */
    // float d = camera->getPositionZ();
    // camera->setPositionY(d/2);
    // camera->lookAt(0,0,0);

    // can.bindToButton(TSGL_1, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(0);
    //     camera->setPositionZ(d);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_2, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(-d/3);
    //     camera->setPositionZ(2*d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_3, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(-2*d/3);
    //     camera->setPositionZ(d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_4, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(-d);
    //     camera->setPositionZ(0);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_5, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(-2*d/3);
    //     camera->setPositionZ(-d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_6, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(-d/3);
    //     camera->setPositionZ(-2*d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_7, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(0);
    //     camera->setPositionZ(-d);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_8, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(d/3);
    //     camera->setPositionZ(-2*d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_9, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(2*d/3);
    //     camera->setPositionZ(-d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_0, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(d);
    //     camera->setPositionZ(0);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_R, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(2*d/3);
    //     camera->setPositionZ(d/3);
    //     camera->lookAt(0,0,0);
    // });

    // can.bindToButton(TSGL_T, TSGL_PRESS, [&camera, &d]() {
    //     camera->setPositionX(d/3);
    //     camera->setPositionZ(2*d/3);
    //     camera->lookAt(0,0,0);
    // });

    while (can.isOpen()) {
        can.sleep();
    }
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Moving Camera", BLACK);
    c.run(cameraFunction);
}