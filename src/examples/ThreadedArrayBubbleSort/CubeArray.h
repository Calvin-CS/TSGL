#ifndef CUBE_ARRAY_H_
#define CUBE_ARRAY_H_

#include <tsgl.h>
#include "Cube.h"
#include <vector>
#include <cstdio>

using namespace tsgl;

typedef int Item;

class CubeArray {
private:
    unsigned mySize;
protected:
    float myX, myY, myZ;
    GLfloat myCubeSideLength;
    std::vector<Cube*> myCubes;
    std::vector<int> myData;
    std::vector<Text*> myText;
    float myYaw, myPitch, myRoll;
public:
    CubeArray();

    CubeArray(float x, float y, float z, GLfloat sideLength, unsigned size, Item dataArray[], unsigned dataArraySize,
                 float yaw, float pitch, float roll, ColorFloat c1, ColorFloat c2);

    // CubeArray(float x, float y, float z, unsigned size, GLfloat sideLength, Item* dataArray, float yaw, float pitch, float roll, ColorFloat c[]);

    void draw(Canvas& can);

    void update();

    void update(unsigned index);

    // Accessors
    unsigned getSize() { return mySize; }
    
    GLfloat getCubeSideLength() { return myCubeSideLength; }
    
    Cube * getCube(unsigned i) { return myCubes[i]; }

    const int& operator[](unsigned i) const { return myData[i]; }
    

    // Mutators
    int& operator[](unsigned i) { return myData[i]; }

    void setCubeSideLength(GLfloat length);
    
    void setColor(ColorFloat c);
    
    void setColor(ColorFloat c[], unsigned size);
    
    void setTextColor(ColorFloat c);

    void setFont(std::string filename);

    void setTextSize(float size);


    void changeYawBy(GLfloat yaw);

    void changePitchBy(GLfloat pitch);

    void changeRollBy(GLfloat roll);

    void visualSplit(unsigned index);

    void visualRegroupAll(float x);

    // Operations
    CubeArray operator+ (CubeArray& c2);

    // bool operator==(CubeArray& a2);
    
    virtual ~CubeArray();
};

#endif /* CUBE_ARRAY_H_ */