#include "Pandemic.h"

#define RADIUS 5

using namespace tsgl;

Pandemic::Pandemic(Canvas& can, unsigned numPeople){
    // current day
    current_day = 0;
    // people counters
    number_of_people = numPeople;
    num_initially_infected = 0;
    // states counters
    num_infected = 0;
    num_susceptible = numPeople;
    num_immune = 0;
    num_dead = 0;
    
    // stats
    num_infections = 0;
    num_infection_attempts = 0;
    num_deaths = 0;
    num_recoveries = 0;

    // Generate random seed
    srand( time(0) );
    // // Set radius
    // float radius = 250/number_of_people;
    // Fill vectors
    for(unsigned i = 0; i < number_of_people; ++i){
        myPersons.push_back(new Person(rand()%(can.getWindowWidth())-(can.getWindowWidth())/2, 
                                        rand()%(can.getWindowHeight())-can.getWindowHeight()/2,
                                        RADIUS, susceptible)); // myPersons
        x_locations.push_back(myPersons[i]->getX());                        // x_locations
        y_locations.push_back(myPersons[i]->getY());                        // y_locations
        infected_x_locations.push_back(0);                                  // infected_x_locations
        infected_y_locations.push_back(0);                                  // infected_y_locations
        states.push_back(susceptible);                                      // states
        num_days_infected.push_back(0);                                     // num_days_infected
    }

}

void Pandemic::draw(Canvas& can){
    for(unsigned i = 0; i < number_of_people; ++i){
        myPersons[i]->draw(can);
    }
}

/*!
 * \brief Destructor for Pandemic.
 */
Pandemic::~Pandemic(){
    for(unsigned i = 0; i < number_of_people; ++i){
        delete myPersons[i];
    }
    myPersons.clear();
    x_locations.clear();
    y_locations.clear();
    infected_x_locations.clear();
    infected_y_locations.clear();
    states.clear();
    num_days_infected.clear();
}