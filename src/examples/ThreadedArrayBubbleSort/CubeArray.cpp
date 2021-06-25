#include "CubeArray.h"

using namespace tsgl;

// typedef Item int;

/*!
 * \brief Default constructor for CubeArray.
 * \details This is the default constructor for the CubeArray class.
 * \return A new CubeArray with a default size, values, and colors.
 */
CubeArray::CubeArray(){
    myX = 0.0;
    myY = 0.0;
    myZ = 0.0;
    mySize = 10;
    myCubeSideLength = 50.0;
    myYaw = 0.0;
    myPitch = 0.0;
    myRoll = 0.0;

    // Fill array of Cubes
    for(unsigned i = 0; i < mySize; ++i){
        myCubes.push_back(new Cube((myX-(int)(mySize-1)*(myCubeSideLength/2.0)) + (i * myCubeSideLength), myY, myZ, 
                                    myCubeSideLength, myYaw, myPitch, myRoll, RED));
        myCubes[i]->setRotationPoint(0,0,0);
    }
    // Copy dataArray into myData
    for(unsigned i = 0; i < mySize; ++i){
        myData.push_back(0);
        myText.push_back(new Text((myX-(int)(mySize-1)*(myCubeSideLength/2.0)) + (i * myCubeSideLength), myY, myZ+myCubeSideLength/2.0, 
                                    std::to_wstring(myData[i]), "/usr/local/include/TSGL/assets/freefont/FreeSansBold.ttf", myCubeSideLength/2.25, 
                                    myYaw, myPitch, myRoll, WHITE));
        myText[i]->setRotationPoint(0,0,0);
    }
}

/*!
 * \brief Explicitly constructs a new CubeArray.
 * \details This is the constructor for the CubeArray class.
 *      \param x The x coordinate of the center of the CubeArray.
 *      \param y The y coordinate of the center of the CubeArray.
 *      \param z The z coordinate of the center of the CubeArray.
 *      \param sideLength The side length of the CubeArray's Cubes.
 *      \param size The number of Cubes in the CubeArray.
 *      \param dataArray The array of values that CubeArray copies.
 *      \param dataArraySize The size of the array passed to CubeArray.
 *      \param yaw The yaw of the CubeArray.
 *      \param pitch The pitch of the CubeArray.
 *      \param roll The roll of the CubeArray.
 *      \param c1 A ColorFloat for the color of the CubeArray's Cubes.
 *      \param c2 A ColorFloat for the color of the CubeArray's Text/numbers.
 * \return A new CubeArray with the specified size, values, and colors.
 */
CubeArray::CubeArray(float x, float y, float z, GLfloat sideLength, unsigned size, Item dataArray[], unsigned dataArraySize,
                        float yaw, float pitch, float roll, ColorFloat c1, ColorFloat c2){
    myX = x;
    myY = y;
    myZ = z;
    mySize = size;
    myCubeSideLength = sideLength;
    myYaw = yaw;
    myPitch = pitch;
    myRoll = roll;

    // Fill array of Cubes
    for(unsigned i = 0; i < size; ++i){
        myCubes.push_back(new Cube((x-(int)(size-1)*(sideLength/2.0)) + (i * sideLength), y, z, sideLength, yaw, pitch, roll, c1));
        myCubes[i]->setRotationPoint(0,0,0);
    }
    // Copy dataArray into myData
    //  if size of dataArray is less than CubeArray size, fill the remaining spaces with blanks
    //  if size of dataArray is greater than CubeArray size, only copy up to the last index of myData
    if(dataArraySize < size){
        for(unsigned i = 0; i < dataArraySize; ++i){
            myData.push_back(dataArray[i]);
            myText.push_back(new Text((x-(int)(size-1)*(sideLength/2.0)) + (i * sideLength), y, z+sideLength/2.0, std::to_wstring(myData[i]),
                                    "/usr/local/include/TSGL/assets/freefont/FreeSansBold.ttf", sideLength/2.25, yaw, pitch, roll, c2));
            myText[i]->setRotationPoint(0,0,0);
        }
        for(unsigned i = dataArraySize; i < size; ++i){
            myData.push_back(0);
            myText.push_back(new Text((x-(int)(size-1)*(sideLength/2.0)) + (i * sideLength), y, z+sideLength/2.0, L"",
                                    "/usr/local/include/TSGL/assets/freefont/FreeSansBold.ttf", sideLength/2.25, yaw, pitch, roll, c2));
            myText[i]->setRotationPoint(0,0,0);
        }
    }
    else{
        for(unsigned i = 0; i < size; ++i){
            myData.push_back(dataArray[i]);
            myText.push_back(new Text((x-(int)(size-1)*(sideLength/2.0)) + (i * sideLength), y, z+sideLength/2.0, std::to_wstring(myData[i]),
                                    "/usr/local/include/TSGL/assets/freefont/FreeSansBold.ttf", sideLength/2.25, yaw, pitch, roll, c2));
            myText[i]->setRotationPoint(0,0,0);
        }
    }
    //TODO: Simplify above code
}


/**
 * \brief Adds the CubeArray to the canvas.
 * \param can The Canvas on which the CubeArray is to be drawn.
 */
void CubeArray::draw(Canvas& can){
    for(unsigned i = 0; i < mySize; ++i){
        can.add(myCubes[i]);
        can.add(myText[i]);
    }
}

/**
 * \brief Updates the graphics/visual portion of the CubeArray to match 
 *          the vector contained by the CubeArray.
 * \note Should be called after every change in the array's data to update graphic Text
 */
void CubeArray::update(){
    for(unsigned i = 0; i < mySize; ++i){
        myText[i]->setText(std::to_wstring(myData[i]));
    }
}

/**
 * \brief Updates the graphics/visual portion of the CubeArray to match 
 *          the value at a specified index contained by the CubeArray.
 * \param index The index of the graphic Text to change.
 * \note Should be called after every change in the array's data to update graphic Text
 */
void CubeArray::update(unsigned index){
    myText[index]->setText(std::to_wstring(myData[index]));
}

/**
 * \brief Sets the side length of the CubeArray's Cubes to a new side length,
 *          resizes the text to retain the Cube/Text size proportion, and
 *          moves the Text so that the resized Cube does not overlap and hide it
 * \param length The new side length.
 */
void CubeArray::setCubeSideLength(GLfloat length){
    for(unsigned i = 0; i < mySize; ++i){
        myCubes[i]->setSideLength(length);
        myText[i]->setSize(length/2.5);
        myText[i]->setCenterZ(myCubes[i]->getSideLength()/2.0);
    }
}

/**
 * \brief Sets the CubeArray's Cubes to a new color.
 * \param color The new ColorFloat.
 */
void CubeArray::setColor(ColorFloat color){
    for (Cube * c : myCubes){
        c->setColor(color);
    }
}

/**
 * \brief Sets the CubeArray's Cubes to new colors.
 * \param c The new array of ColorFloats.
 * \param size The size of the array of ColorFloats
 */
void CubeArray::setColor(ColorFloat c[], unsigned size){
    for(unsigned i = 0; i < mySize; ++i){
        myCubes[i]->setColor(c[i%size]);
    }
}

/**
 * \brief Sets the CubeArray's Text/numbers to a new color.
 * \param color The new ColorFloat.
 */
void CubeArray::setTextColor(ColorFloat color){
    for(Text * t : myText){
        t->setColor(color);
    }
}

/**
 * \brief Sets the font of the CubeArray's Text/numbers to a new font.
 * \param filename The path and file name of the font.
 */
void CubeArray::setFont(std::string filename){
    for(Text * t : myText){
        t->setFont(filename);
    }
}

/**
 * \brief Sets the size of the CubeArray's Text/numbers to a new size.
 * \param size The new size.
 */
void CubeArray::setTextSize(float size){
    for(Text * t : myText){
        t->setSize(size);
    }
}

/**
 * \brief Changes the CubeArray's yaw by a given value.
 * \param yaw The yaw to add to the CubeArray's current yaw.
 */
void CubeArray::changeYawBy(GLfloat yaw){
    for(unsigned i = 0; i < mySize; ++i){
        myCubes[i]->changeYawBy(yaw);
        myText[i]->changeYawBy(yaw);
    }
}

/**
 * \brief Changes the CubeArray's pitch by a given value.
 * \param pitch The pitch to add to the CubeArray's current pitch.
 */
void CubeArray::changePitchBy(GLfloat pitch){
    for(unsigned i = 0; i < mySize; ++i){
        myCubes[i]->changePitchBy(pitch);
        myText[i]->changePitchBy(pitch);
    }
}

/**
 * \brief Changes the CubeArray's roll by a given value.
 * \param roll The roll to add to the CubeArray's current roll.
 */
void CubeArray::changeRollBy(GLfloat roll){
    for(unsigned i = 0; i < mySize; ++i){
        myCubes[i]->changeRollBy(roll);
        myText[i]->changeRollBy(roll);
    }
}

/**
 * \brief If the sizes are equal, adds the values of two CubeArrays and returns 
 *          the sums in a new CubeArray.
 *        If the sizes are not equal, returns a default-constructed CubeArray.
 * \param c2 The CubeArray to be added with the current one.
 */
CubeArray CubeArray::operator+(CubeArray& c2){
    if(mySize == c2.getSize()){
        int summedArray[mySize];
        for(unsigned i = 0; i < mySize; i++){
            summedArray[i] = myData[i] + c2[i];
            printf("%.2d\n", summedArray[i]);
        }
        return CubeArray(0, 0, 0, myCubeSideLength, mySize, summedArray, mySize, myYaw, myPitch, myRoll, RED, WHITE);
    }
    return CubeArray();
}

/*!
 * \brief Destructor for CubeArray.
 */
CubeArray::~CubeArray(){
    for(unsigned i = 0; i < mySize; ++i){
        delete myCubes[i];
        delete myText[i];
    }
    myCubes.clear();
    myText.clear();
}