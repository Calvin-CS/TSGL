#include "Person.h"

using namespace tsgl;

/*!
 * \brief Default constructor for Person.
 * \details This is the default constructor for the CubeArray class.
 * \return A new CubeArray with a default size, values, and colors.
 */
Person::Person(){
    // location
    myX = 0;
    myY = 0;
    // radius
    myCircleRadius = 5;
    myInfectionRadius = 35;
    hasInfectionRadius = false;
    // infection data
    myStatus = susceptible;
    numInfectedNearby = 0;
    numDaysInfected = 0;
    isToDie = false;
    myNumDaysTillDead = 0;
    
    // Create visual representation of person
    myCircle = new Circle(myX, myY, 0.0, myCircleRadius, 0.0, 0.0, 0.0, ColorFloat(1,1,0,1));   // yellow
    // if hasInfectionRadius is set to true, create visual representation of infection radius
    if(hasInfectionRadius){
        ColorFloat infectionRadiusColor = ColorFloat(0,0,0,0);
        if(myStatus == infected){
            infectionRadiusColor = ColorFloat(1,0.5,0,0.5);
        }
        myInfectionCircle = new Circle(myX, myY, 0.0, myInfectionRadius-myCircleRadius, // ensures that only a small part of
                                                                                              // a person needs to be in the visible
                                                                                              // radius to be at risk of infection
            0.0, 0.0, 0.0, infectionRadiusColor);
        myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
    }
}

/*!
 * \brief Explicitly constructs a new Person.
 * \details This is the constructor for the Person class.
 *      \param x The x coordinate of the center of the Person.
 *      \param y The y coordinate of the center of the Person.
 *      \param radius The radius of the circle visually representing the Person.
 *      \param infectionRadius The radius of the circle representing the Person's
 *              infection radius.
 *      \param status The health status of the Person.
 *      \param showInfectionRadius Determines whether or not the infection radius is shown
 *              around the Person.
 * \return A new Person with the specified coordinates, radius, and status.
 */
Person::Person(float x, float y, GLfloat radius, float infectionRadius, char status, bool showInfectionRadius){
    // location
    myX = x;
    myY = y;
    // radius
    myCircleRadius = radius;
    myInfectionRadius = infectionRadius;
    hasInfectionRadius = showInfectionRadius;
    // infection data
    myStatus = status;
    numInfectedNearby = 0;
    numDaysInfected = 0;
    isToDie = false;
    myNumDaysTillDead = 0;
    
    // Determine color
    switch(myStatus){
        case susceptible :
            myColor = ColorFloat(1,1,0,1);      // yellow
            break;
        case infected :
            myColor = ColorFloat(1,0,0,1);      // red
            break;
        case immune :
            myColor = ColorFloat(0,1,0,1);      // green
            break;
        case dead :
            myColor = ColorFloat(0.5,0.5,0.5,1);    // grey
            break;
        default:
            myColor = ColorFloat(0,0,0,1);      // black
    }

    // Create visual representation of person
    myCircle = new Circle(myX, myY, 0.0, myCircleRadius, 0.0, 0.0, 0.0, myColor);
    // if hasInfectionRadius is set to true, create visual representation of infection radius
    if(hasInfectionRadius){
        ColorFloat infectionRadiusColor = ColorFloat(0,0,0,0);
        if(myStatus == infected){
            infectionRadiusColor = ColorFloat(1,0.5,0,0.5);
        }
        myInfectionCircle = new Circle(myX, myY, 0.0, myInfectionRadius-myCircleRadius, // ensures that only a small part of
                                                                                              // a person needs to be in the visible
                                                                                              // radius to be at risk of infection
            0.0, 0.0, 0.0, infectionRadiusColor);
        myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
    }
}

/**
 * \brief Adds the Person to the canvas.
 * \param can The Canvas on which the Person is to be drawn.
 */
void Person::draw(Canvas& can){
    if(hasInfectionRadius){
        if(myStatus == infected){
            can.add(myInfectionCircle);
        }
    }
    can.add(myCircle);
}

/**
 * \brief Changes the status of a Person.
 * \param status The new status of the Person.
 */
void Person::setStatus(char status){
    myStatus = status;
}

/**
 * \brief Sets the color of the Person to a new color.
 * \param c The new color.
 */
void Person::setColor(ColorFloat c){
    myCircle->setColor(c);
}

/**
 * \brief Changes the Person's x and y within certain bounds to simulate movement.
 * \param x The value to add to the Person's current x-coordinate.
 * \param y The value to add to the Person's current y-coordinate.
 * \param max_x The upper bound of the x-coordinate.
 * \param max_y The upper bound of the y-coordinate.
 */
void Person::moveBy(float x, float y, float max_x, float max_y){
    // Check if move is valid (within the window)
    if((myX + x > -max_x) && (myX + x < max_x) && 
        (myY + y > -max_y) && (myY + y < max_y)) {
        // Move Person
        myCircle->changeXBy(x); myCircle->changeYBy(y);
        if(hasInfectionRadius){
            myInfectionCircle->changeXBy(x); myInfectionCircle->changeYBy(y);
        }
        // Update info
        myX += x; myY += y;
    }
}

/**
 * \brief Checks if the Person's x and y is within the infection radius of another Person.
 * \param personVec The vector containing all Person instances.
 * \return true if the Person is within another's infection radius, false otherwise.
 */
bool Person::checkIfInfectedNearby(std::vector<Person*> personVec){
    unsigned distance;
    for(unsigned i = 0; i < personVec.size(); ++i){
        // Search for all people who are infected
        if(personVec[i]->getStatus() == infected){
            // Check if susceptible person is in infection radius
            distance = sqrt( pow(myX - personVec[i]->getX(), 2) + pow(myY - personVec[i]->getY(), 2) );
            if(distance < personVec[i]->getInfectionRadius() + myCircleRadius)
            {
                ++numInfectedNearby;
                return true;
            }
        }
    }
    return false;
}

/**
 * \brief Changes the Person's status to infected if there is 1 or more infected Persons
 *          nearby and if the random number passed to the function is less than or equal to
 *          the contagious factor.
 * \param can The Canvas on which the infection circle is to be drawn (if the Person becomes infected).
 * \param contagiousFactor The probability of a Person getting infected.
 * \param randNum The random number that determines whether or not the Person becomes infected.
 * \return true if the Person has become infected, false otherwise.
 */
bool Person::determineIfInfected(Canvas& can, int contagiousFactor, int randNum){
    if(numInfectedNearby >= 1 && randNum <= contagiousFactor){
        myStatus = infected;
        myCircle->setColor(ColorFloat(1,0,0,1));    // red
        if(hasInfectionRadius){
            myInfectionCircle->setColor(ColorFloat(1,0.5,0,0.5));   // orange
            can.add(myInfectionCircle);
        }
        numInfectedNearby = 0;
        return true;
    }
    return false;
}

/**
 * \brief Determines whether or not a Person is to die from the infection.
 * \param deadlinessFactor The probability of a Person dying.
 * \param randNum The random number that determines whether or not the Person dies.
 * \param daysTillDead The pre-determined number of days until the Person dies.
 */
void Person::determineIsToDie(int deadlinessFactor, int randNum, int daysTillDead){
    // Determine if person will die
    if(randNum <= deadlinessFactor){
        isToDie = true;
        // Determine number of days until person will die
        myNumDaysTillDead = daysTillDead;
    }
}

/**
 * \brief "Kills" a Person; sets the Person's status to dead, removes the Person's infection circle, and
 *          changes the Person's color to gray.
 * \param can The Canvas on which the Person and the Person's infection circle are changed.
 */
void Person::die(Canvas& can){
    myStatus = dead;
    myCircle->setColor(ColorFloat(0.5,0.5,0.5,1));  // grey
    if(hasInfectionRadius){
        myInfectionCircle->setColor(ColorFloat(0,0,0,0));   // make circle transparent to ensure it isn't visible
        myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
        can.remove(myInfectionCircle);
        // hasInfectionRadius = false;
        numInfectedNearby = 0;
    }
}

/**
 * \brief "Recovers" a Person; sets the Person's status to immune, removes the Person's infection circle, and
 *          changes the Person's color to green.
 * \param can The Canvas on which the Person and the Person's infection circle are changed.
 */
void Person::recover(Canvas& can){
    myStatus = immune;
    myCircle->setColor(ColorFloat(0,1,0,1));    // green
    if(hasInfectionRadius){
        myInfectionCircle->setColor(ColorFloat(0,0,0,0));   // make circle transparent to ensure it isn't visible
        myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
        can.remove(myInfectionCircle);
        // hasInfectionRadius = false;
        numInfectedNearby = 0;
    }
}

/*!
 * \brief Destructor for Person.
 */
Person::~Person(){
    delete myCircle;
    if(hasInfectionRadius){
        delete myInfectionCircle;
    }
}