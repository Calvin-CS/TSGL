/*
 * testThreadedArrayOperations.cpp
 *
 * Usage: ./testThreadedArrayOperations [operation] [numberOfThreads] [sizeOfArray]
            operation = 0 (addition), 1 (multiplication), or 2 (exponentiation), defaults to 0
            1 <= numberOfThreads <= 12, defaults to 4
            1 <= sizeOfArray <= 24, defaults to 10
 */

#include <cmath>
#include "CubeArray.h"

// Size constants
#define SIDE_LENGTH 50      // side length of all array blocks
#define RAND_UPPER 9        // maximum value of the random numbers in the arrays

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
#define FONT "./assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 35                                // font size for all text

using namespace tsgl;

// Stores data for threads running in parallel
class ThreadData{
private:
    unsigned threadID;
    ColorFloat myColor;
    Cube* myArrayBlock;
public:
    ThreadData(unsigned tid, ColorFloat color, Cube* arrayBlock){
        threadID = tid;
        myColor = color;
        myArrayBlock = arrayBlock;
    }
    // Runs the animation for threaded array addition processe
    void runAnimation(Canvas& can, float sleepTime){
        can.sleepFor(sleepTime);

        // get value from array
        myArrayBlock->setColor(myColor);
        can.sleepFor(sleepTime);
    }
};

void operationVisualization(Canvas& can, int operation, int numThreads, int arraySize) {
    // Generate random seed
    srand( time(0) );

    // Create arrays to perform operation on
    int numArray[arraySize];

    // Fill numerical array with random numbers between 0 and 9
    for(unsigned i = 0; i < arraySize; i++){
        numArray[i] = (rand() % (RAND_UPPER+1));
    }

    // Create 3D arrays
    CubeArray arrayA(0, ARRAY_Y, ARRAY_Z, SIDE_LENGTH, arraySize, numArray, arraySize, YAW, PITCH, ROLL, ARRAY_COLOR, NUM_COLOR);

    // Create text labels
    Text * arrayALabel = new Text(arrayA.getCube(0)->getCenterX() - SIDE_LENGTH, ARRAY_Y, TEXT_Z,
                                 L"a", FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);
    Text * titleLabel = new Text(0.0, TITLE_Y, TEXT_Z,
                                 L"Array Scalar Operations: a +/*/^ n", FONT, FONT_SIZE + 10, YAW, PITCH, ROLL, YELLOW);
    Text * operationNumLabel = new Text(0.0, TITLE_Y-50, TEXT_Z,
                                 L"n = 0", FONT, FONT_SIZE, YAW, PITCH, ROLL, YELLOW);

    // Create arrays to store labels
    Text * labelArray[] = {arrayALabel, titleLabel, operationNumLabel};
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

    float sleepTime = 0.5;      // initial number of seconds to sleep
    unsigned complete = 0;      // ensures animation only runs once
    while (can.isOpen()) {
        can.sleep();
        // ADDITION
        if(complete == 0 and operation == 0){
            // Generate random number to add (0 - 990)
            int operationNum = rand() % 991;
            // Set labels to match operation
            titleLabel->setText(L"Array Scalar Operations: a + n");
            operationNumLabel->setText(L"n = " + std::to_wstring(operationNum));

            //**************************** Parallel ******************************
            #pragma omp parallel num_threads(numThreads)
            {
                unsigned tid = omp_get_thread_num();

                #pragma omp for schedule(static)
                for(unsigned i = 0; i < arraySize; i++){

                    ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], arrayA.getCube(i));
                    td->runAnimation(can, sleepTime);
                    arrayA[i] = arrayA[i] + operationNum;
                    arrayA.update(i);
                    can.sleepFor(sleepTime);

                    delete td;                    
                }
            }
            //********************************************************************
            printf("\nAdded %d to array of size %d using %d threads\n", operationNum, arraySize, numThreads);
            complete = 1;
        }

        // MULTIPLICATION
        if(complete == 0 and operation == 1){
            // Generate random number to mulitply by (1 - 111)
            int operationNum = (rand() % 111) + 1;
            // Set labels to match operation
            titleLabel->setText(L"Array Scalar Operations: a * n");
            operationNumLabel->setText(L"n = " + std::to_wstring(operationNum));

            //**************************** Parallel ******************************
            #pragma omp parallel num_threads(numThreads)
            {
                unsigned tid = omp_get_thread_num();

                #pragma omp for schedule(static)
                for(unsigned i = 0; i < arraySize; i++){

                    ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], arrayA.getCube(i));
                    td->runAnimation(can, sleepTime);
                    arrayA[i] = arrayA[i] * operationNum;
                    arrayA.update(i);
                    can.sleepFor(sleepTime);

                    delete td;                    
                }
            }
            //********************************************************************
            printf("\nMultiplied array of size %d by %d using %d threads\n", arraySize, operationNum, numThreads);
            complete = 1;
        }

        // EXPONENTIATION
        if(complete == 0 and operation == 2){
            // Generate random number to exponentiate by (2 - 3)
            unsigned int operationNum = (rand() % 2) + 2;
            // Set labels to match operation
            titleLabel->setText(L"Array Scalar Operations: a^n");
            operationNumLabel->setText(L"n = " + std::to_wstring(operationNum));

            //**************************** Parallel ******************************
            #pragma omp parallel num_threads(numThreads)
            {
                unsigned tid = omp_get_thread_num();

                #pragma omp for schedule(static)
                for(unsigned i = 0; i < arraySize; i++){

                    ThreadData * td = new ThreadData(tid, colorArray[tid%numColors], arrayA.getCube(i));
                    td->runAnimation(can, sleepTime);
                    arrayA[i] = pow(arrayA[i], operationNum);
                    arrayA.update(i);
                    can.sleepFor(sleepTime);

                    delete td;                    
                }
            }
            //********************************************************************
            printf("\nExponentiated array of size %d by %d using %d threads\n", arraySize, operationNum, numThreads);
            complete = 1;
        }
    }

    // Output
    printf("Before scalar operation: ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", numArray[i]);
    }
    printf("\nAfter scalar operation:  ");
    for(unsigned i = 0; i < arraySize; i++){
        printf("%5d", arrayA[i]);
    }
    printf("\n\n");

    // Deallocate all object memory
    for (unsigned i = 0; i < numLabels; i++){
        delete labelArray[i];
    }

}

int main(int argc, char* argv[]){
    int operation = (argc > 1) ? atoi(argv[1]) : 0;
    int numberOfThreads = (argc >2) ? atoi(argv[2]) : 4;
    int sizeOfArray = (argc > 3) ? atoi(argv[3]) : 10;

    // Checks validity of operation, numThreads, and sizeOfArray; if one is invalid, set all to default
    if((operation < 0 or operation > 2) or (sizeOfArray <= 0 or sizeOfArray > 24) or
        (numberOfThreads <= 0 or numberOfThreads > 12)){
        printf("Invalid argument(s).\
                \nUsage: ./testThreadedArrayOperations [operation] [numberOfThreads] [sizeOfArray]\n \
                operation = 0 (addition), 1 (multiplication), or 2 (exponentiation), defaults to 0 \
                1 <= numberOfThreads <= 12\n \
                1 <= sizeOfArray <= 24\
                \nUsing default parameters...\n");
        operation = 0;
        numberOfThreads = 4;
        sizeOfArray = 10;
    }

    Canvas c(0, -1, Canvas::getDisplayWidth(), 620, "Threaded Array Scalar Operations", BLACK);
    c.run(operationVisualization, operation, numberOfThreads, sizeOfArray);
}
