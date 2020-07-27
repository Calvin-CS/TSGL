#ifndef PANDEMIC_H_
#define PANDEMIC_H_

#include <random>
#include <cmath>
#include <vector>
#include <cxxopts.hpp>
#include <omp.h>
#include <tsgl.h>
#include "Person.h"

using namespace tsgl;

class Pandemic {
protected:
    int myNumPersons;
    int myInfectionRadius;
    // time
    int myNumDays;
    int mySickDuration;
    // counters
    int my_num_initially_infected;
    int my_num_currently_infected;
    int my_num_susceptible;
    // pandemic factors
    int myContagiousFactor;
    int myDeadlinessFactor;
    // stats
    int my_total_num_infections;
    int my_total_num_infection_attempts;
    int my_total_num_deaths;
    int my_total_num_death_attempts;
    int my_total_num_recoveries;
    float my_true_contagiousness;
    float my_true_mortality;
    // window dimensions
    int myWidth, myHeight;
    float my_max_x, my_max_y;

    // People
    std::vector<Person*> myPeople;
    // Text
    Text * myDayText;
    std::vector<Text*> myDataText;
    // Random generator
    std::default_random_engine generator;

    // Canvas
    Canvas *myCan, *myCan2;
public:
    Pandemic(Canvas& can, int argc, char* argv[]);

    void draw();

    void printInitialInfo();

    void printFinalInfo();

    virtual ~Pandemic();
};

#endif /* Pandemic_H_ */