#include "Drawable.h"

namespace tsgl {

/*!
 * \brief Constructs a new Drawable.
 * \details
 * - Usually <code>vertices</code> is filled with floating point values that represent the vertices of the Drawable to be drawn.
 * - You may define other items in the constructor that pertain to the attributes of the subclass that is extending Drawable.
 * - At a minimum, you *MUST* fill an array of floating point values that pertain to the vertices of the Drawable.
 * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
 * \note Refer to the Drawable class description for more details.
 */
Drawable::Drawable(float x, float y, float z, float yaw, float pitch, float roll) {
    myCurrentYaw = yaw;
    myCurrentPitch = pitch;
    myCurrentRoll = roll;
    myCenterX = x;
    myCenterY = y;
    myCenterZ = z;
    myRotationPointX = myCenterX;
    myRotationPointY = myCenterY;
    myRotationPointZ = myCenterZ;
}

/*!
 * \brief Draw the Drawable.
 * \details This function actually draws the Drawable to the Canvas.
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Drawable cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void Drawable::draw() {
    glPushMatrix();
    glTranslatef(myRotationPointX, myRotationPointY, myRotationPointZ);
    glRotatef(myCurrentYaw, 0, 0, 1);
    glRotatef(myCurrentPitch, 0, 1, 0);
    glRotatef(myCurrentRoll, 1, 0, 0); 
    glTranslatef(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ);
    glScalef(myXScale, myYScale, myZScale);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);

    glDrawArrays(geometryType, 0, numberOfVertices);

    if (edgesOutlined) {
        glVertexPointer(3, GL_FLOAT, outlineStride*sizeof(GLfloat)*3, vertices);
        glColorPointer(4, GL_FLOAT, 0, outlineArray);

        glDrawArrays(outlineGeometryType, 0, numberOfOutlineVertices);
    }

    glPopMatrix();

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

 /*!
  * \brief Adds another vertex to a Drawable.
  * \details This function initializes the next vertex in the Drawable and adds it to a Drawable buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param z The z position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Drawable::addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorGLfloat &color) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    attribMutex.lock();
    vertices[currentVertex] = x;
    vertices[currentVertex + 1] = y;
    vertices[currentVertex + 2] = z;
    currentVertex += 3;
    colors[currentColor] = color.R;
    colors[currentColor+1] = color.G;
    colors[currentColor+2] = color.B;
    colors[currentColor+3] = color.A;
    currentColor += 4;
    attribMutex.unlock();
    if (currentVertex == numberOfVertices*3) {
        attribMutex.lock();
        outlineArray = new GLfloat[numberOfOutlineVertices*4];
        std::fill_n(outlineArray, numberOfOutlineVertices*4, 0.75);
        init = true;
        attribMutex.unlock();
    }
}

/////////////////////////////////////////////////
// MUTATORS
/////////////////////////////////////////////////


/**
 * \brief Sets the Drawable to a new color.
 * \param c The new ColorGLfloat.
 */
void Drawable::setColor(ColorGLfloat c) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
        colors[i*4] = c.R;
        colors[i*4 + 1] = c.G;
        colors[i*4 + 2] = c.B;
        colors[i*4 + 3] = c.A;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Drawable's outline/edges to a new color
 * \param c The new ColorGLfloat.
 */
void Drawable::setEdgeColor(ColorGLfloat c) {
    for (int i = 0; i < numberOfOutlineVertices; i++) {
        outlineArray[4*i] = c.R;
        outlineArray[4*i+1] = c.G;
        outlineArray[4*i+2] = c.B;
        outlineArray[4*i+3] = c.A;
    }
}

/**
 * \brief Alters the Drawable's x position
 * \param deltaX The difference between the new and old vertex x coordinates.
 * \warning This will also alter the Drawable's rotation point similarly if and 
 *          only if the old rotation point was at the Drawable's old center.
 */
void Drawable::changeXBy(float deltaX) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX += deltaX;
    }
    myCenterX += deltaX;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's y position
 * \param deltaY The difference between the new and old vertex y coordinates.
 * \warning This will also alter the Drawable's rotation point similarly if and 
 *          only if the old rotation point was at the Drawable's old center.
 */
void Drawable::changeYBy(float deltaY) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointY += deltaY;
    }
    myCenterY += deltaY;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's z position
 * \param deltaZ The difference between the new and old vertex z coordinates.
 * \warning This will also alter the Drawable's rotation point similarly if and 
 *          only if the old rotation point was at the Drawable's old center.
 */
void Drawable::changeZBy(float deltaZ) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointZ += deltaZ;
    }
    myCenterZ += deltaZ;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's vertex locations.
 * \param deltaX The difference between the new and old vertex x coordinates.
 * \param deltaY The difference between the new and old vertex y coordinates.
 * \param deltaZ The difference between the new and old vertex z coordinates.
 * \warning This will also alter the Drawable's rotation point similarly if and 
 *          only if the old rotation point was at the Drawable's old center.
 */
void Drawable::changeCenterBy(float deltaX, float deltaY, float deltaZ) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX += deltaX;
        myRotationPointY += deltaY;
        myRotationPointZ += deltaZ;
    }
    myCenterX += deltaX;
    myCenterY += deltaY;
    myCenterZ += deltaZ;
    attribMutex.unlock();
}

/**
 * \brief Sets the Drawable's x position
 * \param x The new center x coordinate.
 * \warning This will also alter the Drawable's rotation point similarly if and only 
 *           if the old rotation point was at the Drawable's old center.
 */
void Drawable::setCenterX(float x) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX = x;
    }
    myCenterX = x;
    attribMutex.unlock();
}

/**
 * \brief Sets the Drawable's y position
 * \param y The new center y coordinate.
 * \warning This will also alter the Drawable's rotation point similarly if and only 
 *           if the old rotation point was at the Drawable's old center.
 */
void Drawable::setCenterY(float y) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointY = y;
    }
    myCenterY = y;
    attribMutex.unlock();
}

/**
 * \brief Sets the Drawable's z position
 * \param z The new center z coordinate.
 * \warning This will also alter the Drawable's rotation point similarly if and only 
 *           if the old rotation point was at the Drawable's old center.
 */
void Drawable::setCenterZ(float z) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointZ = z;
    }
    myCenterZ = z;
    attribMutex.unlock();
}

/**
 * \brief Moves the Drawable to new coordinates.
 * \param x The new center x coordinate.
 * \param y The new center y coordinate.
 * \param z The new center z coordinate.
 * \warning This will also alter the Drawable's rotation point similarly if and only 
 *           if the old rotation point was at the Drawable's old center.
 */
void Drawable::setCenter(float x, float y, float z) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX = x;
        myRotationPointY = y;
        myRotationPointZ = z;
    }
    myCenterX = x;
    myCenterY = y;
    myCenterZ = z;
    attribMutex.unlock();
}

/**
 * \brief Mutator for the Drawable's yaw
 * \param yaw The new yaw value for Drawable.
 */
void Drawable::setYaw(float yaw) {
    attribMutex.lock();
    myCurrentYaw = yaw;
    attribMutex.unlock();
}

/**
 * \brief Mutator for the Drawable's pitch
 * \param pitch The new pitch value for Drawable.
 */
void Drawable::setPitch(float pitch) {
    attribMutex.lock();
    myCurrentPitch = pitch;
    attribMutex.unlock();
}

/**
 * \brief Mutator for the Drawable's roll
 * \param roll The new roll value for Drawable.
 */
void Drawable::setRoll(float roll) {
    attribMutex.lock();
    myCurrentRoll = roll;
    attribMutex.unlock();
}

/*!
 * \brief Mutator for the yaw, pitch, and roll of the Drawable.
 * \param yaw The new yaw value for Drawable.
 * \param pitch The new pitch value for Drawable.
 * \param roll The new roll value for Drawable.
 */
void Drawable::setYawPitchRoll(float yaw, float pitch, float roll) {
    attribMutex.lock();
    myCurrentYaw = yaw;
    myCurrentPitch = pitch;
    myCurrentRoll = roll;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's yaw by a specified amount.
 * \param deltaYaw The change in yaw value for Drawable.
 */
void Drawable::changeYawBy(float deltaYaw) {
    attribMutex.lock();
    myCurrentYaw += deltaYaw;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's pitch by a specified amount.
 * \param deltaPitch The change in pitch value for Drawable.
 */
void Drawable::changePitchBy(float deltaPitch) {
    attribMutex.lock();
    myCurrentPitch += deltaPitch;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's roll by a specified amount.
 * \param deltaRoll The change in roll value for Drawable.
 */
void Drawable::changeRollBy(float deltaRoll) {
    attribMutex.lock();
    myCurrentRoll += deltaRoll;
    attribMutex.unlock();
}

/**
 * \brief Alters the Drawable's yaw, pitch, and roll by a specified amount.
 * \param deltaYaw The change in yaw value for Drawable.
 * \param deltaPitch The change in pitch value for Drawable.
 * \param deltaRoll The change in roll value for Drawable.
 */
void Drawable::changeYawPitchRollBy(float deltaYaw, float deltaPitch, float deltaRoll) {
    attribMutex.lock();
    myCurrentYaw += deltaYaw;
    myCurrentPitch += deltaPitch;
    myCurrentRoll += deltaRoll;
    attribMutex.unlock();
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable's x value.
 * \details Alters myRotationPointX;
 * \param z myRotationPointX's new float value.
 */
void Drawable::setRotationPointX(float x) {
    attribMutex.lock();
    myRotationPointX = x;
    attribMutex.unlock();
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable's y value.
 * \details Alters myRotationPointY;
 * \param y myRotationPointY's new float value.
 */
void Drawable::setRotationPointY(float y) {
    attribMutex.lock();
    myRotationPointY = y;
    attribMutex.unlock();
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable's z value.
 * \details Alters myRotationPointZ;
 * \param z myRotationPointZ's new float value.
 */
void Drawable::setRotationPointZ(float z) {
    attribMutex.lock();
    myRotationPointZ = z;
    attribMutex.unlock();
}

/**
 * \brief Sets the point around which Drawable is rotated.
 * \param x The x coordinate of the new rotation point.
 * \param y The y coordinate of the new rotation point.
 * \param z The z coordinate of the new rotation point.
 */
void Drawable::setRotationPoint(float x, float y, float z) {
    attribMutex.lock();
    myRotationPointX = x;
    myRotationPointY = y;
    myRotationPointZ = z;
    attribMutex.unlock();
}

/*!
 * \brief Accessor for the center x-coordinate of the Drawable.
 * \details Returns the value of the myCenterX private variable, rotated by myCurrentYaw, myCurrentPitch, and myCurrentRoll about myRotationPointX;
 * \note See https://math.stackexchange.com/questions/2093314/rotation-matrix-of-rotation-around-a-point-other-than-the-origin
 * \note and http://planning.cs.uiuc.edu/node102.html for more more understanding.
 */
float Drawable::getCenterX() {
    if (centerMatchesRotationPoint()) {
        return myCenterX;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    return cosYaw * cosPitch * (myCenterX - myRotationPointX) + (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) * (myCenterY - myRotationPointY) + (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll) * (myCenterZ - myRotationPointZ) + myRotationPointX;
}

/*!
 * \brief Accessor for the center z-coordinate of the Drawable.
 * \details Returns the value of the myCenterY private variable, rotated by myCurrentYaw, myCurrentPitch, and myCurrentRoll about myRotationPointY;
 * \note See https://math.stackexchange.com/questions/2093314/rotation-matrix-of-rotation-around-a-point-other-than-the-origin
 * \note and http://planning.cs.uiuc.edu/node102.html for more more understanding.
 */
float Drawable::getCenterY() {
    if (centerMatchesRotationPoint()) {
        return myCenterY;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    return sinYaw * cosPitch * (myCenterX - myRotationPointX) + (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) * (myCenterY - myRotationPointY) + (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll) * (myCenterZ - myRotationPointZ)  + myRotationPointY;
}

/*!
 * \brief Accessor for the center z-coordinate of the Drawable.
 * \details Returns the value of the myCenterZ private variable, rotated by myCurrentYaw, myCurrentPitch, and myCurrentRoll about myRotationPointZ;
 * \note See https://math.stackexchange.com/questions/2093314/rotation-matrix-of-rotation-around-a-point-other-than-the-origin
 * \note and http://planning.cs.uiuc.edu/node102.html for more more understanding.
 */
float Drawable::getCenterZ() {
    if (centerMatchesRotationPoint()) {
        return myCenterZ;
    }
    float cosYaw = cos(myCurrentYaw * PI / 180);
    float sinYaw = sin(myCurrentYaw * PI / 180);
    float cosPitch = cos(myCurrentPitch * PI / 180);
    float sinPitch = sin(myCurrentPitch * PI / 180);
    float cosRoll = cos(myCurrentPitch * PI / 180);
    float sinRoll = sin(myCurrentRoll * PI / 180);
    return -sinPitch * (myCenterX - myRotationPointX) + cosPitch * sinRoll * (myCenterY - myRotationPointY) + cosPitch * cosRoll * (myCenterZ - myRotationPointZ) + myRotationPointZ;
}

Drawable::~Drawable() {
    delete[] vertices;
    delete[] colors;
    delete[] outlineArray;
}

}