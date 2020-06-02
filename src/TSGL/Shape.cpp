#include "Shape.h"

namespace tsgl {

/*!
 * \brief Constructs a new Shape.
 * \details
 * - Usually <code>vertices</code> is filled with floating point values that represent the vertices of the shape to be drawn.
 * - You may define other items in the constructor that pertain to the attributes of the subclass that is extending Shape.
 * - At a minimum, you *MUST* fill an array of floating point values that pertain to the vertices of the shape.
 * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Shape.</b>
 * \note Refer to the Shape class description for more details.
 */
Shape::Shape(float x, float y, float z, float yaw, float pitch, float roll) : Drawable(x,y,z,yaw,pitch,roll) { }

/**
 * \brief Sets the Shape to a new color.
 * \param c The new ColorFloat.
 */
void Shape::setColor(ColorFloat c) {
    for(int i = 0; i < numberOfVertices; i++) {
        colors[i*4] = c.R;
        colors[i*4 + 1] = c.G;
        colors[i*4 + 2] = c.B;
        colors[i*4 + 3] = c.A;
    }
}

/**
 * \brief Sets the Shape to a new array of colors.
 * \param c The new array of ColorFloats.
 */
void Shape::setColor(ColorFloat c[]) {
    for(int i = 0; i < numberOfVertices; i++) {
        colors[i*4] = c[i].R;
        colors[i*4 + 1] = c[i].G;
        colors[i*4 + 2] = c[i].B;
        colors[i*4 + 3] = c[i].A;
    }
}

}