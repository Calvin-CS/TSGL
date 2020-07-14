#include "Person.h"

using namespace tsgl;

Person::Person(float x, float y, GLfloat radius, char status){
    myX = x;
    myY = y;
    myCircleRadius = radius;
    myStatus = status;

    switch(myStatus){
        case susceptible :
            myColor = ColorFloat(1,1,0,1);
            break;
        case infected :
            myColor = ColorFloat(1,0,0,1);
            break;
        case immune :
            myColor = ColorFloat(0,1,0,1);
            break;
        case dead :
            myColor = ColorFloat(0.5,0.5,0.5,1);
            break;
        default:
            myColor = ColorFloat(0,0,0,1);
    }

    myCircle = new Circle(myX, myY, 0.0, myCircleRadius, 0.0, 0.0, 0.0, myColor);

}

/**
 * \brief Adds the Person to the canvas.
 * \param can The Canvas on which the Person is to be drawn.
 */
void Person::draw(Canvas& can){
    can.add(myCircle);
}

/**
 * \brief Sets the radius of the Person's Circle to a new radius.
 * \param radius The new radius.
 */
void Person::setCircleRadius(GLfloat radius){
    myCircle->setRadius(radius);
}

// /**
//  * \brief Sets the Person's Cubes to a new color.
//  * \param color The new ColorFloat.
//  */
// void Person::setColor(ColorFloat color){
//     for (Cube * c : myCubes){
//         c->setColor(color);
//     }
// }

// /**
//  * \brief Sets the Person's Cubes to new colors.
//  * \param c The new array of ColorFloats.
//  * \param size The size of the array of ColorFloats
//  */
// void Person::setColor(ColorFloat c[], unsigned size){
//     for(unsigned i = 0; i < mySize; ++i){
//         myCubes[i]->setColor(c[i%size]);
//     }
// }

// /**
//  * \brief Sets the Person's Text/numbers to a new color.
//  * \param color The new ColorFloat.
//  */
// void Person::setTextColor(ColorFloat color){
//     for(Text * t : myText){
//         t->setColor(color);
//     }
// }

// /**
//  * \brief Sets the font of the Person's Text/numbers to a new font.
//  * \param filename The path and file name of the font.
//  */
// void Person::setFont(std::string filename){
//     for(Text * t : myText){
//         t->setFont(filename);
//     }
// }

// /**
//  * \brief Sets the font size of the Person's Text/numbers to a new size.
//  * \param fontsize The new font size.
//  */
// void Person::setFontSize(unsigned int fontsize){
//     for(Text * t : myText){
//         t->setFontSize(fontsize);
//     }
// }


// void Person::changeYawBy(GLfloat yaw){
//     for(unsigned i = 0; i < mySize; ++i){
//         myCubes[i]->changeYawBy(yaw);
//         myText[i]->changeYawBy(yaw);
//     }
// }

// void Person::changePitchBy(GLfloat pitch){
//     for(unsigned i = 0; i < mySize; ++i){
//         myCubes[i]->changePitchBy(pitch);
//         myText[i]->changePitchBy(pitch);
//     }
// }

// void Person::changeRollBy(GLfloat roll){
//     for(unsigned i = 0; i < mySize; ++i){
//         myCubes[i]->changeRollBy(roll);
//         myText[i]->changeRollBy(roll);
//     }
// }

// void Person::visualSplit(unsigned index){
//     for(unsigned i = 0; i < index; ++i){
//         myCubes[i]->changeXBy(-myCubeSideLength/2.0);
//         myText[i]->changeXBy(-myCubeSideLength/2.0);
//     }
//     for(unsigned i = index; i < mySize; ++i){
//         myCubes[i]->changeXBy(myCubeSideLength/2.0);
//         myText[i]->changeXBy(myCubeSideLength/2.0);
//     }
// }

// // void Person::visualRegroup(unsigned index){

// // }

// void Person::visualRegroupAll(float x){
//     for(unsigned i = 0; i < mySize; i++){
//         myCubes[i]->setCenterX((x-(int)(mySize-1)*(myCubeSideLength/2.0)) + (i * myCubeSideLength));
//         myText[i]->setCenterX((x-(int)(mySize-1)*(myCubeSideLength/2.0)) + (i * myCubeSideLength));
//     }
// }


// /**
//  * \brief If the sizes are equal, adds the values of two Persons and returns 
//  *          the sums in a new Person.
//  *        If the sizes are not equal, returns a default-constructed Person.
//  * \param c2 The Person to be added with the current one.
//  */
// Person Person::operator+(Person& c2){
//     if(mySize == c2.getSize()){
//         int summedArray[mySize];
//         for(unsigned i = 0; i < mySize; i++){
//             summedArray[i] = myData[i] + c2[i];
//             printf("%.2d\n", summedArray[i]);
//         }
//         return Person(0, 0, 0, myCubeSideLength, mySize, summedArray, mySize, myYaw, myPitch, myRoll, RED, WHITE);
//     }
//     return Person();
// }

/*!
 * \brief Destructor for Table.
 */
Person::~Person(){
    delete myCircle;
}