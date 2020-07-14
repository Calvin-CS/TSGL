#ifndef PANDEMIC_H_
#define PANDEMIC_H_

#include <tsgl.h>
#include <vector>
#include "Person.h"
#include "statusEnums.h"

using namespace tsgl;

class Pandemic {
private:
    char* myName;
protected:
    // persons
    std::vector<Person*> myPersons;
    // current day
    unsigned current_day;
    // people counters
    unsigned number_of_people;
    unsigned num_initially_infected;
    // states counters
    unsigned num_infected;
    unsigned num_susceptible;
    unsigned num_immune;
    unsigned num_dead;
    // locations
    std::vector<int> x_locations;
    std::vector<int> y_locations;
    //infected people's locations
    std::vector<int> infected_x_locations;
    std::vector<int> infected_y_locations;
    // state
    // std::vector<char> states;
    // infected time
    // std::vector<unsigned> num_days_infected;

    // window dimensions
    float max_x;
    float max_y;
    
    // stats
    unsigned contagiousness_factor;
    unsigned num_infections;
    unsigned num_infection_attempts;
    unsigned num_deaths;
    unsigned num_recoveries;
public:
    Pandemic(Canvas& can, unsigned numPeople, unsigned numToInfect, unsigned infectionRate);

    void draw(Canvas& can);
    
    // Accessors
    Person * getPerson(unsigned i) { return myPersons[i]; }

    unsigned getCurrentDay() { return current_day; }

    unsigned getNumPeople() { return number_of_people; }

    unsigned getNumInitiallyInfected() { return num_initially_infected; }

    unsigned getNumInfected() { return num_infected; }

    unsigned getNumSusceptible() { return num_susceptible; }

    unsigned getNumImmune() { return num_immune; }

    unsigned getNumDead() { return num_dead; }

    int getXLocation(unsigned i) { return x_locations[i]; }

    int getYLocation(unsigned i) { return y_locations[i]; }

    int getInfectedXLocation(unsigned i) { return infected_x_locations[i]; }

    int getInfectedYLocation(unsigned i) { return infected_y_locations[i]; }

    unsigned getNumInfections() { return num_infections; }

    unsigned getNumInfectionAttempts() { return num_infection_attempts; }

    unsigned getNumDeaths() { return num_deaths; }

    unsigned getNumRecoveries() { return num_recoveries; }


    // Utility
    void updateStatuses();

    // void findInfected();

    void movePersons(Canvas& can);

    void checkForInfection();

    virtual ~Pandemic();
};

#endif /* PANDEMIC_H_ */