#include "Camera.h"


Camera::Camera()
{
    Update();
}

glm::mat4 Camera::GetMatrix()
{
    return glm::lookAt(camPosition, 
        camPosition + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::Translate(glm::vec3 translate)
{
    //translate = glm::rotate(translate, 45.0f, glm::vec3((0.0f, 0.0f, -1.0f));
}

void Camera::Update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camZ = glm::normalize(front);

    camX = glm::normalize(glm::cross(camZ, WorldUp));
    camY = glm::normalize(glm::cross(camX, camZ));
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

