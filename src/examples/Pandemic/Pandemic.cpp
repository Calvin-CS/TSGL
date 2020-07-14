#include "Pandemic.h"

#define RADIUS 5        // radius for the Person circles
#define infection_radius 30

using namespace tsgl;

// Initializes the pandemic
Pandemic::Pandemic(Canvas& can, unsigned numPeople, unsigned numToInfect, unsigned infectionRate){
    // current day
    current_day = 0;
    // people counters
    number_of_people = numPeople;
    num_initially_infected = numToInfect;
    // states counters
    num_infected = numToInfect;
    num_susceptible = numPeople - numToInfect;
    num_immune = 0;
    num_dead = 0;
    
    // stats
    contagiousness_factor = infectionRate;
    num_infections = numToInfect;
    num_infection_attempts = 0;
    num_deaths = 0;
    num_recoveries = 0;

    // window dimensions
    max_x = can.getWindowWidth()/2;
    max_y = can.getWindowHeight()/2;

    // Generate random seed
    srand( time(0) );
    // Fill vectors with infected
    for(unsigned i = 0; i < num_infected; ++i){
        myPersons.push_back(new Person(rand()%(can.getWindowWidth())-max_x, 
                                        rand()%(can.getWindowHeight())-max_y,
                                        RADIUS, infected)); // myPersons
        x_locations.push_back(myPersons[i]->getX());               // x_locations
        y_locations.push_back(myPersons[i]->getY());               // y_locations
        infected_x_locations.push_back(myPersons[i]->getX());      // infected_x_locations
        infected_y_locations.push_back(myPersons[i]->getY());      // infected_y_locations
        // states.push_back(infected);                                // states
        // num_days_infected.push_back(0);                            // num_days_infected
    }
    // Fill vectors with susceptible
    for(unsigned i = num_infected; i < number_of_people; ++i){
        myPersons.push_back(new Person(rand()%(can.getWindowWidth())-max_x, 
                                        rand()%(can.getWindowHeight())-max_y,
                                        RADIUS, susceptible)); // myPersons
        x_locations.push_back(myPersons[i]->getX());                        // x_locations
        y_locations.push_back(myPersons[i]->getY());                        // y_locations
        infected_x_locations.push_back(0);                                  // infected_x_locations
        infected_y_locations.push_back(0);                                  // infected_y_locations
        // states.push_back(susceptible);                                      // states
        // num_days_infected.push_back(0);                                     // num_days_infected
    }

}

void Pandemic::draw(Canvas& can){
    for(unsigned i = 0; i < number_of_people; ++i){
        myPersons[i]->draw(can);
    }
}

void Pandemic::updateStatuses(){
    for(unsigned i = 0; i < number_of_people; ++i){
        myPersons[i]->updateColor();
    }
}

// void Pandemic::findInfected(){
//     for(unsigned i = 0; i < number_of_people; ++i){
//         if(myPersons[i]->getStatus() == infected){
//             infected_x_locations[i] = myPersons[i]->getX();
//             infected_y_locations[i] = myPersons[i]->getY();
//         }
//     }
// }

void Pandemic::movePersons(Canvas& can){
    float x_move_direction, y_move_direction;
    for(unsigned i = 0; i < number_of_people; ++i){
        if(myPersons[i]->getStatus() != dead){
            // x_move_direction = ((rand()%3) - 1) * 10;
            // y_move_direction = ((rand()%3) - 1) * 10;
            x_move_direction = ((rand()%301)-150)/10;
            y_move_direction = ((rand()%301)-150)/10;

            // Check if move is valid (within the window)
            if((x_locations[i] + x_move_direction > -max_x) &&
                (x_locations[i] + x_move_direction < max_x) &&
                (y_locations[i] + y_move_direction > -max_y) &&
                (y_locations[i] + y_move_direction < max_y))
            {
                // Move Person
                myPersons[i]->changeXYBy(x_move_direction, y_move_direction);
                // Update locations
                x_locations[i] += x_move_direction;
                y_locations[i] += y_move_direction;
                // findInfected();
            }
        }
    }
}

void Pandemic::checkForInfection(){
    // For all people
    for(unsigned i = 0; i < number_of_people; ++i){
        if(myPersons[i]->getStatus() == susceptible){
            // For those that are susceptible, check if there are any infected nearby
            for(unsigned j = 0; j < number_of_people; ++j){
                if(myPersons[j]->getStatus() == infected){
                    // Check if susceptible person is in infection radius
                    // printf("X: %f  %f   Y: %f  %f\n", myPersons[i]->getX(), myPersons[j]->getX(), myPersons[i]->getY(), myPersons[j]->getY());
                    if((myPersons[i]->getX() > myPersons[j]->getX() - infection_radius) &&
                        (myPersons[i]->getX() < myPersons[j]->getX() + infection_radius) &&
                        (myPersons[i]->getY() > myPersons[j]->getY() - infection_radius) &&
                        (myPersons[i]->getY() < myPersons[j]->getY() + infection_radius))
                    {   // ISSUE IS WITH THE FOR LOOPS: SOMEHOW NOT EVERYTHING IS GETTING CHECKED, OR AT LEAST NOT GETTING CHECKED CORRECTLY
                        printf("AT RISK: PERSON %d\n", i);
                        ++num_infection_attempts;
                        // // Determine if person gets infected
                        // printf("RandNum: %d\n", randNum);
                        if(rand()%101 <= contagiousness_factor){
                            printf("PERSON %d INFECTED\n", i);
                            myPersons[i]->setStatus(infected);
                            myPersons[i]->updateColor();
                            ++num_infected;
                            // printf("num_infected increased to %d\n", num_infected);
                            --num_susceptible;
                            break;          // to break out of the j for loop
                        }
                    }
                }
            }
        }
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
    // states.clear();
    // num_days_infected.clear();
}