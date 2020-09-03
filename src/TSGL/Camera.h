/*
 * Camera.h provides a way to control the view matrix for a Canvas.
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>
#include "Util.h"           // Needed constants and has cmath for performing math operations

namespace tsgl {

/*! \class Camera
 *  \brief Defines a movable, rotatable camera-like object and its corresponding view matrix.
 *  \details Camera is used within TSGL::Canvas to define the view matrix.
 */
class Camera {
protected:
    // camera Attributes
    glm::vec3 myPosition;
    glm::vec3 myFront;
    glm::vec3 myUp;
    glm::vec3 myRight;
    glm::vec3 myWorldUp;
    float myYaw;
    float myPitch;

    std::mutex attribMutex;

    void updateCameraAngle();
public:
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 target);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float targetX, float targetY, float targetZ);

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using the LookAt Matrix
    glm::mat4 getViewMatrix();

    /* POSITION VECTOR MUTATORS */
    void changeXBy(float delta);

    void changeYBy(float delta);

    void changeZBy(float delta);

    void moveBackward(float delta);

    void moveForward(float delta);

    void moveDown(float delta);

    void moveUp(float delta);

    void moveLeft(float delta);

    void moveRight(float delta);

    void setPosition(glm::vec3 pos);

    void setPosition(float x, float y, float z);

    void setPositionX(float x);

    void setPositionY(float y);

    void setPositionZ(float z);

    /* TARGET VECTOR MUTATORS */
    void lookAt(float x, float y, float z);

    void changeYawBy(float delta);

    void changePitchBy(float delta);

    /* ACCESSORS */
    float getPitch();

    float getYaw();

    glm::vec3 getPosition();

    float getPositionX();

    float getPositionY();

    float getPositionZ();

    glm::vec3 getTarget();

    float getTargetX();

    float getTargetY();

    float getTargetZ();
};

}

#endif /* CAMERA_H_ */