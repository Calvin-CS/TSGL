/*
 * testArrayBubbleSort.cpp
 *
 * Usage: ./testArrayBubbleSort [sizeOfArray]
            1 <= sizeOfArray <= 24, defaults to 10
 */

#include <cmath>
#include "CubeArray.h"
using namespace std;

// Size constants
#define SIDE_LENGTH 50      // side length of all array blocks
#define RAND_UPPER 99        // maximum value of the random numbers in the arrays

// Space/distance constants
#define TITLE_Y 225.0       // y-coordinate of title label
#define ARRAY_Y 0.0         // y-coordinate for Array
#define ARRAY_Z 0.0         // z-coordinate for Array
#define TEXT_Z SIDE_LENGTH/2.0   // z-coordinate for all Text objects
#define YAW 0.0                  // yaw value for all Drawable objecs
#define PITCH 0.0                // pitch value for all Drawable objects
#define ROLL 0.0                 // roll value for all Drawable objects

// Other constants
#define ARRAY_COLOR ColorFloat(0.5, 0.5, 0.5, 1)    // color value for all arrays (gray)
#define NUM_COLOR WHITE                             // color value for all numbers
#define FINISHED_COLOR ColorFloat(0, 0.6, 0, 1)     // color value for sorted numbers (green)
#define FONT_SIZE 35                                // font size for all text

using namespace tsgl;

void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

void sortVisualizationFunction(Canvas& can, int arraySize) {
    // Create arrays to perform operation on
    int originalArray[arraySize];

    // Fill numerical array with random numbers between 0 and 99
    for(unsigned i = 0; i < arraySize; i++){
        originalArray[i] = saferand(0, RAND_UPPER + 1);
    }

    printf("Before bubble sort: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", originalArray[i]);
    }
    printf("\n");

    // Create 3D array
    CubeArray arrayA(0, ARRAY_Y, ARRAY_Z, SIDE_LENGTH, arraySize, originalArray, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);

    // Create text labels
    Text * titleLabel = new Text(0.0, TITLE_Y, TEXT_Z,
                                 L"Array Bubble Sort", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    Text * sortedLabel = new Text(0.0, ARRAY_Y - 200.0, TEXT_Z,
                                 L"Sorted!", FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);

    // Draw arrays on canvas
    arrayA.draw(can);
    // Draw labels on canvas
    can.add(titleLabel);

    float sleepTime = 0.125;      // initial number of seconds to sleep

    // Key bindings to speed up/slow down animation
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&sleepTime](){
        sleepTime /= 2;
    });
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&sleepTime](){
        if(sleepTime < 8){
            sleepTime *= 2;
        }
    });

    unsigned complete = 0;      // ensures animation only runs once
    while (can.isOpen()) {
        can.sleep();
        if(complete == 0){
            int end = arraySize;
            // Bubble sort algorithm
            for(unsigned i = 0; i < arrayA.getSize(); i++){
                for(unsigned j = 1; j < end; j++){ // OR j < arrayA.getSize();
                    arrayA.getCube(j)->setColor(RED);
                    arrayA.getCube(j-1)->setColor(BLUE);
                    can.sleepFor(sleepTime);
                    if(arrayA[j] < arrayA[j-1]){
                        swap(arrayA[j], arrayA[j-1]);
                        arrayA.update(j);
                        arrayA.update(j-1);
                        can.sleepFor(sleepTime);
                    }
                    arrayA.getCube(j)->setColor(ARRAY_COLOR);
                    arrayA.getCube(j-1)->setColor(ARRAY_COLOR);
                    can.sleepFor(sleepTime);
                }
                arrayA.getCube(end-1)->setColor(FINISHED_COLOR);
                end--;
            }
            arrayA.setColor(FINISHED_COLOR);
            can.add(sortedLabel);
            complete = 1;
        }
    }

    // Output
    printf("\nAfter bubble sort:  ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", arrayA[i]);
    }
    printf("\n\n");

    // Deallocate all object memory
    delete titleLabel;
    delete sortedLabel;

}

int main(int argc, char* argv[]){
    int sizeOfArray = (argc > 1) ? atoi(argv[1]) : 10;

    // Checks validity of sizeOfArray; if invalid, set to default
    if((sizeOfArray <= 0 or sizeOfArray > 24)){
        printf("Invalid argument(s).\
                \nUsage: ./testArrayBubbleSort [sizeOfArray]\n \
                1 <= sizeOfArray <= 24\
                \nUsing default parameters...\n");
        sizeOfArray = 10;
    }

    Canvas c(0, -1, Canvas::getDisplayWidth(), 620, "Array Bubble Sort", BLACK);
    c.run(sortVisualizationFunction, sizeOfArray);
}
