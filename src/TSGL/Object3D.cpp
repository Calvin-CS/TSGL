#include "Object3D.h"

namespace tsgl {

/*!
 * \brief Constructs a new Object3D.
 * \details
 * - Usually <code>vertices</code> is filled with floating point values that represent the vertices of the Object3D to be drawn.
 * - You may define other items in the constructor that pertain to the attributes of the subclass that is extending Object3D.
 * - At a minimum, you *MUST* fill an array of floating point values that pertain to the vertices of the Object3D.
 * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Object3D.</b>
 * \note Refer to the Object3D class description for more details.
 */
Object3D::Object3D(float x, float y, float z, float yaw, float pitch, float roll) : Drawable() {
    isTextured = false;
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
 * \brief Draw the Object3D.
 * \details This function actually draws the Object3D to the Canvas.
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Object3D cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void Object3D::draw() {
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

        glDrawArrays(outlineGeometryType, outlineFirstIndex, numberOfOutlineVertices);
    }

    glPopMatrix();

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

 /*!
  * \brief Adds another vertex to a Object3D.
  * \details This function initializes the next vertex in the Object3D and adds it to a Object3D buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param z The z position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Object3D::addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorGLfloat &color) {
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
 * \brief Sets the Object3D to a new color.
 * \param c The new ColorGLfloat.
 */
void Object3D::setColor(ColorGLfloat c) {
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
 * \brief Sets the Object3D's outline/edges to a new color
 * \param c The new ColorGLfloat.
 */
void Object3D::setEdgeColor(ColorGLfloat c) {
    for (int i = 0; i < numberOfOutlineVertices; i++) {
        outlineArray[4*i] = c.R;
        outlineArray[4*i+1] = c.G;
        outlineArray[4*i+2] = c.B;
        outlineArray[4*i+3] = c.A;
    }
}

/**
 * \brief Alters the Object3D's x position
 * \param deltaX The difference between the new and old vertex x coordinates.
 * \warning This will also alter the Object3D's rotation point similarly if and 
 *          only if the old rotation point was at the Object3D's old center.
 */
void Object3D::changeXBy(float deltaX) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX += deltaX;
    }
    myCenterX += deltaX;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's y position
 * \param deltaY The difference between the new and old vertex y coordinates.
 * \warning This will also alter the Object3D's rotation point similarly if and 
 *          only if the old rotation point was at the Object3D's old center.
 */
void Object3D::changeYBy(float deltaY) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointY += deltaY;
    }
    myCenterY += deltaY;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's z position
 * \param deltaZ The difference between the new and old vertex z coordinates.
 * \warning This will also alter the Object3D's rotation point similarly if and 
 *          only if the old rotation point was at the Object3D's old center.
 */
void Object3D::changeZBy(float deltaZ) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointZ += deltaZ;
    }
    myCenterZ += deltaZ;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's vertex locations.
 * \param deltaX The difference between the new and old vertex x coordinates.
 * \param deltaY The difference between the new and old vertex y coordinates.
 * \param deltaZ The difference between the new and old vertex z coordinates.
 * \warning This will also alter the Object3D's rotation point similarly if and 
 *          only if the old rotation point was at the Object3D's old center.
 */
void Object3D::changeCenterBy(float deltaX, float deltaY, float deltaZ) {
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
 * \brief Sets the Object3D's x position
 * \param x The new center x coordinate.
 * \warning This will also alter the Object3D's rotation point similarly if and only 
 *           if the old rotation point was at the Object3D's old center.
 */
void Object3D::setCenterX(float x) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointX = x;
    }
    myCenterX = x;
    attribMutex.unlock();
}

/**
 * \brief Sets the Object3D's y position
 * \param y The new center y coordinate.
 * \warning This will also alter the Object3D's rotation point similarly if and only 
 *           if the old rotation point was at the Object3D's old center.
 */
void Object3D::setCenterY(float y) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointY = y;
    }
    myCenterY = y;
    attribMutex.unlock();
}

/**
 * \brief Sets the Object3D's z position
 * \param z The new center z coordinate.
 * \warning This will also alter the Object3D's rotation point similarly if and only 
 *           if the old rotation point was at the Object3D's old center.
 */
void Object3D::setCenterZ(float z) {
    attribMutex.lock();
    if (centerMatchesRotationPoint()) {
        myRotationPointZ = z;
    }
    myCenterZ = z;
    attribMutex.unlock();
}

/**
 * \brief Moves the Object3D to new coordinates.
 * \param x The new center x coordinate.
 * \param y The new center y coordinate.
 * \param z The new center z coordinate.
 * \warning This will also alter the Object3D's rotation point similarly if and only 
 *           if the old rotation point was at the Object3D's old center.
 */
void Object3D::setCenter(float x, float y, float z) {
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
 * \brief Mutator for the Object3D's yaw
 * \param yaw The new yaw value for Object3D.
 */
void Object3D::setYaw(float yaw) {
    attribMutex.lock();
    myCurrentYaw = yaw;
    attribMutex.unlock();
}

/**
 * \brief Mutator for the Object3D's pitch
 * \param pitch The new pitch value for Object3D.
 */
void Object3D::setPitch(float pitch) {
    attribMutex.lock();
    myCurrentPitch = pitch;
    attribMutex.unlock();
}

/**
 * \brief Mutator for the Object3D's roll
 * \param roll The new roll value for Object3D.
 */
void Object3D::setRoll(float roll) {
    attribMutex.lock();
    myCurrentRoll = roll;
    attribMutex.unlock();
}

/*!
 * \brief Mutator for the yaw, pitch, and roll of the Object3D.
 * \param yaw The new yaw value for Object3D.
 * \param pitch The new pitch value for Object3D.
 * \param roll The new roll value for Object3D.
 */
void Object3D::setYawPitchRoll(float yaw, float pitch, float roll) {
    attribMutex.lock();
    myCurrentYaw = yaw;
    myCurrentPitch = pitch;
    myCurrentRoll = roll;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's yaw by a specified amount.
 * \param deltaYaw The change in yaw value for Object3D.
 */
void Object3D::changeYawBy(float deltaYaw) {
    attribMutex.lock();
    myCurrentYaw += deltaYaw;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's pitch by a specified amount.
 * \param deltaPitch The change in pitch value for Object3D.
 */
void Object3D::changePitchBy(float deltaPitch) {
    attribMutex.lock();
    myCurrentPitch += deltaPitch;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's roll by a specified amount.
 * \param deltaRoll The change in roll value for Object3D.
 */
void Object3D::changeRollBy(float deltaRoll) {
    attribMutex.lock();
    myCurrentRoll += deltaRoll;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's yaw, pitch, and roll by a specified amount.
 * \param deltaYaw The change in yaw value for Object3D.
 * \param deltaPitch The change in pitch value for Object3D.
 * \param deltaRoll The change in roll value for Object3D.
 */
void Object3D::changeYawPitchRollBy(float deltaYaw, float deltaPitch, float deltaRoll) {
    attribMutex.lock();
    myCurrentYaw += deltaYaw;
    myCurrentPitch += deltaPitch;
    myCurrentRoll += deltaRoll;
    attribMutex.unlock();
}

/**
 * \brief Sets the point around which Object3D is rotated.
 * \param x The x coordinate of the new rotation point.
 * \param y The y coordinate of the new rotation point.
 * \param z The z coordinate of the new rotation point.
 */
void Object3D::setRotationPoint(float x, float y, float z) {
    attribMutex.lock();
    myRotationPointX = x;
    myRotationPointY = y;
    myRotationPointZ = z;
    attribMutex.unlock();
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Object3D's z value.
 * \details Alters myRotationPointZ;
 * \param z myRotationPointZ's new float value.
 */
void Object3D::setRotationPointZ(float z) {
    attribMutex.lock();
    myRotationPointZ = z;
    attribMutex.unlock();
}

/*!
 * \brief Accessor for the center x-coordinate of the Object3D.
 * \details Returns the value of the myCenterX private variable,
 *          rotated by yaw, pitch, and roll about myRotationPointX;
 */
float Object3D::getCenterX() {
    if (centerMatchesRotationPoint()) {
        return myCenterX;
    }
    // return myCenterX;
    return cos(myCurrentYaw * PI / 180) * cos(myCurrentPitch * PI / 180) * (myCenterX - myRotationPointX) + (cos(myCurrentYaw * PI / 180) * sin(myCurrentPitch * PI / 180) * sin(myCurrentRoll * PI / 180) - sin(myCurrentYaw * PI / 180) * cos(myCurrentRoll * PI / 180)) * (myCenterY - myRotationPointY) + (cos(myCurrentYaw * PI / 180) * sin(myCurrentPitch * PI / 180) * cos(myCurrentRoll * PI / 180) + sin(myCurrentYaw * PI / 180) * sin(myCurrentRoll * PI / 180)) * (myCenterZ - myRotationPointZ) + myRotationPointX;
}

/*!
 * \brief Accessor for the center z-coordinate of the Object3D.
 * \details Returns the value of the myCenterZ private variable.
 */
float Object3D::getCenterY() {
    if (centerMatchesRotationPoint()) {
        return myCenterY;
    }
    // return myCenterY;
    return sin(myCurrentYaw * PI / 180) * cos(myCurrentPitch * PI / 180) * (myCenterX - myRotationPointX) + (sin(myCurrentYaw * PI / 180) * sin(myCurrentPitch * PI / 180) * sin(myCurrentRoll * PI / 180) + cos(myCurrentYaw * PI / 180) * cos(myCurrentRoll * PI / 180)) * (myCenterY - myRotationPointY) + (sin(myCurrentYaw * PI / 180) * sin(myCurrentPitch * PI / 180) * cos(myCurrentRoll * PI / 180) - cos(myCurrentYaw * PI / 180) * sin(myCurrentRoll * PI / 180)) * (myCenterZ - myRotationPointZ)  + myRotationPointY;
}

/*!
 * \brief Accessor for the center z-coordinate of the Object3D.
 * \details Returns the value of the myCenterZ private variable.
 */
float Object3D::getCenterZ() {
    if (centerMatchesRotationPoint()) {
        return myCenterZ;
    }
    // return myCenterZ;
    return -sin(myCurrentPitch * PI / 180) * (myCenterX - myRotationPointX) + cos(myCurrentPitch * PI / 180) * sin(myCurrentRoll * PI / 180) * (myCenterY - myRotationPointY) + cos(myCurrentPitch * PI / 180) * cos(myCurrentRoll * PI / 180) * (myCenterZ - myRotationPointZ) + myRotationPointZ;
}

void Object3D::setRotation(float radians) {

}

Object3D::~Object3D() {
    delete[] vertices;
    delete[] colors;
    delete[] outlineArray;
}

}