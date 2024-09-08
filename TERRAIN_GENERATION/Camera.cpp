#include "Camera.h"



glm::mat4 Camera::GetMatrix()
{
    return glm::lookAt(camPosition, 
        camPosition + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}


void Camera::processKeyInput(CAM_MOVEMENT dir, float dt)
{
    float speed = travelSpeed * dt;

    if (dir == FORWARD)
        camPosition += camZ * speed;
    if (dir == BACKWARD)
        camPosition -= camZ * speed;
    if (dir == LEFT)
        camPosition -= camX * speed;
    if (dir == RIGHT)
        camPosition += camX * speed;
    if (dir == UP)
        camPosition += camY * speed;
    if (dir == DOWN)
        camPosition -= camY * speed;

}