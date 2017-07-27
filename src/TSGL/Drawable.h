/*
* Drawable.h provides a base class from which to extend other drawable shapes.
*/

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

// #include <GL/glew.h>    // Needed for GL function calls
#include <glad/glad.h>      // New loader for GL function calls TODO: fix the path here
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
    bool isTextured = false; //TODO remove this
    bool discreteRender; /*! TODO Whether the shape is textured or not. If extending Drawable, <B> you *must* leave this at false (unless you are working with an image). </B> */
    int renderLayer; // The depth index to control the drawing order of the shapes
    float rotationDeg = 0.0;
    float rotationX = 0.0;
    float rotationY = 0.0;

    bool hasOutline = false; ///< Whether the shape has an outline. If not implementing methods to get vertices and other information for an outline, this <B>must</B> remain false.

  public:

    /*!
    * \brief Constructs a new Drawable.
    * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Drawable.</b>
    * \note Refer to the Drawable class description for more details.
    */
    Drawable() {
      attribMutex.lock();
      discreteRender = false;
      renderLayer = -1;   // -1 is the uninitialized layer value for the shape.
      // If it is not set in the object before adding to the
      // canvas, the canvas sets the layer value to the canvas'
      // current drawing layer
      attribMutex.unlock();
    }

    /*!
    * \brief Destroys the Drawable.
    * \details Destructor for a Drawable.
    * \details Frees up memory that was allocated to a Drawable object.
    */
    virtual ~Drawable() {
      attribMutex.lock(); //TODO: decide if we need this. Is it necessary so the item isn't destroyed when another thread is using?
      attribMutex.unlock();
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

    /*!
     * \brief Accessor for <code>discreteRender</code>.
     * \return Whether the Drawable should be discretely rendered.
     */
    bool getIsDiscreteRendered() {
      attribMutex.lock();
      bool retVal = discreteRender;
      attribMutex.unlock();
      return retVal;
    }

    /*!
     * \brief Accessor for <code>hasOutline</code>.
     * \return Whether the Drawable also has an outline.
     */
    bool getHasOutline() {
      attribMutex.lock();
      bool outline = hasOutline;
      attribMutex.unlock();
      return outline;
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

    /**
    * \brief Sets the rotation of the object
    *    \param deg The rotation, in degrees, of the Drawable
    *    \param x The x coordinate around which to rotate
    *    \param y The y coordinate around which to rotate
    */
    void setRotation(float deg, float x, float y) {
      attribMutex.lock();
      rotationDeg = deg;
      rotationX = x;
      rotationY = y;
      attribMutex.unlock();
    }

    // Returns true if the object has a set rotation, otherwise false
    // Used by the canvas to avoid instantiating a tuple every time to check if
    // an object should be rotated.
    bool shouldBeRotated() {
      return (rotationDeg!=0.0);
    }

    /**
    * \brief Accessor for the rotation details of the Drawable
    */
    void getRotation(float &deg, float &x, float &y) {
      attribMutex.lock();
      deg = rotationDeg;
      x = rotationX;
      y = rotationY;
      attribMutex.unlock();
    }
  };


  /*! \class DiscreteDrawable
  *  \brief Drawable extended for Drawables that are rendered from a render() method.
  */
  class DiscreteDrawable : public Drawable {
  public:
    virtual void render() = 0;
  };
};


#endif /* DRAWABLE_H_ */
