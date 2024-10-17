#include "Camera.h"


Camera::Camera()
{
    Update();
}

void Camera::SetPosition(float x, float y, float z)
{
    camPosition = glm::vec3(x, y, z);
}

glm::mat4 Camera::GetMatrix()
{
    return glm::lookAt(camPosition, 
        camPosition + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}


glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 N = camTarget;
    glm::normalize(N);

    glm::vec3 U = glm::cross(camUp, camTarget);
    glm::normalize(U);

    glm::vec3 V = glm::cross(N, U);

    return glm::mat4x4( U.x, U.y, U.z, -camPosition.x,
                        V.x, V.y, V.z, -camPosition.y,
                        N.x, N.y, N.z, -camPosition.z,
                        0.0f, 0.0f, 0.0f, 1.0f );
}   

void Camera::Update()
{
    //glm::vec3 front;
    //front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //front.y = sin(glm::radians(pitch));
    //front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //camZ = glm::normalize(front);

    //camX = glm::normalize(glm::cross(camZ, WorldUp));
    //camY = glm::normalize(glm::cross(camX, camZ));
}

void Camera::processKeyInput(CAM_MOVEMENT dir, float dt)
{
    float speed = travelSpeed * dt;

    if (dir == FORWARD)
    {
        camPosition += camTarget * speed;
    }
    if (dir == BACKWARD)
    {
        camPosition -= camTarget * speed;
    }
    if (dir == LEFT)
    {
        glm::vec3 left = glm::cross(camTarget, camUp);
        glm::normalize(left);
        camPosition += left * speed;
    }
    if (dir == RIGHT)
    {
        glm::vec3 right = glm::cross(camUp, camTarget);
        glm::normalize(right);
        camPosition += right * speed;
    }
    if (dir == UP)
    {
        camPosition += camY * speed;
    }
    if (dir == DOWN)
    {
        camPosition -= camY * speed;
    }

}

