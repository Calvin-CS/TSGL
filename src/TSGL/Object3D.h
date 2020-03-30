/*
 * Object3D.h provides a base class from which to extend other drawable Object3Ds.
 */

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include "Color.h"      // Needed for color type
#include "Drawable.h"

namespace tsgl {

/*! \class Object3D
 *  \brief A class for drawing 3D objects onto a Canvas or CartesianCanvas.
 *  \warning <b><i>Though extending this class must be allowed due to the way the code is set up, attempting to do so
 *  could potentially mess up the internal GL calls the library uses. Proceed with great caution.</i></b>
 *  \details Object3D provides a base class for drawing 3D objects to a Canvas or CartesianCanvas.
 *  \note Object3D is abstract, and must be extended by the user.
 *  \details <code>vertices</code> should be an array of floating point values in TSGL's vertex format.
 *  One face consists of 3 vertices per triangle that it is divided into, #FIXME 7? vertices per face.
 *  E.g., to draw a cube, you would need 6 faces, 2 triangles per face, 3 vertices per triangle, 36 vertices total.
 *  \details <code>numberofvertices</code> should be the actual integer number of vertices to be drawn (e.g., *36* for a cube).
 *  \details <code>geometryType</code> should be one of GL's primitive drawing modes.
 *  See https://www.opengl.org/sdk/docs/man2/xhtml/glBegin.xml for further information.
 *  \details Theoretically, you could potentially extend the Object3D class so that you can create another Object3D class that suits your needs.
 *  \details However, this is not recommended for normal use of the TSGL library.
 */
class Object3D : public Drawable {
 protected:
    int numberOfVertices;
    GLfloat* vertices;
    GLfloat* colors;
    int currentVertex = 0;
    int currentColor = 0;
    float myCurrentYaw, myCurrentPitch, myCurrentRoll;
    float myCenterZ;                                    // myCenterX and myCenterY inherited
    float myRotationPointZ;                             // myRotationPointX and myRotationPointY inherited
    GLenum geometryType;
    bool init = false;
 public:
    Object3D(float yaw, float pitch, float roll, float x, float y, float z);

    virtual ~Object3D();

    virtual void draw();

    virtual void addVertex(float x, float y, float z, const ColorGLfloat &color = ColorGLfloat(1,1,1,1));

    virtual void setColor(ColorGLfloat c);
    virtual void setColor(ColorGLfloat c[]);

    virtual void changeXBy(float deltaX);
    virtual void changeYBy(float deltaY);
    virtual void changeZBy(float deltaZ);
    virtual void changeCenterBy(float deltaX, float deltaY, float deltaZ);
    
    virtual void setCenterX(float x);
    virtual void setCenterY(float y);
    virtual void setCenterZ(float z);
    virtual void setCenter(float x, float y, float z);

    virtual void setYaw(float yaw);
    virtual void setPitch(float pitch);
    virtual void setRoll(float roll);
    virtual void setYawPitchRoll(float yaw, float pitch, float roll);

    virtual void changeYawBy(float deltaYaw);
    virtual void changePitchBy(float deltaPitch);
    virtual void changeRollBy(float deltaRoll);
    virtual void changeYawPitchRollBy(float deltaYaw, float deltaPitch, float deltaRoll);

    virtual void setRotationPoint(float x, float y, float z);

    /*!
    * \brief Accessor for the center z-coordinate of the Object3D.
    * \details Returns the value of the myCenterZ private variable.
    */
    virtual float getCenterZ() { return myCenterZ; } // getCenterX, getCenterY inherited

    /*!
    * \brief Accessor for the Yaw of the Object3D.
    * \details Returns the value of the myCurrentYaw private variable.
    */
    virtual float getYaw() { return myCurrentYaw; }

    /*!
    * \brief Accessor for the Pitch of the Object3D.
    * \details Returns the value of the myCurrentPitch private variable.
    */
    virtual float getPitch() { return myCurrentPitch; }

    /*!
    * \brief Accessor for the Roll of the Drawable.
    * \details Returns the value of the myCurrentRoll private variable.
    */
    virtual float getRoll() { return myCurrentRoll; }

   /*!
    * \brief Accessor for the rotation z-coordinate of the Drawable.
    * \details Returns the value of the myRotationPointZ private variable.
    */
    virtual float getRotationPointZ() { return myRotationPointZ; }

    virtual void setRotation(float radians);
    


   /*!
    * \brief Accessor that returns if Object3D is processed and ready to be drawn
    * \details This function returns true only if all vertices have been inserted into an array.
    */
    virtual bool isProcessed() { return init; }
};

}

#endif /* OBJECT3D_H_ */
