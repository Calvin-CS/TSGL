/*
 * Drawable.h provides a base class from which to extend other drawable shapes.
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

// #include <GL/glew.h>    // Needed for GL function calls
#include <glad/glad.h>      // New loader for GL function calls TODO: fix the path here
#include "Color.h"      // Needed for color type

namespace tsgl {

/*! \class Drawable
 *  \brief A class for drawing onto a Canvas or CartesianCanvas.
 *  \details Drawable provides a base class for drawing a shape, text, or image to a Canvas or CartesianCanvas.
 *  \note Drawable is abstract and must be extended.
 */
class Drawable {
 protected:
    bool isTextured; ///< Whether the shape is textured or not. If extending Drawable, <B> you *must* leave this at false (unless you are working with an image). </B>
    int renderLayer; ///< The depth index to control the drawing order of the shapes
 public:

    /*!
     * \brief Constructs a new Drawable.
     * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
     * \note Refer to the Drawable class description for more details.
     */
    Drawable() {
      isTextured = false;
      renderLayer = -1;   // -1 is the uninitialized layer value for the shape.
                          // If it is not set in the object before adding to the
                          // canvas, the canvas sets the layer value to the canvas'
                          // current drawing layer
    }

    /*!
     * \brief Destroys the Drawable.
     * \details Destructor for a Drawable.
     * \details Frees up memory that was allocated to a Drawable object.
     */
    virtual ~Drawable() {};

    /*!
     * \brief Accessor for <code>isTextured</code>.
     * \return Whether the drawable is a textured primitive or not.
     */
    bool getIsTextured() { return isTextured; }

    /**
     * \brief Sets the layer of the Drawable.
     *    \param n The new layer of the Drawable.
     * \details Sets <code>renderLayer</code> to n if n >= 0.
     */
    void setLayer(int n) { if (n>=0) renderLayer = n; }  //TODO: make this validate layer numbers and return an error if not ok

    /**
     * \brief Accessor for <code>renderLayer</code>.
     * \return The layer the drawable is set at.
     */
    int getLayer() { return renderLayer; }
};

}

#endif /* DRAWABLE_H_ */
