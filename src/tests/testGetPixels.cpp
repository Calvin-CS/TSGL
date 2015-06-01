/*
 * testGetPixels.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

typedef CartesianCanvas Cart;

/*!
 * \brief Grabs the pixels from an image on the Canvas and plays with them.
 * \details
 * - Set a predetermined number of threads and store it in: \b THREADS.
 * - Store the Canvas' dimensions for easy use.
 * - Draw an image on the initially blank Canvas, stretched to fill it.
 * - Initialize a pointer to the Canvas' screen buffer.
 * - Set up a parallel OMP block with \b THREADS threads.
 * - Set up the internal timer of the Canvas to expire every 1/100th of a second.
 * - Determine a block size for each thread based on the Canvas' height and the number
 * of spawned threads.
 * - Determine a starting row for each thread based on \b blocksize and the thread's id.
 * - While the Canvas is open:
 *   - Initialize a new offset pointer to the Canvas' draw buffer + row*width pixels
 *   ( times 3 colors ).
 *   - For each row:
 *     - Reset the index to 0.
 *     - For each column:
 *       - Over each old pixel, draw a new pixel with each of the RGB components incremented
 *       and wrapped.
 *       .
 *     - Increment the buffer offset by \b width pixels ( times 3 colors ).
 *     .
 *   - Sleep until the Canvas is ready to draw again.
 *   .
 * .
 *
 * \param can, Reference to the Canvas being drawn to
 */
void getPixelsFunction(Canvas& can) {
    const int THREADS = 2;
    unsigned int width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    can.drawImage("assets/test.png", 0, 0, width, height);
    Timer::threadSleepFor(.75);
//    can.recordForNumFrames(100);
//    uint8_t* buffer = can.getScreenBuffer();

    #pragma omp parallel num_threads(THREADS)
    {
        unsigned int blocksize = (double)height / omp_get_num_threads();
        unsigned int row = blocksize * omp_get_thread_num();
        while (can.getIsOpen()) {
//            uint8_t* buffer_offset = buffer + row * width * 3;
            for (unsigned int y = row; y < row + blocksize; y++) {
                for (unsigned int x = 0; x < width; x++) {
                    ColorInt c = can.getPoint(x,y);
                    can.drawPoint(x, y, ColorInt((1+c.R) % 256, (1+c.G) % 256, (1+c.B) % 256));
                }
            }
            can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
            printf("%f\n", can.getTimeBetweenSleeps());
        }
    }
}

int main() {
    Canvas c28(0, 0, 800, 600, "", .01);
    c28.setBackgroundColor(GREY);
    c28.start();
    getPixelsFunction(c28);
    c28.close();
}
