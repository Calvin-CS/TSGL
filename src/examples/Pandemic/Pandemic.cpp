#include "Pandemic.h"

// person constants
#define PERSON_RADIUS 5         // the size of each person on screen
// text constants
#define FONT "./assets/freefont/FreeSansBold.ttf"   // font for all text
#define FONT_SIZE 20                                // font size for all text
#define TEXT_COLOR ColorFloat(0.2,1,1,1)            // color for all text (light blue)

using namespace tsgl;

// To parse command line for custom program options
cxxopts::ParseResult parse(int argc, char* argv[]) {
  try {
    cxxopts::Options options("Pandemic Simulation", "- command line options");

    options.positional_help("[optional args]").show_positional_help();

    options.allow_unrecognised_options().add_options()("help", "Print help")(
      "n, num", "the number of people in the model", cxxopts::value<int>()->default_value("50"))(
      "i, initial", "the number of initially infected people", cxxopts::value<int>()->default_value("5"))(
      "D, days", "the number of time days in the model", cxxopts::value<int>()->default_value("60"))(
      "T, sicktime", "the duration of the disease (in days)", cxxopts::value<int>()->default_value("5"))(
      "c, contagiousness" , "the contagiousness factor of the disease (0-100)", cxxopts::value<int>()->default_value("50"))(
      "r, radius", "the infection radius of the disease", cxxopts::value<int>()->default_value("35"))(
      "d, deadliness", "the deadliness factor of the disease (0-100)", cxxopts::value<int>()->default_value("2"))(
      "h, canvas-height", "TSGL canvas height", cxxopts::value<int>()->default_value("600"))(
      "w, canvas-width", "TSGL canvas width", cxxopts::value<int>()->default_value("600"));

    auto results = options.parse(argc, argv);

    if (results.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    return results;

  } catch (const cxxopts::OptionSpecException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}


Pandemic::Pandemic(Canvas& can, int argc, char* argv[]){
    // Parse command line
    auto result = parse(argc, argv);

    // Set options   TODO: add upper bounds on options, check options for validity
    if (result["num"].as<int>() > 0) {
        myNumPersons = result["num"].as<int>();
    }
    if (result["initial"].as<int>() > 0) {
        my_num_initially_infected = result["initial"].as<int>();
    }
    if (result["days"].as<int>() > 0) {
        myNumDays = result["days"].as<int>();
    }
    if (result["sicktime"].as<int>() > 0) {
        mySickDuration = result["sicktime"].as<int>();
    }
    if (result["contagiousness"].as<int>() >= 0) {
        myContagiousFactor = result["contagiousness"].as<int>();
    }
    if (result["radius"].as<int>() > 0) {
        myInfectionRadius = result["radius"].as<int>();
    }
    if (result["deadliness"].as<int>() >= 0) {
        myDeadlinessFactor = result["deadliness"].as<int>();
    }
    if (result["canvas-width"].as<int>() > 0) {
        myWidth = result["canvas-width"].as<int>();
    }
    if (result["canvas-height"].as<int>() > 0) {
        myHeight = result["canvas-height"].as<int>();
    }

    my_num_currently_infected = my_num_initially_infected;
    my_num_susceptible = myNumPersons - my_num_initially_infected;
    my_total_num_infections = my_num_initially_infected;
    my_total_num_infection_attempts = my_num_initially_infected;

    my_max_x = myWidth - PERSON_RADIUS;
    my_max_y = myHeight - PERSON_RADIUS - FONT_SIZE;


    // random number generator
    std::uniform_int_distribution<int> init_x_distr(-my_max_x, my_max_x);
    std::uniform_int_distribution<int> init_y_distr(-my_max_y, my_max_y);
    std::uniform_int_distribution<int> move_distr(-20, 20);
    std::uniform_int_distribution<int> chance_distr(1, 100);

    // Insert infected into array
    for(int i = 0; i < my_num_initially_infected; ++i){
        myPeople.push_back(new Person(init_x_distr(generator), init_y_distr(generator),
                                    PERSON_RADIUS, infected, true));
        myPeople[i]->draw(can);
    }
    // Insert susceptible into array
    for(int i = my_num_initially_infected; i < myNumPersons; ++i){
        myPeople.push_back(new Person(init_x_distr(generator), init_y_distr(generator),
                                    PERSON_RADIUS, susceptible, true));
        myPeople[i]->draw(can);
    }

    // Create text to display the current day
    Text * myDayText = new Text(0, 300, 0, L"Day 1", FONT, FONT_SIZE, 0, 0, 0, TEXT_COLOR);

    myCan = &can;
    // Create canvas for real-time data
    myCan2 = new Canvas(0, 0, myWidth, myHeight,"Pandemic Data");
    myCan2->start();
    myCan2->setBackgroundColor(BLACK);

}


/**
 * \brief Adds the Pandemic to the canvas.
 * \param can The Canvas on which the Pandemic is to be drawn.
 */
void Pandemic::draw(){
    myCan->add(myDayText);
    for(unsigned i = 0; i < myNumPersons; ++i){
        myPeople[i]->draw(*myCan);
    }
}

void Pandemic::printInitialInfo(){
    printf("\n***********************\
            \n* Initial information *\
            \n***********************\
            \n\
            \nChance of infection: %d%%\
            \nDaily chance of death: %d%%\
            \n\
            \nStarting number of people infected: %d\
            \nTotal number of people: %d\
            \n\n", myContagiousFactor, myDeadlinessFactor, my_num_initially_infected, myNumPersons);
}

void Pandemic::printFinalInfo(){
    // To avoid a divide-by-zero error (in case there were no infection attempts)
    if(my_total_num_infection_attempts != 0){
        my_true_contagiousness = (my_total_num_infections/static_cast<float>(my_total_num_infection_attempts)) * 100.0;
    }
    my_true_mortality = (my_total_num_deaths/static_cast<float>(my_total_num_death_attempts)) * 100.0;

    printf("\n***********************\
            \n* Statistics and data *\
            \n***********************\
            \n\
            \n*** Population after %d days ***\
            \nSusceptible: %d\
            \nInfected: %d\
            \nImmune: %d\
            \nDead: %d\
            \n", myNumDays, my_num_susceptible, my_num_currently_infected, 
                 my_total_num_recoveries, my_total_num_deaths);
    printf("\n*** Statistics ***\
            \nTotal number of infections: %d\
            \nTotal number of deaths: %d\
            \nTotal number of recoveries: %d\
            \nTrue contagiousness: %.2f%%\
            \nTrue mortality rate: %.2f%%\
            \n", my_total_num_infections, my_total_num_deaths, my_total_num_recoveries,
                    my_true_contagiousness, my_true_mortality);
}

/*!
 * \brief Destructor for Pandemic.
 */
Pandemic::~Pandemic(){
    for(unsigned i = 0; i < myPeople.size(); ++i){
        delete myPeople[i];
    }
    for(unsigned i = 0; i < myDataText.size(); ++i){
        delete myDataText[i];
    }
    delete myDayText;
    if (myCan2->isOpen())
        myCan2->stop();
    delete myCan2;
    myPeople.clear();
    myDataText.clear();
}