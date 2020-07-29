/*
 * testForestFire.cpp
 *
 * Usage: ./testForestFire <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Pseudo-simulates a forest fire using a lot of probability and randomness.
 * \details
 * - Store the Canvas's dimensions for ease of use.
 * - Set the fire's life, strength, and maximum spread distance to some predetermined numbers.
 * - Seed the random number generator.
 * - Allocate arrays for storing each pixel's onFire status and flammability.
 * - For each pixel:
 *   - Get its distance from the center of the Canvas.
 *   - Set its flammability and color based upon a semi-arbitrary single-line function.
 *   .
 * - Draw 32 random square "lakes" with very low flammabilities onto the Canvas.
 * - Declare a mini-firePoint struct with coordinates, life, and strength.
 * - Make a 3x3 square of fire in the middle of the Canvas, and color the pixels accordingly.
 * - The internal timer of the Canvas is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - For each fire point:
 *     - Pop it from the queue, pushing it back on only if its life > 0.
 *     - For each cell adjacent to the fire, if it is not on the edge of the screen, not already
 *       on fire, and the random number generator rolls a number lower than the cell's flammability,
 *       set that cell on fire.
 *     .
 *   .
 * - Deallocate the onFire and flammability arrays.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void forestFireFunction(Canvas& can) {
    Background * bg = can.getBackground();
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    const float LIFE = 10,
                STRENGTH = 0.03,
                MAXDIST = sqrt(WINDOW_W * WINDOW_W + WINDOW_H * WINDOW_H) / 2;
    // srand(time(NULL));  // Seed the random number generator
    bool* onFire = new bool[WINDOW_W * WINDOW_H]();
    float* flammability = new float[WINDOW_W * WINDOW_H]();
    //Setting each pixel's flammablity
    for (int i = 0; i < WINDOW_W; i++) {  // For each individual point
        for (int j = 0; j < WINDOW_H; j++) {
            float xi = std::abs(WINDOW_W / 2 - i);
            float yi = std::abs(WINDOW_H / 2 - j);
            float tdist = (MAXDIST - sqrt(xi * xi + yi * yi)) / MAXDIST;
            float f = 0.01 + (i * j % 100) / 100.0 * randfloat(100) / 2 * tdist;
            flammability[i * WINDOW_H + j] = f;
            bg->drawPixel(i - WINDOW_W/2, WINDOW_H/2 - j, ColorFloat(0.0f, f, 0.0f, 1.0f));
        }
    }
    can.sleep(); // sleep in order to get all the pixels rendered before lakes
    //"Lakes"
    for (int reps = 0; reps < 32; reps++) {
        int x = saferand(-WINDOW_W/2, WINDOW_W/2);
        int y = saferand(-WINDOW_H/2, WINDOW_H/2);
        int w = saferand(0, WINDOW_W/2 - abs(x)) * 2;
        int h = saferand(0, WINDOW_H/2 - abs(y)) * 2;
        if (w > 32) w = 32;
        if (h > 32) h = 32;
        for (int i = -w/2; i < w/2; i++) {
            for (int j = -h/2; j < h/2; j++) {
                flammability[(x + i + WINDOW_W/2) * WINDOW_H + (y + j + WINDOW_H/2)] = 0.01;
                bg->drawPixel(x + i, y + j, ColorFloat(0.0f, 0.0f, 1.0f, 0.25f));
            }
        }
    }
    struct firePoint {
        int x;
        int y;
        float life;
        float strength;
    };
    std::queue<firePoint> fires;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            firePoint fire = { WINDOW_W / 2 - 1 + i, WINDOW_H / 2 - 1 + j, LIFE, STRENGTH };
            fires.push(fire);
            bg->drawPixel(0 - 1 + i, 0 - 1 + j, ColorFloat(1.0f, 0.0f, 0.0f, STRENGTH));
        }
    }
    while (can.isOpen()) {
        can.sleep();
        int l = fires.size();
        for (int i = 0; i < l; i++) {
            firePoint f = fires.front();
            fires.pop();
            if (--f.life > 0) fires.push(f);
            int myCell = f.x * WINDOW_H + f.y;
            if (f.x > 0 && !onFire[myCell - WINDOW_H] && randfloat() < flammability[myCell - WINDOW_H]) {
                firePoint fire = { f.x - 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - WINDOW_H] = true;
                bg->drawPixel(f.x - WINDOW_W/2 - 1, f.y - WINDOW_H/2, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.x < WINDOW_W - 1 && !onFire[myCell + WINDOW_H]
                && randfloat() < flammability[myCell + WINDOW_H]) {
                firePoint fire = { f.x + 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + WINDOW_H] = true;
                bg->drawPixel(f.x - WINDOW_W/2 + 1, f.y - WINDOW_H/2, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y > 0 && !onFire[myCell - 1] && randfloat() < flammability[myCell - 1]) {
                firePoint fire = { f.x, f.y - 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - 1] = true;
                bg->drawPixel(f.x - WINDOW_W/2, f.y - WINDOW_H/2 - 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y < WINDOW_H && !onFire[myCell + 1] && randfloat() < flammability[myCell + 1]) {
                firePoint fire = { f.x, f.y + 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + 1] = true;
                bg->drawPixel(f.x - WINDOW_W/2, f.y - WINDOW_H/2 + 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
        }
    }
    delete[] onFire;
    delete[] flammability;
}

//Takes width and height as command line arguments
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1200;
      h = 900;                  //If not, set the width and height to a default value
    }
    Canvas c(-1, -1, w, h, "Forest Fire");
    c.run(forestFireFunction);
}
