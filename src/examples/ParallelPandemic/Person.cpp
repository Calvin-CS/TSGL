#include "Person.h"

using namespace tsgl;

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


void Person::setStatus(char status){
    myStatus = status;
}

/**
 * \brief Sets the radius of the Person's Circle to a new radius.
 * \param radius The new radius.
 */
void Person::setColor(ColorFloat c){
    myCircle->setColor(c);
}

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

bool Person::checkIfInfectedNearby(std::vector<Person*> personVec){
    for(unsigned i = 0; i < personVec.size(); ++i){
        // Search for all people who are infected
        if(personVec[i]->getStatus() == infected){
            // Check if susceptible person is in infection radius
            unsigned distance = sqrt( pow(myX - personVec[i]->getX(), 2) + pow(myY - personVec[i]->getY(), 2) );
            // if((myX > personVec[i]->getX() - personVec[i]->getInfectionRadius()) &&
            //     (myX < personVec[i]->getX() + personVec[i]->getInfectionRadius()) &&
            //     (myY > personVec[i]->getY() - personVec[i]->getInfectionRadius()) &&
            //     (myY < personVec[i]->getY() + personVec[i]->getInfectionRadius()))
            if(distance < personVec[i]->getInfectionRadius() + myCircleRadius)
            {
                ++numInfectedNearby;
                return true;
            }
        }
    }
    return false;
}

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

void Person::determineIsToDie(int deadlinessFactor, int randNum, int daysTillDead){
    // Determine if person will die
    if(randNum <= deadlinessFactor){
        isToDie = true;
        // Determine number of days until person will die
        myNumDaysTillDead = daysTillDead;
    }
    //     myStatus = dead;
    //     myCircle->setColor(ColorFloat(0.5,0.5,0.5,1));  // grey
    //     if(hasInfectionRadius){
    //         myInfectionCircle->setColor(ColorFloat(0,0,0,0));   // make circle transparent to ensure it isn't visible
    //         myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
    //         can.remove(myInfectionCircle);
    //         // hasInfectionRadius = false;
    //     }
    //     numInfectedNearby = 0;
    //     return true;
    // }
    // return false;
}

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