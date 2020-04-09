/*
* Drawable.h provides a base class from which to extend other drawable shapes.
*/

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <mutex>            // Needed for locking the attribute mutex for thread-safety
#include "Color.h"      // Needed for color type

namespace tsgl {

  /*! \class Drawable
  *  \brief A class for drawing onto a Canvas or CartesianCanvas.
  *  \details Drawable provides a base class for drawing a shape, text, or image to a Canvas or CartesianCanvas.
  *  \note Drawable is abstract and must be extended.
  */
  class Drawable {
  protected:
    std::mutex      attribMutex;  ///< Protects the attributes of the Drawable from being accessed while simultaneously being changed
    bool isTextured = false; ///< Whether the Drawable is a Textured object.
    float myRotationPointX, myRotationPointY, myCenterX, myCenterY;
    int renderLayer; // The depth index to control the drawing order of the shapes
  public:
    Drawable();

    /*!
    * \brief Destroys the Drawable.
    * \details Destructor for a Drawable.
    * \details Frees up memory that was allocated to a Drawable object.
    */
    virtual ~Drawable() { }

    /*!
     * \brief Actually draws the Drawable to the Canvas.
     * \details This method renders the drawable to the Canvas.
     *  - When you extend the Drawable class, you *MUST* provide a definition for this method.
     *  - The definition must follow this format:
     *    <code>
     *    glBufferData(drawingMode, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
     *    glDrawArrays(drawingMode, 0, numberOfVertices);
     *    </code>
     *  - Really bad things could potentially happen if you do not follow this format. These two statements *MUST* be
     *    in the draw() method of the subclass that extends the Drawable class.
     *  - You can add other statements in the subclass
     * \note Please refer to the class description for more information and warnings about overriding this method.
     */
    virtual void draw() = 0;  // Abstract method for actually drawing the shape

    bool getIsTextured();

    void setLayer(int n);

    int getLayer();
    
   /*!
    * \brief Virtual accessor that returns if Drawable is processed and ready to be drawn
    * \details This function returns true only if all vertices have been inserted into an array.
    */
    virtual bool isProcessed() { return true; }

    /*!
     * \brief Pure virtual mutator that changes the rotation of the Drawable
     * \details Rotates the Drawable clockwise by radians radians.
     * \param radians The number of radians to rotate the Drawable.
     */
    virtual void setRotation(float radians) = 0;

    virtual void setRotationPoint(float x, float y);

    virtual void setRotationPointX(float x);

    virtual void setRotationPointY(float y);

    /*!
    * \brief Accessor for the center x-coordinate of the Drawable.
    * \details Returns the value of the myCenterX private variable.
    */
    virtual float getCenterX() { return myCenterX; }

    /*!
    * \brief Accessor for the center y-coordinate of the Drawable.
    * \details Returns the value of the myCenterY private variable.
    */
    virtual float getCenterY() { return myCenterY; }

    /*!
    * \brief Accessor for the rotation x-coordinate of the Drawable.
    * \details Returns the value of the myRotationPointX private variable.
    */
    virtual float getRotationPointX() { return myRotationPointX; }

    /*!
    * \brief Accessor for the rotation y-coordinate of the Drawable.
    * \details Returns the value of the myRotationPointY private variable.
    */
    virtual float getRotationPointY() { return myRotationPointY; }
  };
};

#endif /* DRAWABLE_H_ */