#ifndef PERSON_H_
#define PERSON_H_

#include <cmath>
#include <tsgl.h>
#include <random>
#include "Circle.h"
#include "statusEnums.h"

using namespace tsgl;

class Person {
protected:
    // location
    float myX, myY;
    // radius
    GLfloat myCircleRadius;
    float myInfectionRadius;
    bool hasInfectionRadius;
    // TSGL shapes/objects
    Circle* myCircle;
    Circle* myInfectionCircle;
    ColorFloat myColor;
    // infection data
    char myStatus;
    int numInfectedNearby;
    int numDaysInfected;
    bool isToDie;
    int myNumDaysTillDead;
public:
    // Constructors //

    Person();

    Person(float x, float y, GLfloat radius, float infectionRadius, char status, bool hasInfectedRadius);

    // Draw //

    void draw(Canvas& can);

    // Accessors //

    GLfloat getCircleRadius() { return myCircleRadius; }

    float getInfectionRadius() { return myInfectionRadius; }
        
    Circle * getCircle() { return myCircle; }

    float getX() { return myX; }

    float getY() { return myY; }

    char getStatus() { return myStatus; }

    int getNumDaysInfected() { return numDaysInfected; }

    bool willDie() { return isToDie; }
    
    int getNumDaysTillDead() { return myNumDaysTillDead; }

    // Mutators //

    void setStatus(char status);

    void setColor(ColorFloat c);

    void moveBy(float x, float y, float max_x, float max_y);

    // Checking/updating functions //

    bool checkIfInfectedNearby(std::vector<Person*> personVec);

    void increaseNumDaysInfected() { numDaysInfected += 1; }

    bool determineIfInfected(Canvas& can, int contagiousFactor, int randNum);

    void determineIsToDie(int deadlinessFactor, int randNum, int daysTillDead);

    void die(Canvas& can);

    void recover(Canvas& can);

    virtual ~Person();
};

#endif /* PERSON_H_ */