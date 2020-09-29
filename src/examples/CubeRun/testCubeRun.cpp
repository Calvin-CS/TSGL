/*
 * testCubeRun.cpp
 *
 * Usage: ./testCubeRun
 */

#include <tsgl.h>
#include <cmath>
#include <ctime>
#define FONT "./assets/freefont/FreeSansBold.ttf"
#define zDelta 10
#define obRoll 45.0
#define OBJECT_Y -53.0
#define PLAYER_MOVE_DIST 15.0
#define numObstacles 20
#define SIDE_LENGTH 25
#define interObstacleDistance 600.0

using namespace tsgl;

class ThreadData{
private:
    unsigned threadID;
    Cube * myObstacle;
    float myXDelta;
    float myYDelta;
    float myInitialZ;

public:
    ThreadData(unsigned tid, Cube* obstacle, float x, float z){
        threadID = tid;
        myObstacle = obstacle;
        myXDelta = x;
        myInitialZ = z;
    }
    void moveObCloser(){
        myObstacle->changeXBy(myXDelta);
        myObstacle->changeZBy(zDelta); 
    }

    void resetLocation(){
        myObstacle->setCenterX(0.0);
        myObstacle->setCenterY(OBJECT_Y);
        myObstacle->setCenterZ(myInitialZ);
    }

    Cube * getObstacle(){ return myObstacle; }

    float getInitialZ(){ return myInitialZ; }
};

// Player cube
ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
            ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
            ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8) };
Cube * playerCube = new Cube(0.0, OBJECT_Y, 300, SIDE_LENGTH, 0.0, 0.0, 0.0, colors);


void increaseX() {
    playerCube->changeXBy(PLAYER_MOVE_DIST);
    // printf("X: %f\n", playerCube->getCenterX());
    if(playerCube->getCenterX() - SIDE_LENGTH/2.0 >= 135){
        playerCube->setCenterX(-135);
    }
}

void decreaseX() {
    playerCube->changeXBy(-PLAYER_MOVE_DIST);
    if(playerCube->getCenterX() + 12.5 <= -135){
        playerCube->setCenterX(135);
    }
}

float genXDelta(Cube * obstacle){
    float num = 0;
    if(obstacle->getCenterZ() > -4000.0){
        num = (rand() % 101) / 100.0 - 0.5;      // generate random number between -0.5 and 0.5
    }
    else if(obstacle->getCenterZ() <= -4000.0){
        num = (rand() % 31) / 100.0 - 0.15;     // generate random number between -0.15 and 0.15
    }
    return num;
}

// Main function to run animation and gameplay
void gameFunction(Canvas& can) {
    srand( time(0) );       // initialize random seed

    ColorFloat colorArray[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, GRAY, WHITE};

    // Initialize arrays to contain all obstacles and some obstacle data
    Cube * obstacleArray[numObstacles];
    float initialZArray[numObstacles];
    float xDeltaArray[numObstacles];

    // Fill arrays
    for(unsigned i = 0; i < numObstacles; i++){
        obstacleArray[i] = new Cube(0.0, OBJECT_Y, (i+1.0)*(-interObstacleDistance), 
                                    SIDE_LENGTH, 0.0, 0.0, 0.0, colorArray[i%8]);
        xDeltaArray[i] = genXDelta(obstacleArray[i]);
        initialZArray[i] = obstacleArray[i]->getCenterZ();
    }

    // Create text object
    unsigned scoreNum = 0;
    Text * scoreText = new Text(265, 265, 0.0, std::to_wstring(scoreNum), FONT, 25, 0.0, 0.0, 0.0, WHITE);

    // Add objects to canvas
    can.add(playerCube);
    for(unsigned i = 0; i < numObstacles; i++){
        can.add(obstacleArray[i]);
    }
    can.add(scoreText);


    // Initial key bindings
    can.bindToButton(TSGL_RIGHT, TSGL_PRESS, increaseX);
    can.bindToButton(TSGL_LEFT, TSGL_PRESS, decreaseX);

    while (can.isOpen()){
        can.sleep();
        #pragma omp parallel
        {
            unsigned tid = omp_get_thread_num();
            unsigned numThreads = omp_get_num_threads();

            #pragma omp for schedule(static)
            for(unsigned i = 0; i < numObstacles; i ++){
                ThreadData * td = new ThreadData(tid, obstacleArray[i], xDeltaArray[i], initialZArray[i]);

                td->moveObCloser();

                // Respawns obstacle at original starting point with a new xDelta
                if(td->getObstacle()->getCenterZ() >= td->getInitialZ() + (numObstacles * interObstacleDistance) + 500.0){
                    td->resetLocation();
                    xDeltaArray[i] = genXDelta(td->getObstacle());
                }
                // Lose conditions (if player cube collides with obstacle)
                if(playerCube->getCenterZ() <= td->getObstacle()->getCenterZ() + SIDE_LENGTH and
                    playerCube->getCenterZ() >= td->getObstacle()->getCenterZ() - SIDE_LENGTH and
                    playerCube->getCenterX() <= td->getObstacle()->getCenterX() + SIDE_LENGTH and
                    playerCube->getCenterX() >= td->getObstacle()->getCenterX() - SIDE_LENGTH){
                    can.close();
                }
            }
        }

        // Update score
        scoreNum++;
        scoreText->setText(std::to_wstring(scoreNum));
    }

    // Display final score
    printf("\n*********************** \
            \nFinal score: %d \
            \n***********************\n", scoreNum);

    // Delete all objects
    for(unsigned i = 0; i < numObstacles; i++){
        delete obstacleArray[i];
    }
    delete scoreText;
}

int main(int argc, char* argv[]) {
    Canvas c(-1, -1, 620, 620, "Cube Run", BLACK);
    c.run(gameFunction);
}