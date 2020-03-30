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
    glTranslatef(myCenterX, myCenterY, myCenterZ);
    glRotatef(myCurrentYaw, 0, 0, 1);
    glRotatef(myCurrentPitch, 0, 1, 0);
    glRotatef(myCurrentRoll, 1, 0, 0);
    // glTranslatef(myRotationPointX, myRotationPointY, myRotationPointZ);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(geometryType, 0, 24);

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
 * \brief Sets the Object3D to a new array of colors.
 * \param c The new array of ColorGLfloats.
 */
void Object3D::setColor(ColorGLfloat c[]) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
        colors[i*4] = c[i].R;
        colors[i*6 + 1] = c[i].G;
        colors[i*6 + 2] = c[i].B;
        colors[i*6 + 3] = c[i].A;
    }
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's x position
 * \param deltaX The difference between the new and old vertex x coordinates.
 */
void Object3D::changeXBy(float deltaX) {
    attribMutex.lock();
    // if (myCenterX == myRotationPointX) {
    //     myRotationPointX += deltaX;
    // }
    myCenterX += deltaX;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's y position
 * \param deltaY The difference between the new and old vertex y coordinates.
 */
void Object3D::changeYBy(float deltaY) {
    attribMutex.lock();
    // if (myCenterY == myRotationPointY) {
    //     myRotationPointY += deltaY;
    // }
    myCenterY += deltaY;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's z position
 * \param deltaZ The difference between the new and old vertex z coordinates.
 */
void Object3D::changeZBy(float deltaZ) {
    attribMutex.lock();
    // if (myCenterZ == myRotationPointZ) {
    //     myRotationPointZ += deltaZ;
    // }
    myCenterZ += deltaZ;
    attribMutex.unlock();
}

/**
 * \brief Alters the Object3D's vertex locations.
 * \param deltaX The difference between the new and old vertex x coordinates.
 * \param deltaY The difference between the new and old vertex y coordinates.
 * \param deltaZ The difference between the new and old vertex z coordinates.
 * \warning This will also alter the Object3D's rotation point if and only if the 
 *          old rotation point was at the Object3D's old center.
 */
void Object3D::changeCenterBy(float deltaX, float deltaY, float deltaZ) {
    attribMutex.lock();
    // if (myCenterX == myRotationPointX && myCenterY == myRotationPointY && myCenterZ == myRotationPointZ) {
    //     myRotationPointX += deltaX;
    //     myRotationPointY += deltaY;
    //     myRotationPointZ += deltaZ;
    // }
    myCenterX += deltaX;
    myCenterY += deltaY;
    myCenterZ += deltaZ;
    attribMutex.unlock();
}

/**
 * \brief Sets the Object3D's x position
 * \param x The new center x coordinate.
 */
void Object3D::setCenterX(float x) {
    attribMutex.lock();
    // if (myCenterX == myRotationPointX) {
    //     myRotationPointX = x;
    // }
    myCenterX = x;
    attribMutex.unlock();
}

/**
 * \brief Sets the Object3D's y position
 * \param y The new center y coordinate.
 */
void Object3D::setCenterY(float y) {
    attribMutex.lock();
    // if (myCenterY == myRotationPointY) {
    //     myRotationPointY = y;
    // }
    myCenterY = y;
    attribMutex.unlock();
}

/**
 * \brief Sets the Object3D's z position
 * \param z The new center z coordinate.
 */
void Object3D::setCenterZ(float z) {
    attribMutex.lock();
    // if (myCenterZ == myRotationPointZ) {
    //     myRotationPointZ = z;
    // }
    myCenterZ = z;
    attribMutex.unlock();
}

/**
 * \brief Moves the Object3D to new coordinates.
 * \param x The new center x coordinate.
 * \param y The new center y coordinate.
 * \param z The new center z coordinate.
 * \warning This will also alter the Object3D's rotation point if and only if the 
 *          old rotation point was at the Object3D's old center.
 */
void Object3D::setCenter(float x, float y, float z) {
    attribMutex.lock();
    // if (myCenterX == myRotationPointX && myCenterY == myRotationPointY && myCenterZ == myRotationPointZ) {
    //     myRotationPointX = x;
    //     myRotationPointY = y;
    //     myRotationPointZ = z;
    // }
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

void Object3D::setRotation(float radians) {

}

Object3D::~Object3D() {
    delete[] vertices;
}

}