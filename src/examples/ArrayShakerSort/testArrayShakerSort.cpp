/*
 * testArrayShakerSort.cpp
 *
 * Usage: ./testArrayShakerSort [sizeOfArray]
            1 <= sizeOfArray <= 24, defaults to 10
 */

#include <tsgl.h>
#include <cmath>
#include "CubeArray.h"

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
#define FONT "./assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 35                                // font size for all text

using namespace tsgl;

void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

void sortVisualizationFunction(Canvas& can, int arraySize) {
    // Generate random seed
    srand( time(0) );

    // Create arrays to perform operation on
    int originalArray[arraySize];
    // int sortedArray[] = {40, 35, 80, 62, 60, 74, 36, 10, 19, 5};

    // Fill numerical array with random numbers between 0 and 99
    for(unsigned i = 0; i < arraySize; i++){
        originalArray[i] = rand() % (RAND_UPPER + 1);
        // originalArray[i] = i + 90;
    }

    printf("Before shaker sort: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", originalArray[i]);
    }
    printf("\n");

    // Create 3D array
    CubeArray arrayA(0, ARRAY_Y, ARRAY_Z, SIDE_LENGTH, arraySize, originalArray, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);

    // Create text labels
    Text * titleLabel = new Text(0.0, TITLE_Y, TEXT_Z,
                                 L"Array Shaker Sort", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    // Text * speedLabel = new Text(0.0, ARRAY_Y - 100.0, TEXT_Z,
    //                              "Speed: x" + std::to_string(sleepTime/250000), FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);
    Text * sortedLabel = new Text(0.0, ARRAY_Y - 200.0, TEXT_Z,
                                 L"Sorted!", FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);

    // Create arrays to store labels
    Text * labelArray[] = {titleLabel};
    unsigned numLabels = sizeof(labelArray)/sizeof(labelArray[0]);

    // Draw arrays on canvas
    arrayA.draw(can);
    // Draw labels on canvas
    for(unsigned i = 0; i < numLabels; i++){
        can.add(labelArray[i]);
    }

    float sleepTime = 0.5;   // number of seconds to sleep

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
    bool swapped = true; 
    int start = 0; 
    int end = arraySize - 1; 
    while (can.isOpen()) {
        can.sleep();

        if(complete == 0){
            // Shaker sort
            while (swapped) { 
                // reset the swapped flag on entering 
                // the loop, because it might be true from 
                // a previous iteration. 
                swapped = false; 
          
                // Going up
                for (int pos = start; pos < end; ++pos) {
                    arrayA.getCube(pos)->setColor(RED);
                    arrayA.getCube(pos+1)->setColor(BLUE);
                    can.sleepFor(sleepTime);
                    // Compare values
                    if (arrayA[pos] > arrayA[pos + 1]) { 
                        swap(arrayA[pos], arrayA[pos + 1]); 
                        swapped = true;
                        arrayA.update(pos); arrayA.update(pos + 1);
                    }
                    // Animation
                    can.sleepFor(sleepTime);
                    arrayA.getCube(pos)->setColor(ARRAY_COLOR);
                    arrayA.getCube(pos+1)->setColor(ARRAY_COLOR);
                } 
                // if nothing moved, then array is sorted. 
                if (!swapped) 
                    break; 
          
                // otherwise, reset the swapped flag so that it 
                // can be used in the next stage 
                swapped = false;
                arrayA.getCube(end)->setColor(FINISHED_COLOR); 
                --end; 
          
                // Going down
                for (int pos = end - 1; pos >= start; --pos) {
                    arrayA.getCube(pos)->setColor(RED);
                    arrayA.getCube(pos+1)->setColor(BLUE);
                    can.sleepFor(sleepTime); 
                    if (arrayA[pos] > arrayA[pos + 1]) { 
                        swap(arrayA[pos], arrayA[pos + 1]); 
                        swapped = true; 
                        arrayA.update(pos); arrayA.update(pos + 1);
                    }
                    // Animation
                    can.sleepFor(sleepTime);
                    arrayA.getCube(pos)->setColor(ARRAY_COLOR);
                    arrayA.getCube(pos+1)->setColor(ARRAY_COLOR);
                }  
                arrayA.getCube(start)->setColor(FINISHED_COLOR); 
                ++start; 
            } 
            arrayA.setColor(FINISHED_COLOR);
            can.add(sortedLabel);
            complete = 1;
        }
    }

    // Output
    printf("\nAfter shaker sort:  ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", arrayA[i]);
    }
    printf("\n\n");

    // Deallocate all object memory
    for (unsigned i = 0; i < numLabels; i++){
        delete labelArray[i];
    }
    delete sortedLabel;

}

int main(int argc, char* argv[]){
    int sizeOfArray = (argc > 1) ? atoi(argv[1]) : 10;

    // Checks validity of sizeOfArray; if invalid, set to default
    if((sizeOfArray <= 0 or sizeOfArray > 24)){
        printf("Invalid argument(s).\
                \nUsage: ./testArrayShakerSort [sizeOfArray]\n \
                1 <= sizeOfArray <= 24\
                \nUsing default parameters...\n");
        sizeOfArray = 10;
    }

    printf("Use the up and down arrow keys to speed up and slow down animation, respectfully.\n");

    Canvas c(0, -1, 1820, 620, "Array Shaker Sort");
    c.setBackgroundColor(BLACK);
    c.run(sortVisualizationFunction, sizeOfArray);
}



