/*
 * testPandemic.cpp
 *
 * Usage: ./testPandemic [numPersons] [contagiousFactor]
            1 <= numPersons <= 200, defaults to 50
            0 <= contagiousFactor <= 100, defaults to 50
 */

#include "Pandemic.h"

using namespace tsgl;

void pandemicFunction(Canvas& can, int argc, char* argv[]) {
    
    Pandemic p(can, argc, argv);

    p.draw();

    float sleepTime = 1;      // initial number of seconds to sleep 0.03125

    // Key bindings to speed up/slow down animation
    // can.bindToButton(TSGL_UP, TSGL_PRESS, [&sleepTime](){
    //     sleepTime /= 2;
    // });
    // can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&sleepTime](){
    //     if(sleepTime < 1){
    //         sleepTime *= 2;
    //     }
    // });

    int complete = 0;  // ensures simulation only runs once
    while (can.isOpen()) {
        // if(complete == 0){
        //     for(unsigned currentDay = 0; currentDay < numDays; ++currentDay){
        //         // Update day number
        //         dayText->setText(L"Day " + std::to_wstring(currentDay+1));

        //         #pragma omp parallel num_threads(numPersons)
        //         {
        //             int id = omp_get_thread_num();
                    
        //             if(personVec[id]->getStatus() != dead){
        //                 // Move people
        //                 personVec[id]->moveBy(move_distr(generator), move_distr(generator), max_x, max_y);    
        //                 can.sleepFor(sleepTime);
                    
        //                 // If person is infected
        //                 if(personVec[id]->getStatus() == infected){
        //                     ++total_num_death_attempts;
        //                     // Check if the person has died
        //                     if(personVec[id]->determineIfDead(can, deadlinessFactor, chance_distr(generator))){
        //                             --num_currently_infected;
        //                             ++total_num_deaths;
        //                             // printf("Person %d dead.\n", id);
        //                     }
        //                     // Check if person has been infected for the duration of the disease
        //                     else if(personVec[id]->getNumDaysInfected() >= sickDuration){
        //                         --num_currently_infected;
        //                         ++total_num_recoveries;
        //                         personVec[id]->recover(can);
        //                     }
        //                     // If not dead or recovered, increase number of days infected by 1
        //                     else{
        //                         personVec[id]->increaseNumDaysInfected();
        //                     }
        //                 }

        //                 // If person is susceptible
        //                 if(personVec[id]->getStatus() == susceptible){
        //                     // Check if the person is within an infected radius
        //                     if(personVec[id]->checkIfInfectedNearby(personVec, infectionRadius)){
        //                         ++total_num_infection_attempts;
        //                         // Determine if the person has been infected
        //                         if(personVec[id]->determineIfInfected(can, contagiousFactor, chance_distr(generator))){
        //                             --num_susceptible;
        //                             ++num_currently_infected;
        //                             ++total_num_infections;
        //                         }
        //                     }
        //                 }
        //             }
        //          } // end pragma
        //     }   // end the 'days' for loop
        //     complete = 1;
        // }
    }   // end main while loop

}

int main(int argc, char* argv[]){
    Canvas c(0, -1, 620, 620, "Pandemic Simulation");
    c.setBackgroundColor(BLACK);
    c.run(pandemicFunction, argc, argv);
}
