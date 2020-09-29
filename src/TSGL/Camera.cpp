#include "Camera.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Camera.
  * \details Explicit constructor for a Camera object.
  *   \param position The Camera's location in 3D space, a glm::vec3.
  *   \param up A 3D vector indicating the up direction in world space.
  *   \param target The point at which the Camera will look.
  * \return A new Camera object, used to represent and return a corresponding view matrix.
  */
Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 target) {
    attribMutex.lock();
    myPosition = position;
    myWorldUp = up;
    myFront = glm::normalize(target - myPosition);
    myRight = glm::normalize(glm::cross(myFront, myWorldUp));
    myUp = glm::cross(myRight, myFront);
    myPitch = glm::degrees(asin(myFront.y));
    // a simple acos() doesn't really cut it... undefined in some places + wrongly signed elsewhere
    if (myFront.z == 0) {
        if (myFront.x > 0) {
            myYaw = 0;
        } else {
            myYaw = 180;
        }
    } else if (myFront.z > 0) {
        myYaw = glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    } else {
        myYaw = -glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    }
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Camera.
  * \details Explicit constructor for a Camera object.
  *   \param posX The Camera's positional x-coordinate in 3D space, a float.
  *   \param posY The Camera's positional y-coordinate in 3D space, a float.
  *   \param posZ The Camera's positional z-coordinate in 3D space, a float.
  *   \param upX The x-component of a 3D vector indicating the up direction in world space.
  *   \param upY The y-component of a 3D vector indicating the up direction in world space.
  *   \param upZ The z-component of a 3D vector indicating the up direction in world space.
  *   \param targetX The x-coordinate of the point at which the Camera will look.
  *   \param targetY The y-coordinate of the point at which the Camera will look.
  *   \param targetZ The z-coordinate of the point at which the Camera will look.
  * \return A new Camera object, used to represent and return a corresponding view matrix.
  */
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float targetX, float targetY, float targetZ) {
    attribMutex.lock();
    myPosition = glm::vec3(posX, posY, posZ);
    myWorldUp = glm::vec3(upX, upY, upZ);
    myFront = glm::normalize(glm::vec3(targetX, targetY, targetZ) - myPosition);
    myRight = glm::normalize(glm::cross(myFront, myWorldUp));
    myUp = glm::cross(myRight, myFront);
    myPitch = glm::degrees(asin(myFront.y));
    // a simple acos() doesn't really cut it... undefined in some places + wrongly signed elsewhere
    if (myFront.z == 0) {
        if (myFront.x > 0) {
            myYaw = 0;
        } else {
            myYaw = 180;
        }
    } else if (myFront.z > 0) {
        myYaw = glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    } else {
        myYaw = -glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    }
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Camera.
  * \details Explicit constructor for a Camera object.
  *   \param position The Camera's location in 3D space, a glm::vec3.
  *   \param up A 3D vector indicating the up direction in world space.
  *   \param yaw The camera's yaw rotation (horizontal) in degrees.
  *   \param pitch The camera's pitch rotation (vertical) in degrees.
  * \note With position.x and position.y equal to 0 and a positive position.z, a yaw of -90 degrees and pitch of 0 degrees will look at (0,0,0).
  * \note pitch is constrained between -89 and 89 degrees.
  * \return A new Camera object, used to represent and return a corresponding view matrix.
  */
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    attribMutex.lock();
    myPosition = position;
    myWorldUp = up;
    myYaw = yaw;
    if (pitch > 89.0f) {
        myPitch = 89.0f;
    } else if (pitch < -89.0f) {
        myPitch = -89.0f;
    } else {
        myPitch = pitch;
    }
    updateCameraAngle();
    attribMutex.unlock(); 
}

 /*!
  * \brief Explicitly constructs a new Camera.
  * \details Explicit constructor for a Camera object.
  *   \param posX The Camera's positional x-coordinate in 3D space, a float.
  *   \param posY The Camera's positional y-coordinate in 3D space, a float.
  *   \param posZ The Camera's positional z-coordinate in 3D space, a float.
  *   \param upX The x-component of a 3D vector indicating the up direction in world space.
  *   \param upY The y-component of a 3D vector indicating the up direction in world space.
  *   \param upZ The z-component of a 3D vector indicating the up direction in world space.
  *   \param yaw The camera's yaw rotation (horizontal) in degrees.
  *   \param pitch The camera's pitch rotation (vertical) in degrees.
  * \note With posX and posY equal to 0 and a positive posZ, a yaw of -90 degrees and pitch of 0 degrees will look at (0,0,0).
  * \note pitch is constrained between -89 and 89 degrees.
  * \return A new Camera object, used to represent and return a corresponding view matrix.
  */
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    attribMutex.lock();
    myPosition = glm::vec3(posX, posY, posZ);
    myWorldUp = glm::vec3(upX, upY, upZ);
    myYaw = yaw;
    if (pitch > 89.0f) {
        myPitch = 89.0f;
    } else if (pitch < -89.0f) {
        myPitch = -89.0f;
    } else {
        myPitch = pitch;
    }
    updateCameraAngle();
    attribMutex.unlock(); 
}

 /*!
  * \brief Accessor for the corresponding view matrix for the Camera.
  * \details Returns a glm::mat4 based on Camera's position and orientation.
  * \return A glm::mat4 calculated by glm::lookAt().
  */
glm::mat4 Camera::getViewMatrix() {
    attribMutex.lock();
    glm::mat4 v = glm::lookAt(myPosition, myPosition + myFront, myUp);
    attribMutex.unlock();
    return v;
}

 /*!
  * \brief Mutator for Camera's x-coordinate in world space.
  * \details Alters the x component of Camera's myPosition vector by delta.
  *   \param delta The difference between camera's new and old x-coordinates.
  */
void Camera::changeXBy(float delta) {
    attribMutex.lock();
    myPosition += glm::vec3(delta,0.0f,0.0f);
    attribMutex.unlock();
}

 /*!
  * \brief Mutator for Camera's y-coordinate in world space.
  * \details Alters the y component of Camera's myPosition vector by delta.
  *   \param delta The difference between camera's new and old y-coordinates.
  */
void Camera::changeYBy(float delta) {
    attribMutex.lock();
    myPosition += glm::vec3(0.0f,delta,0.0f);
    attribMutex.unlock();
}

 /*!
  * \brief Mutator for Camera's z-coordinate in world space.
  * \details Alters the z component of Camera's myPosition vector by delta.
  *   \param delta The difference between camera's new and old z-coordinates.
  */
void Camera::changeZBy(float delta) {
    attribMutex.lock();
    myPosition += glm::vec3(0.0f,0.0f,delta);
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera backward, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the opposite direction of its myFront vector.
  *   \param delta How far backwards to move the Camera, in world space.
  */
void Camera::moveBackward(float delta) {
    attribMutex.lock();
    myPosition -= myFront * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera forward, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the same direction as its myFront vector.
  *   \param delta How far forward to move the Camera, in world space.
  */
void Camera::moveForward(float delta) {
    attribMutex.lock();
    myPosition += myFront * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera downward, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the opposite direction of its myUp vector.
  *   \param delta How far downward to move the Camera, in world space.
  */
void Camera::moveDown(float delta) {
    attribMutex.lock();
    myPosition -= myUp * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera upward, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the same direction as its myUp vector.
  *   \param delta How far upward to move the Camera, in world space.
  */
void Camera::moveUp(float delta) {
    attribMutex.lock();
    myPosition += myUp * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera left, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the opposite direction of its myRight vector.
  *   \param delta How far left to move the Camera, in world space.
  */
void Camera::moveLeft(float delta) {
    attribMutex.lock();
    myPosition -= myRight * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Moves the Camera right, relative to its orientation, by delta.
  * \details Alters Camera's myPosition vector by delta, in the same direction as its myUp vector.
  *   \param delta How far right to move the Camera, in world space.
  */
void Camera::moveRight(float delta) {
    attribMutex.lock();
    myPosition += myRight * delta;
    attribMutex.unlock();
}

 /*!
  * \brief Alters the Camera's position in world space.
  * \details Sets the x, y, and z components of Camera's myPosition vector according to the parameters.
  *   \param pos 3D vector containing Camera's new x, y, and z positional coordinates.
  */
void Camera::setPosition(glm::vec3 pos) {
    attribMutex.lock();
    myPosition = pos;
    attribMutex.unlock();
}

 /*!
  * \brief Alters the Camera's position in world space.
  * \details Sets the x, y, and z components of Camera's myPosition vector according to the parameters.
  *   \param x The Camera's new x-coordinate in world space.
  *   \param y The Camera's new y-coordinate in world space.
  *   \param z The Camera's new z-coordinate in world space.
  */
void Camera::setPosition(float x, float y, float z) {
    attribMutex.lock();
    myPosition = glm::vec3(x,y,z);
    attribMutex.unlock();
}

 /*!
  * \brief Sets the Camera's x-coordinate in world space.
  * \details Sets the x component of Camera's myPosition vector equal to the parameter.
  *   \param x The Camera's new x-coordinate in world space.
  */
void Camera::setPositionX(float x) {
    attribMutex.lock();
    myPosition.x = x;
    attribMutex.unlock();
}

 /*!
  * \brief Sets the Camera's y-coordinate co in world space.
  * \details Sets the y component of Camera's myPosition vector equal to the parameter.
  *   \param y The Camera's new y-coordinate in world space.
  */
void Camera::setPositionY(float y) {
    attribMutex.lock();
    myPosition.y = y;
    attribMutex.unlock();
}

 /*!
  * \brief Sets the Camera's z-coordinate in world space.
  * \details Sets the z component of Camera's myPosition vector equal to the parameter.
  *   \param z The Camera's new z-coordinate in world space.
  */
void Camera::setPositionZ(float z) {
    attribMutex.lock();
    myPosition.z = z;
    attribMutex.unlock();
}

 /*!
  * \brief Alters the point at which the Camera looks in world space.
  * \details Sets Camera's myFront vector, myRight vector, myUp vector, myPitch, and myYaw based on the parameters.
  *   \param x The new x-coordinate at which Camera looks, in world space.
  *   \param y The new y-coordinate at which Camera looks, in world space.
  *   \param z The new z-coordinate at which Camera looks, in world space.
  */
void Camera::lookAt(float x, float y, float z) {
    attribMutex.lock();
    myFront = glm::normalize(glm::vec3(x,y,z) - myPosition);
    myRight = glm::normalize(glm::cross(myFront, myWorldUp));
    myUp = glm::cross(myRight, myFront);
    myPitch = glm::degrees(asin(myFront.y));
    if (myFront.z == 0) {
        if (myFront.x > 0) {
            myYaw = 0;
        } else {
            myYaw = 180;
        }
    } else if (myFront.z > 0) {
        myYaw = glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    } else {
        myYaw = -glm::degrees( acos(myFront.x / cos(glm::radians(myPitch))) );
    }
    attribMutex.unlock();
}

/*!
 * \brief Alters the Camera's yaw rotation.
 * \details Based on the original direction the Camera was set to face, the Camera is rotated according to the parameter.
 *  \param delta The amount by which to change the Camera's yaw, in degrees.
 */
void Camera::changeYawBy(float delta) {
    attribMutex.lock();
    myYaw += delta;
    updateCameraAngle();
    attribMutex.unlock();
}

/*!
 * \brief Alters the Camera's pitch rotation.
 * \details Based on the original direction the Camera was set to face, the Camera is rotated according to the parameter.
 *  \param delta The amount by which to change the Camera's pitch, in degrees.
 * \note pitch is constrained between -89 and 89 degrees.
 */
void Camera::changePitchBy(float delta) {
    attribMutex.lock();
    myPitch += delta;
    if (myPitch > 89.0f)
        myPitch = 89.0f;
    if (myPitch < -89.0f)
        myPitch = -89.0f;
    updateCameraAngle();
    attribMutex.unlock();
}

/*!
 * \brief Accessor for the Camera's pitch.
 * \details Returns the value of the myPitch instance variable
 */
float Camera::getPitch() {
    attribMutex.lock();
    float p = myPitch;
    attribMutex.unlock();
    return p;
}

/*!
 * \brief Accessor for the Camera's yaw.
 * \details Returns the value of the myYaw instance variable
 */
float Camera::getYaw() {
    attribMutex.lock();
    float y = myYaw;
    attribMutex.unlock();
    return y;
}

/*!
 * \brief Accessor for the Camera's position in world space.
 * \details Returns the value of the myPosition instance variable
 */
glm::vec3 Camera::getPosition() {
    attribMutex.lock();
    glm::vec3 p = myPosition;
    attribMutex.unlock();
    return p;  
}

/*!
 * \brief Accessor for the x-coordinate of the Camera's position in world space.
 * \details Returns the x component of the myPosition instance variable
 */
float Camera::getPositionX() {
    attribMutex.lock();
    float x = myPosition.x;
    attribMutex.unlock();
    return x;  
}

/*!
 * \brief Accessor for the y-coordinate of the Camera's position in world space.
 * \details Returns the y component of the myPosition instance variable
 */
float Camera::getPositionY() {
    attribMutex.lock();
    float y = myPosition.y;
    attribMutex.unlock();
    return y;  
}

/*!
 * \brief Accessor for the z-coordinate of the Camera's position in world space.
 * \details Returns the z component of the myPosition instance variable
 */
float Camera::getPositionZ() {
    attribMutex.lock();
    float z = myPosition.z;
    attribMutex.unlock();
    return z;  
}

/*!
 * \brief Accessor for the point at which Camera looks in world space.
 * \details Returns the sum of the myPosition and myFront instance variables
 */
glm::vec3 Camera::getTarget() {
    attribMutex.lock();
    glm::vec3 t = myPosition + myFront;
    attribMutex.unlock();
    return t;  
}

/*!
 * \brief Accessor for the x-coordinate of the point at which Camera looks in world space.
 * \details Returns the x component of the sum of the myPosition and myFront instance variables
 */
float Camera::getTargetX() {
    attribMutex.lock();
    glm::vec3 t = myPosition + myFront;
    attribMutex.unlock();
    return t.x;  
}

/*!
 * \brief Accessor for the y-coordinate of the point at which Camera looks in world space.
 * \details Returns the y component of the sum of the myPosition and myFront instance variables
 */
float Camera::getTargetY() {
    attribMutex.lock();
    glm::vec3 t = myPosition + myFront;
    attribMutex.unlock();
    return t.y;  
}

/*!
 * \brief Accessor for the z-coordinate of the point at which Camera looks in world space.
 * \details Returns the z component of the sum of the myPosition and myFront instance variables
 */
float Camera::getTargetZ() {
    attribMutex.lock();
    glm::vec3 t = myPosition + myFront;
    attribMutex.unlock();
    return t.z;  
}

/*!
 * \brief Protected helper method that recalculates Camera's myFront vector based on yaw and pitch.
 * \details Alters myFront, myRight, and myUp according to yaw and pitch.
 */
void Camera::updateCameraAngle() {
    glm::vec3 front;
    front.x = cos(glm::radians(myYaw)) * cos(glm::radians(myPitch));
    front.y = sin(glm::radians(myPitch));
    front.z = sin(glm::radians(myYaw)) * cos(glm::radians(myPitch));
    myFront = glm::normalize(front);
    myRight = glm::normalize(glm::cross(myFront, myWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    myUp    = glm::normalize(glm::cross(myRight, myFront));
}

}