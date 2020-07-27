#include "Person.h"

using namespace tsgl;

Person::Person(){
    // Initialize random seed and random
    srand( time(0) );
    myX = rand()%1016;
    myY = rand()%286;
    myCircleRadius = 5;
    myStatus = susceptible;
    numInfectedNearby = 0;
    numDaysInfected = 0;
    myColor = ColorFloat(1,1,0,1);      // yellow

    // Create visual representation
    myCircle = new Circle(myX, myY, 0.0, myCircleRadius, 0.0, 0.0, 0.0, myColor);
    // myInfectionCircle = new Circle(myX, myY, 0.0, 50, 0.0, 0.0, 0.0, ColorFloat(1,0.5,0,0));
    myInfectionCircle = new Square(myX, myY, 0.0, 70, 0.0, 0.0, 0.0, ColorFloat(1,0.5,0,0));
}

Person::Person(float x, float y, GLfloat radius, char status, bool showInfectedRadius){
    // myID = id;
    myX = x;
    myY = y;
    myCircleRadius = radius;
    myStatus = status;
    numInfectedNearby = 0;
    numDaysInfected = 0;
    hasInfectedRadius = showInfectedRadius;

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

    // Create visual representation
    myCircle = new Circle(myX, myY, 0.0, myCircleRadius, 0.0, 0.0, 0.0, myColor);
    if(showInfectedRadius){
        ColorFloat infectedRadiusColor = ColorFloat(0,0,0,0);
        if(myStatus == infected){
            infectedRadiusColor = ColorFloat(1,0.5,0,0.5);
        }
        myInfectionCircle = new Square(myX, myY, 0.0, 70, 0.0, 0.0, 0.0, infectedRadiusColor);
        myInfectionCircle->setOutlineColor(ColorFloat(0,0,0,0));
    }
}

/**
 * \brief Adds the Person to the canvas.
 * \param can The Canvas on which the Person is to be drawn.
 */
void Person::draw(Canvas& can){
    if(myStatus == infected){
        can.add(myInfectionCircle);
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
// void Person::setCircleRadius(GLfloat radius){
//     myCircle->setRadius(radius);
// }

void Person::moveBy(float x, float y, float max_x, float max_y){
    // Check if move is valid (within the window)
    if((myX + x > -max_x) && (myX + x < max_x) && 
        (myY + y > -max_y) && (myY + y < max_y)) {
        // Move Person
        myCircle->changeXBy(x); myCircle->changeYBy(y);
        myInfectionCircle->changeXBy(x); myInfectionCircle->changeYBy(y);
        // Update info
        myX += x; myY += y;
    }
}

/**
 * \brief Sets the Person's Cubes to a new color.
 */
// void Person::updateColor(){
//     ColorFloat infectedRadiusColor = ColorFloat(0,0,0,0);
//     switch(myStatus){
//         case susceptible :
//             myColor = ColorFloat(1,1,0,1);      // yellow
//             break;
//         case infected :
//             myColor = ColorFloat(1,0,0,1);      // red
//             infectedRadiusColor = ColorFloat(1,0.5,0,0.5);  // orange
//             break;
//         case immune :
//             myColor = ColorFloat(0,1,0,1);      // green
//             break;
//         case dead :
//             myColor = ColorFloat(0.5,0.5,0.5,1);    // gray
//             break;
//         default:
//             myColor = ColorFloat(0,0,0,1);      // black
//     }
//     myCircle->setColor(myColor);
//     if(hasInfectedRadius){
//         myInfectionCircle->setColor(infectedRadiusColor);
//     }
// }

bool Person::checkIfInfectedNearby(std::vector<Person*> personVec, float infectedRadius){
    for(unsigned i = 0; i < personVec.size(); ++i){
        // Search for all people who are infected
        if(personVec[i]->getStatus() == infected){
            // Check if susceptible person is in infection radius
            if((myX > personVec[i]->getX() - infectedRadius) &&
                (myX < personVec[i]->getX() + infectedRadius) &&
                (myY > personVec[i]->getY() - infectedRadius) &&
                (myY < personVec[i]->getY() + infectedRadius))
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
        if(hasInfectedRadius){
            myInfectionCircle->setColor(ColorFloat(1,0.5,0,0.5));   // orange
            can.add(myInfectionCircle);
        }
        numInfectedNearby = 0;
        return true;
    }
    return false;
}

bool Person::determineIfDead(Canvas& can, int deadlinessFactor, int randNum){
    if(randNum <= deadlinessFactor){
        myStatus = dead;
        myCircle->setColor(ColorFloat(0.5,0.5,0.5,1));  // grey
        if(hasInfectedRadius){
            can.remove(myInfectionCircle);
        }
        numInfectedNearby = 0;
        return true;
    }
    return false;
}

void Person::recover(Canvas& can){
    myStatus = immune;
    myCircle->setColor(ColorFloat(0,1,0,1));
    if(hasInfectedRadius){
            can.remove(myInfectionCircle);
    }
}

/*!
 * \brief Destructor for Person.
 */
Person::~Person(){
    delete myCircle;
    if(hasInfectedRadius){
        delete myInfectionCircle;
    }
}