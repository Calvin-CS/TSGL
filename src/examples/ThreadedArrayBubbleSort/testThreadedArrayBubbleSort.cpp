/*
 * testArrayBubbleSort.cpp
 *
 * Usage: ./testThreadedArrayBubbleSort [numberOfThreads] [sizeOfArrays]
            1 <= numberOfThreads <= 12, defaults to 4
            1 <= sizeOfArrays <= 24, defaults to 10
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
#define ARRAY_COLOR ColorFloat(0.5, 0.5, 0.5, 1)    // color value for all arrays
#define NUM_COLOR WHITE                             // color value for all numbers
#define FINISHED_COLOR ColorFloat(0, 0.6, 0, 1)     // color value for sorted numbers (green)
#define FONT "./assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 35                                // font size for all text

using namespace tsgl;

// Stores data for threads running in parallel
class ThreadData{
private:
    unsigned threadID;
    ColorFloat myColor;
    unsigned myIndex;
public:
    // Constructor
    ThreadData(unsigned tid, ColorFloat color, unsigned index){
        threadID = tid;
        myColor = color;
        myIndex = index;
    }

    // Swaps the positions of two values in the array
    void swap(int& a, int& b){
        int temp = a;
        a = b;
        b = temp;
    }

    // Runs the sorting and animation for threaded array bubble sort process
    void runSort(CubeArray& arr, Canvas& can, float sleepTime){
        can.sleepFor(sleepTime);

        // Get value from first block
        arr.getCube(myIndex)->setColor(myColor);
        // Get value from second block
        arr.getCube(myIndex+1)->setColor(myColor);
        can.sleepFor(sleepTime);

        // Compare values
        if(arr[myIndex] > arr[myIndex+1]){
            swap(arr[myIndex], arr[myIndex+1]);
            arr.update(myIndex); arr.update(myIndex+1);
        }
        can.sleepFor(sleepTime);

        // Leave blocks
        arr.getCube(myIndex)->setColor(ARRAY_COLOR);
        arr.getCube(myIndex+1)->setColor(ARRAY_COLOR);
    }
};

void sortVisualizationFunction(Canvas& can, int numThreads, int arraySize) {
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

    printf("Before bubble sort: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", originalArray[i]);
    }
    printf("\n");

    // Create 3D array
    CubeArray arrayA(0, ARRAY_Y, ARRAY_Z, SIDE_LENGTH, arraySize, originalArray, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);

    // Create text labels
    Text * titleLabel = new Text(0.0, TITLE_Y, TEXT_Z,
                                 L"Array Bubble Sort in Parallel", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    // Text * speedLabel = new Text(0.0, ARRAY_Y - 100.0, TEXT_Z,
    //                              "Speed: x" + std::to_string(sleepTime/250000), FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);
    Text * sortedLabel = new Text(0.0, ARRAY_Y - 200.0, TEXT_Z,
                                 L"Sorted!", FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);

    // Create arrays to store labels
    Text * labelArray[] = {titleLabel};
    unsigned numLabels = sizeof(labelArray)/sizeof(labelArray[0]);

    // Create arrays to store colors
    ColorFloat colorArray[] = {ColorFloat(0.6, 0, 0, 1), // red
                                 ColorFloat(0.6, 0.298, 0, 1), // brown
                                 ColorFloat(0.6, 0.6, 0, 1), // yellow
                                 ColorFloat(0.298, 0.6, 0, 1), // yellow-green
                                 ColorFloat(0, 0.6, 0, 1), // green
                                 ColorFloat(0, 0.6, 0.298, 1), // blue-green
                                 ColorFloat(0, 0.6, 0.6, 1), // turquoise
                                 ColorFloat(0, 0.298, 0.6, 1), // blue
                                 ColorFloat(0, 0, 0.6, 1), // dark blue
                                 ColorFloat(0.298, 0, 0.6, 1), // purple
                                 ColorFloat(0.6, 0, 0.6, 1), // magenta
                                 ColorFloat(0.6, 0, 0.298, 1)}; // dark magenta
    unsigned numColors = sizeof(colorArray)/sizeof(colorArray[0]);

    // Draw arrays on canvas
    arrayA.draw(can);
    // Draw labels on canvas
    for(unsigned i = 0; i < numLabels; i++){
        can.add(labelArray[i]);
    }

    float sleepTime = 0.125;      // initial number of seconds to sleep 0.5

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
            // Parallel bubble sort
            for(int i = 0; i < arrayA.getSize(); i++){
                int first = i%2;
                
                //**************************** Parallel ******************************
                #pragma omp parallel num_threads(numThreads)
                {
                    unsigned tid = omp_get_thread_num();

                    #pragma omp for schedule(static)
                    for(unsigned j = first; j < arraySize - 1; j+=2){
                        ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], j);
                        td->runSort(arrayA, can, sleepTime);
                        delete td;
                    }
                }
                //********************************************************************       
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
    printf("\nSorted array of size %d using %d threads.\n", arraySize, numThreads);

    // Deallocate all object memory
    for (unsigned i = 0; i < numLabels; i++){
        delete labelArray[i];
    }
    delete sortedLabel;

}

int main(int argc, char* argv[]){
    int numberOfThreads = (argc >1) ? atoi(argv[1]) : 4;
    int sizeOfArrays = (argc > 2) ? atoi(argv[2]) : 10;

    // Checks validity of numThreads and sizeOfArrays; if one is invalid, set all to default
    if((sizeOfArrays <= 0 or sizeOfArrays > 24) or (numberOfThreads <= 0 or numberOfThreads > 12)){
        printf("Invalid argument(s).\
                \nUsage: ./testThreadedArrayBubbleSort [numberOfThreads] [sizeOfArrays]\n \
                1 <= numberOfThreads <= 12\n \
                1 <= sizeOfArrays <= 24\
                \nUsing default parameters...\n");
        numberOfThreads = 4;
        sizeOfArrays = 10;
    }

    printf("Use the up and down arrow keys to speed up and slow down the animation, respectively.\n");

    Canvas c(0, -1, Canvas::getDisplayWidth(), 620, "Threaded Array Bubble Sort", BLACK);
    c.run(sortVisualizationFunction, numberOfThreads, sizeOfArrays);
}