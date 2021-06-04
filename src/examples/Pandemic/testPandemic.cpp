/*
 * testPandemic.cpp
 *
 * Usage: ./testPandemic [numPersons] [contagiousFactor]
            1 <= numPersons <= 200, defaults to 50
            0 <= contagiousFactor <= 100, defaults to 50
 */

#include <tsgl.h>
#include <cmath>
#include <random>
#include <vector>
#include "Person.h"
#include <cxxopts.hpp>

// disease constants
#define PERSON_RADIUS 5         // the size of each person on screen
// text constants
#define FONT "/usr/include/TSGL/assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 20                                // font size for all text
#define TEXT_COLOR ColorFloat(0.2,1,1,1)            // color for all text (light blue)

using namespace tsgl;

// To parse command line for custom program options
cxxopts::ParseResult parse(int argc, char* argv[]) {
    try {
        cxxopts::Options options("./src/examples/ParallelPandemic/testPandemic", "- command line options");

        options.positional_help("[optional args]").show_positional_help();

        options.allow_unrecognised_options().add_options()
            ("h, help", "Print help")
            ("n, num", "the number of people in the model", cxxopts::value<int>()->default_value("100"))
            ("i, initial", "the number of initially infected people (1-n)", cxxopts::value<int>()->default_value("1"))
            ("R, person-radius", "the radius of each person (1-30)", cxxopts::value<float>()->default_value("5.0"))
            ("t, sick-for", "the duration of the disease (in days)", cxxopts::value<int>()->default_value("21"))
            ("c, contagiousness" , "the contagiousness factor of the disease (0-100)", cxxopts::value<int>()->default_value("50"))
            ("r, infect-radius", "the infection radius of the disease (1-200)", cxxopts::value<int>()->default_value("35"))
            ("m, mortality", "the mortality factor of the disease (0-100)", cxxopts::value<int>()->default_value("2"))
            ("s, show-radius", "include to display the infection radius around each person", 
                cxxopts::value<bool>()->default_value("false"))
            ("a, attendance", "include to print out the thread attendance (to see if all threads are running)",
                cxxopts::value<bool>()->default_value("false"));

        auto results = options.parse(argc, argv);

        if(results.count("help") || results.count("h")){
            std::cout << options.help({"", "Group"}) << std::endl;
            std::cout << "Use the up and down arrow keys to increase or decrease the animation speed, respectively." << std::endl;
            exit(0);
        }

        return results;

    } catch (const cxxopts::OptionSpecException& e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}

void pandemicFunction(Canvas& can, int argc, char* argv[]) {
    // INITIALIZE VARIABLES //
    int numPersons = 100;
    float personRadius = 5.0;
    float infectionRadius = 35;
    bool showInfectionRadius;
    bool takeAttendance;
    // time
    int numDays = 0;
    int sickDuration = 21;
    // counters
    int num_initially_infected = 1;
    int num_currently_infected;
    int num_susceptible;
    // pandemic factors
    int contagiousFactor = 50;
    int mortalityFactor = 2;
    // ending stats/counters
    int total_num_infections;
    int total_num_infection_attempts = 0;
    int total_num_deaths = 0;
    // int total_num_death_attempts = 0;
    int total_num_recoveries = 0;
    float true_contagiousness = 0.0;
    float true_mortality = 0.0;
    float avgInfectionLength;
    // Parse command line
    auto result = parse(argc, argv);

    // SET OPTIONS //
    if (result["num"].as<int>() > 0) {
        numPersons = result["num"].as<int>();
    }
    if (result["initial"].as<int>() > 0 and result["initial"].as<int>() <= numPersons) {
        num_initially_infected = result["initial"].as<int>();
    }
    if (result["person-radius"].as<float>() > 0.0 and result["person-radius"].as<float>() <= 30) {
        personRadius = result["person-radius"].as<float>();
    }
    if (result["sick-for"].as<int>() > 0) {
        sickDuration = result["sick-for"].as<int>();
    }
    if (result["contagiousness"].as<int>() >= 0 and result["contagiousness"].as<int>() <= 100) {
        contagiousFactor = result["contagiousness"].as<int>();
    }
    if (result["infect-radius"].as<int>() > 0 and result["infect-radius"].as<int>() <= 200) {
        infectionRadius = result["infect-radius"].as<int>();
    }
    if (result["mortality"].as<int>() >= 0 and result["mortality"].as<int>() <= 100) {
        mortalityFactor = result["mortality"].as<int>();
    }

    // boolean switches
    showInfectionRadius = result["show-radius"].as<bool>();
    takeAttendance = result["attendance"].as<bool>();

    // counters
    num_currently_infected = num_initially_infected;
    num_susceptible = numPersons - num_initially_infected;
    total_num_infections = num_initially_infected;
    total_num_infection_attempts = num_initially_infected;
    // movement bounds
    float max_x = can.getWindowWidth()/2 - personRadius;
    float max_y = can.getWindowHeight()/2 - personRadius - FONT_SIZE;

    // random number generator, seed, and distributions
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> init_x_distr(-max_x, max_x);
    std::uniform_int_distribution<int> init_y_distr(-max_y, max_y);
    std::uniform_int_distribution<int> move_distr(-20, 20);
    std::uniform_int_distribution<int> sick_distr(1, sickDuration);
    std::uniform_int_distribution<int> chance_distr(1, 100);

    // Create arrays
    std::vector<Person*> personVec;
    std::vector<int> threadAttendance;

    // Insert infected into array
    for(int i = 0; i < num_initially_infected; ++i){
        personVec.push_back(new Person(init_x_distr(generator), init_y_distr(generator),
                                    personRadius, infectionRadius, infected, showInfectionRadius));
        personVec[i]->draw(can);
    }
    // Determine if the initially infected will die
    #pragma omp parallel num_threads(num_initially_infected)
    {
        int id = omp_get_thread_num();
        personVec[id]->determineIsToDie(mortalityFactor, chance_distr(generator), sick_distr(generator));
    }

    // Insert susceptible into array
    for(int i = num_initially_infected; i < numPersons; ++i){
        personVec.push_back(new Person(init_x_distr(generator), init_y_distr(generator),
                                    personRadius, infectionRadius, susceptible, showInfectionRadius));
        personVec[i]->draw(can);
    }

    // Set up thread "attendance sheet"
    for(int i = 0; i < numPersons; ++i){
        threadAttendance.push_back(-1);
    }

    // Create background
    Background * bg = can.getBackground();

    // Create text to display the current day
    Text * dayText = new Text(0, 300, 0, L"Day 1", FONT, FONT_SIZE, 0, 0, 0, TEXT_COLOR);
    can.add(dayText);

    float sleepTime = 0.5;      // initial number of seconds to sleep

    // KEY BINDINGS // -> to speed up/slow down animation
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&sleepTime](){
        sleepTime /= 2;
    });
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&sleepTime](){
        if(sleepTime < 1){
            sleepTime *= 2;
        }
    });

    // OUTPUT INITIAL INFORMATION //
    printf("\n***********************\
            \n* Initial information *\
            \n***********************\
            \n\
            \nInfection radius: %.1f\
            \nChance of infection: %d%%\
            \nChance of death: %d%%\
            \n\
            \nStarting number of people infected: %d\
            \nTotal number of people: %d\
            \n\n", infectionRadius, contagiousFactor, mortalityFactor, 
                    num_initially_infected, numPersons);

    bool complete = false;  // ensures simulation only runs once
    bool printInfo = true; // ensures information is only printed once

    // RUN SIMULATION //
    while (can.isOpen()) {
        if(!complete){
            // Runs until there are no more infections
            while(num_currently_infected != 0){
                // Increment number of days for pandemic
                ++numDays;
                // Update day number
                dayText->setText(L"Day " + std::to_wstring(numDays));
                can.sleepFor(sleepTime);

                //------------------------------------- Parallel Block ------------------------------------------
                #pragma omp parallel num_threads(numPersons)
                {
                    int id = omp_get_thread_num();
                    
                    // Ensure there is one thread running per person (take attendance for threads)
                    if(numDays == 1){
                        threadAttendance[id] = id;
                    }

                    // No actions apply to people who are dead
                    if(personVec[id]->getStatus() != dead){
                        // Move people
                        personVec[id]->moveBy(move_distr(generator), move_distr(generator), max_x, max_y);
                        // #ifdef _WIN32
                       	// can.sleepFor(sleepTime);
						// #elif __CYGWIN__
                        // can.sleepFor(sleepTime);
                    
                        // INFECTED
                        if(personVec[id]->getStatus() == infected){
                            // DEATH: check if the person has died
                            if(personVec[id]->willDie() and 
                               personVec[id]->getNumDaysTillDead() == personVec[id]->getNumDaysInfected()){
                                #pragma omp atomic update
                                --num_currently_infected;
                                #pragma omp atomic update
                                ++total_num_deaths;
                                personVec[id]->die(can);
                            }
                            // RECOVERY: check if person has been infected for the duration of the disease
                            else if( !personVec[id]->willDie() and personVec[id]->getNumDaysInfected() >= sickDuration){
                                #pragma omp atomic update
                                --num_currently_infected;
                                #pragma omp atomic update
                                ++total_num_recoveries;
                                personVec[id]->recover(can);
                            }
                            // STILL INFECTED: if not dead or recovered, increase number of days infected by 1
                            else{
                                personVec[id]->increaseNumDaysInfected();
                            }
                        }

                        // SUSCEPTIBLE
                        if(personVec[id]->getStatus() == susceptible){
                            // Check if the person is within an infected radius
                            if(personVec[id]->checkIfInfectedNearby(personVec)){
                                #pragma omp atomic update
                                ++total_num_infection_attempts;
                                // Determine if the person has been infected
                                if(personVec[id]->determineIfInfected(can, contagiousFactor, chance_distr(generator))){
                                    #pragma omp atomic update
                                    --num_susceptible;
                                    #pragma omp atomic update
                                    ++num_currently_infected;
                                    #pragma omp atomic update
                                    ++total_num_infections;
                                    personVec[id]->determineIsToDie(mortalityFactor, chance_distr(generator), sick_distr(generator));
                                }
                            }
                        }
                    }
                } // end main parallel block
                //------------------------------------- Parallel Block End ---------------------------------------

            }   // end simulation while loop
            complete = true;
            if(printInfo){
                // CALCULATIONS //
                if(total_num_infection_attempts != 0){  // To avoid a divide-by-zero error (in case there were no infection attempts)
                    true_contagiousness = (total_num_infections/static_cast<float>(total_num_infection_attempts)) * 100.0;
                }
                if(total_num_infections != 0){  // To avoid a divide-by-zero error (in case there were no infection attempts)
                    true_mortality = (total_num_deaths/static_cast<float>(total_num_infections)) * 100.0;
                }
                for(unsigned i = 0; i < personVec.size(); ++i){
                    avgInfectionLength += personVec[i]->getNumDaysInfected();
                }
                avgInfectionLength /= total_num_infections;


                // OUTPUT //
                printf("\n***********************\
                        \n* Statistics and data *\
                        \n***********************\
                        \n\
                        \n*** Herd immunity achieved in %d days ***\
                        \nSusceptible: %d\
                        \nImmune: %d\
                        \nDead: %d\
                        \n", numDays, num_susceptible, 
                             total_num_recoveries, total_num_deaths);
                printf("\n*** Statistics ***\
                        \nTotal number of infections: %d\
                        \nTotal number of deaths: %d\
                        \nTotal number of recoveries: %d\
                        \nTrue contagiousness: %.2f%%\
                        \nTrue mortality rate: %.2f%%\
                        \n", total_num_infections, total_num_deaths, total_num_recoveries,
                                true_contagiousness, true_mortality);

                // Print thread attendance
                if(takeAttendance){
                    printf("\n*** Thread Attendance (-1 if absent) ***");
                    for(unsigned i = 0; i < threadAttendance.size(); ++i){
                        if(i%10 == 0){
                            printf("\n");
                        }
                        printf("%5d", threadAttendance[i]);
                    }
                    printf("\n");
                }

                printInfo = false;
            }   // end output

        }	// end "complete" if

        can.sleep();

    }   // end Canvas while loop

    // DEALLOCATION OF OBJECT MEMORY //
    for(int i = 0; i < numPersons; ++i){
        delete personVec[i];
    }
    delete dayText;
    personVec.clear();
    threadAttendance.clear();

}

int main(int argc, char* argv[]){
    Canvas c(0, -1, 620, 620, "Pandemic Simulation", BLACK);
    c.run(pandemicFunction, argc, argv);
}
