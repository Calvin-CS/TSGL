/*
 * testPandemic.cpp
 *
 * Usage: ./testPandemic [numPersons] [infectionRate]
            1 <= numPersons <= 200, defaults to 50
            0 <= infectionRate <= 100, defaults to 20
 */

#include <tsgl.h>
#include <cmath>
#include "Person.h"
#include "Pandemic.h"

// disease constants
#define INFECTION_RADIUS 10
#define NUM_TO_INFECT 10
// #define DURATION_OF_DISEASE
// #define contagiousness_factor;
// #define deadliness_factor;  // 
// time constants
#define NUM_DAYS 365         // total number of days for pandemic
#define MS_PER_DAY 200000;  // microseconds per day

using namespace tsgl;

void pandemicFunction(Canvas& can, unsigned numPersons, unsigned infectionRate) {
    // Generate random seed
    srand( time(0) );

    // Create Pandemic
    Pandemic * p = new Pandemic(can, numPersons, NUM_TO_INFECT, infectionRate);

    float sleepTime = 0.03125;      // initial number of seconds to sleep 0.03125

    // Key bindings to speed up/slow down animation
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&sleepTime](){
        sleepTime /= 2;
    });
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&sleepTime](){
        if(sleepTime < 1){
            sleepTime *= 2;
        }
    });

    p->draw(can);

    unsigned complete = 0;  // ensures simulation only runs once
    printf("%d\n", infectionRate);
    while (can.isOpen()) {
        if(complete == 0){
            for(unsigned currentDay = 0; currentDay < NUM_DAYS; ++currentDay){
                printf("********Day %d********\n", currentDay+1);
                p->updateStatuses();
                // p->findInfected();
                can.sleepFor(sleepTime);
                p->movePersons(can);
                can.sleepFor(sleepTime);
                p->checkForInfection();
            }
            complete = 1;
        }
    }

    // Output
    printf("\n***********************\
            \n* Statistics and data *\
            \n***********************\
            \n\
            \nInfection rate: %.2f\
            \n\
            \nSusceptible: %d\
            \nInfected: %d\
            \nImmune: %d\
            \nDead: %d\
            \n", infectionRate/100.0, p->getNumSusceptible(), p->getNumInfected(), p->getNumImmune(), p->getNumDead());

    // Deallocate all object memory
    delete p;

}

int main(int argc, char* argv[]){
    int numPersons = (argc > 1) ? atoi(argv[1]) : 50;
    int infectionRate = (argc > 2) ? atoi(argv[2]) : 100;

    // Checks validity of numPersons and infectionRate; if invalid, set to default
    if((numPersons <= 0 or numPersons > 200) || (infectionRate < 0 or infectionRate > 100)){
        printf("Invalid argument(s).\
                \nUsage: ./testPandemic [numPersons] [infectionRate]\n \
                1 <= numPersons <= 200\
                0 <= infectionRate <= 100 \
                \nUsing default parameters...\n");
        numPersons = 50;
        infectionRate = 20;
    }

    Canvas c(0, -1, 620, 620, "Pandemic Simulation");
    c.setBackgroundColor(BLACK);
    c.run(pandemicFunction, numPersons, infectionRate);
}
