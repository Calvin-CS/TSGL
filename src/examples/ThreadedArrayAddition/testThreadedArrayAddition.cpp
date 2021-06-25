/*
 * testThreadedArrayAddition.cpp
 *
 * Usage: ./testThreadedArrayAddition [threadingType] [numberOfThreads] [sizeOfArrays]
            threadingType = 0 (slices) or 1 (chunks), defaults to 0
            1 <= numberOfThreads <= 12, defaults to 4
            1 <= sizeOfArrays <= 24, defaults to 10
 */

#include <cmath>
#include "CubeArray.h"

// Size constants
#define SIDE_LENGTH 50      // side length of all array blocks
#define RAND_UPPER 49       // maximum value of the random numbers in the arrays

// Space/distance constants
#define TITLE_Y 225.0       // y-coordinate of title label
#define ARRAY_A_Y TITLE_Y - 125.0        // y-coordinate for Array 1 blocks
#define ARRAY_B_Y TITLE_Y - (2 * 125.0)  // y-coordinate for Array 2 blocks
#define ARRAY_C_Y TITLE_Y - (3 * 125.0)  // y-coordinate for Array 3 blocks
#define CUBE_Z 0.0               // z-coordinate for all Cube objects
#define TEXT_Z SIDE_LENGTH/2.0   // z-coordinate for all Text objects
#define YAW 0.0                  // yaw value for all Drawable objecs
#define PITCH 0.0                // pitch value for all Drawable objects
#define ROLL 0.0                 // roll value for all Drawable objects

// Other constants
#define ARRAY_COLOR ColorFloat(0.5, 0.5, 0.5, 1)    // color value for all arrays (gray)
#define NUM_COLOR WHITE                             // color value for all numbers
#define FONT "/usr/local/include/TSGL/assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 25                                // font size for all text

using namespace tsgl;

// Stores data for threads running in parallel
class ThreadData{
private:
    unsigned threadID;
    ColorFloat myColor;
    Cube* myArrayABlock;
    Cube* myArrayBBlock;
    Cube* myArrayCBlock;
public:
    // Constructor
    ThreadData(unsigned tid, ColorFloat color, Cube* arrayABlock, Cube* arrayBBlock, Cube* arrayCBlock){
        threadID = tid;
        myColor = color;
        myArrayABlock = arrayABlock;
        myArrayBBlock = arrayBBlock;
        myArrayCBlock = arrayCBlock;
    }
    // Runs the animation for threaded array addition process
    void runAddition(Canvas& can, float sleepTime){
        can.sleepFor(sleepTime);

        // get value from arrayA
        myArrayABlock->setColor(myColor);
        can.sleepFor(sleepTime);

        // get value from arrayB
        myArrayBBlock->setColor(myColor);
        can.sleepFor(sleepTime);

        // enter sum into arrayC
        myArrayCBlock->setColor(myColor);
    }
};


void arrayFunction(Canvas& can, int threadingType, int numThreads, int arraySize) {
    // Generate random seed
    srand( time(0) );

    // Create arrays to sum
    int numArrayA[arraySize];
    int numArrayB[arraySize];
    int numArrayC[arraySize];

    // Fill numerical arrays A and B with random numbers between 0 and 49
    for(unsigned i = 0; i < arraySize; i++){
        numArrayA[i] = (rand() % (RAND_UPPER+1));
        numArrayB[i] = (rand() % (RAND_UPPER+1));
    }

    // Create 3D arrays
    CubeArray arrayA(0, ARRAY_A_Y, CUBE_Z, SIDE_LENGTH, arraySize, numArrayA, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);
    CubeArray arrayB(0, ARRAY_B_Y, CUBE_Z, SIDE_LENGTH, arraySize, numArrayB, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);
    CubeArray arrayC(0, ARRAY_C_Y, CUBE_Z, SIDE_LENGTH, arraySize, numArrayC, 0, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);

    // Create text labels
    Text * arrayALabel = new Text(arrayA.getCube(0)->getCenterX() - SIDE_LENGTH, ARRAY_A_Y, TEXT_Z,
                                 L"a", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    Text * arrayBLabel = new Text(arrayB.getCube(0)->getCenterX() - SIDE_LENGTH, ARRAY_B_Y, TEXT_Z,
                                 L"b", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    Text * arrayCLabel = new Text(arrayC.getCube(0)->getCenterX() - SIDE_LENGTH, ARRAY_C_Y, TEXT_Z,
                                 L"c", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    Text * titleLabel = new Text(0.0, TITLE_Y, TEXT_Z,
                                 L"Array Addition: c[i] = a[i] + b[i]", FONT, FONT_SIZE + 20, YAW, PITCH, ROLL, YELLOW);

    // Create arrays to store labels
    Text * labelArray[] = {arrayALabel, arrayBLabel, arrayCLabel, titleLabel};
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
    arrayB.draw(can);
    arrayC.draw(can);
    // Draw labels on canvas
    for(unsigned i = 0; i < numLabels; i++){
        can.add(labelArray[i]);
    }

    float sleepTime = 0.5;      // initial number of seconds to sleep
    unsigned complete = 0;      // ensures animation only runs once
    while (can.isOpen()) {
        can.sleep();
        if(complete == 0 and threadingType == 0){
            //**************************** Parallel ******************************
            #pragma omp parallel num_threads(numThreads)
            {
                unsigned tid = omp_get_thread_num();

                #pragma omp for schedule(static)
                for(unsigned i = 0; i < arraySize; i++){

                    numArrayC[i] = numArrayA[i] + numArrayB[i];

                    ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], arrayA.getCube(i), arrayB.getCube(i), 
                                                        arrayC.getCube(i));
                    td->runAddition(can, sleepTime);
                    arrayC[i] = numArrayC[i];
                    arrayC.update(i);
                    can.sleepFor(sleepTime);

                    delete td;                    
                }
            }
            //********************************************************************
            complete = 1;
        }
        if(complete == 0 and threadingType == 1){
            //**************************** Parallel ******************************
            #pragma omp parallel num_threads(numThreads)
            {
                unsigned tid = omp_get_thread_num();

                #pragma omp for schedule(static, 1)
                for(unsigned i = 0; i < arraySize; i++){

                    numArrayC[i] = numArrayA[i] + numArrayB[i];

                    ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], arrayA.getCube(i), arrayB.getCube(i), 
                                                        arrayC.getCube(i));
                    td->runAddition(can, sleepTime);
                    arrayC[i] = numArrayC[i];
                    arrayC.update(i);
                    can.sleepFor(sleepTime);
                    
                    delete td;                    
                }
            }
            //********************************************************************
            complete = 1;
        }
    }

    // Output
    printf("\nAdded two arrays of size %d using %d threads\n", arraySize, numThreads);
    printf("\nArray A: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", numArrayA[i]);
    }
    printf("\nArray B: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", numArrayB[i]);
    }
    printf("\nArray C: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", numArrayC[i]);
    }
    printf("\n");

    // Deallocate all object memory
    for (unsigned i = 0; i < numLabels; i++){
        delete labelArray[i];
    }

}

int main(int argc, char* argv[]){
    int threadingType = (argc > 1) ? atoi(argv[1]) : 0;
    int numberOfThreads = (argc >2) ? atoi(argv[2]) : 4;
    int sizeOfArrays = (argc > 3) ? atoi(argv[3]) : 10;

    // Checks validity of threadingType, numThreads, and sizeOfArrays; if one is invalid, set all to default
    if((threadingType < 0 or threadingType > 1) or (sizeOfArrays <= 0 or sizeOfArrays > 24) or
        (numberOfThreads <= 0 or numberOfThreads > 12)){
        printf("Invalid argument(s).\
                \nUsage: ./testThreadedArrayAddition [threadingType] [numberOfThreads] [sizeOfArrays]\n \
                threadingType = 0 (slices) or 1 (chunks)\n \
                1 <= numberOfThreads <= 12\n \
                1 <= sizeOfArrays <= 24\
                \nUsing default parameters...\n");
        threadingType = 0;
        numberOfThreads = 4;
        sizeOfArrays = 10;
    }

    Canvas c(0, -1, Canvas::getDisplayWidth(), 620, "Threaded Array Addition", BLACK);
    c.run(arrayFunction, threadingType, numberOfThreads, sizeOfArrays);
}
