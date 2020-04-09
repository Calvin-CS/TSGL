#include "Drawable.h"

namespace tsgl {

/*!
 * \brief Constructs a new Drawable.
 * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
 * \note Refer to the Drawable class description for more details.
 */
Drawable::Drawable() {
    attribMutex.lock();
    renderLayer = -1;
    attribMutex.unlock();
}

/*!
 * \brief Accessor for <code>isTextured</code>.
 * \return Whether the drawable is a textured primitive or not.
 */
bool Drawable::getIsTextured() {
    attribMutex.lock();
    bool retVal = isTextured;
    attribMutex.unlock();
    return retVal;
}

/**
* \brief Sets the layer of the Drawable.
*    \param n The new layer of the Drawable.
* \details Sets <code>renderLayer</code> to n if n >= 0.
*/
void Drawable::setLayer(int n) {
    attribMutex.lock();
    if (n>=0) { renderLayer = n; }
    attribMutex.unlock();
}

/**
 * \brief Accessor for <code>renderLayer</code>.
 * \return The layer the drawable is set at.
 */
int Drawable::getLayer() {
    attribMutex.lock();
    int retVal = renderLayer;
    attribMutex.unlock();
    return retVal;
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable
 * \details Alters myRotationPointX and myRotationPointY;
 * \param x myRotationPointX's new float value.
 * \param y myRotationPointY's new float value.
 */
void Drawable::setRotationPoint(float x, float y) {
    myRotationPointX = x;
    myRotationPointY = y;
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable's x value.
 * \details Alters myRotationPointX;
 * \param x myRotationPointX's new float value.
 */
void Drawable::setRotationPointX(float x) {
    myRotationPointX = x;
}

/*!
 * \brief Virtual mutator that changes the rotation point of the Drawable's y value.
 * \details Alters myRotationPointY;
 * \param y myRotationPointY's new float value.
 */
void Drawable::setRotationPointY(float y) {
    myRotationPointY = y;
}

}