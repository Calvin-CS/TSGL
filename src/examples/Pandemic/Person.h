#ifndef PERSON_H_
#define PERSON_H_

#include <tsgl.h>
#include "Circle.h"
#include "statusEnums.h"

using namespace tsgl;

class Person {
protected:
    float myX, myY;
    GLfloat myCircleRadius;
    Circle* myCircle;
    // Circle* myCircle[1];
    ColorFloat myColor;

    // Pandemic data
    char myStatus;
    unsigned numInfectedNearby;
    unsigned numDaysInfected;
public:
    // Person();

    Person(float x, float y, GLfloat radius, char status);

    void draw(Canvas& can);


    // Accessors
    GLfloat getCircleRadius() { return myCircleRadius; }
        
    Circle * getCircle() { return myCircle; }

    float getX() { return myX; }

    float getY() { return myY; }

    char getStatus() { return myStatus; }

    unsigned getNumDaysInfected() { return numDaysInfected; }
    

    // // Mutators
    void setStatus(char status);

    void setCircleRadius(GLfloat radius);

    void changeXYBy(float x, float y);

    void updateColor();
    
    // void setColor(ColorFloat c);
    
    // void setColor(ColorFloat c[], unsigned size);
    
    // void setTextColor(ColorFloat c);

    // void setFont(std::string filename);

    // void setFontSize(unsigned int fontsize);


    // void changeYawBy(GLfloat yaw);

    // void changePitchBy(GLfloat pitch);

    // void changeRollBy(GLfloat roll);

    // void visualSplit(unsigned index);

    // void visualRegroupAll(float x);

    // // Operations
    // CubeArray operator+ (CubeArray& c2);

    // bool operator==(CubeArray& a2);
    
    virtual ~Person();
};

#endif /* PERSON_H_ */