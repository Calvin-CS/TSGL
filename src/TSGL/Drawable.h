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
    int renderLayer; // The depth index to control the drawing order of the shapes
  public:

    /*!
    * \brief Constructs a new Drawable.
    * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
    * \note Refer to the Drawable class description for more details.
    */
    Drawable() {
      attribMutex.lock();
      renderLayer = -1;
      attribMutex.unlock();
    }


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

    /*!
    * \brief Destroys the Drawable.
    * \details Destructor for a Drawable.
    * \details Frees up memory that was allocated to a Drawable object.
    */
    virtual ~Drawable() {
      // attribMutex.lock(); //TODO: decide if we need this. Is it necessary so the item isn't destroyed when another thread is using?
      // attribMutex.unlock();
    };

    /*!
    * \brief Accessor for <code>isTextured</code>.
    * \return Whether the drawable is a textured primitive or not.
    */
    bool getIsTextured() {
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
    void setLayer(int n) {
      attribMutex.lock();
      if (n>=0) { renderLayer = n; }
      attribMutex.unlock();
    }  //TODO: make this validate layer numbers and return an error if not ok

    /**
    * \brief Accessor for <code>renderLayer</code>.
    * \return The layer the drawable is set at.
    */
    int getLayer() {
      attribMutex.lock();
      int retVal = renderLayer;
      attribMutex.unlock();
      return retVal;
    }
    
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
  };
};

#endif /* DRAWABLE_H_ */