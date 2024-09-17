#pragma once

#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum CAM_MOVEMENT
    {
        INVALID,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

public:
    Camera();
    ~Camera() = default;

    glm::mat4 GetMatrix();

    void Reset();

    // luam transformul si ii aplicam roatia si scalarea curenta
    void Translate(glm::vec3 translate);
    void Rotate(float dx, float dy);
    void Update();

    void processKeyInput(CAM_MOVEMENT dir, float dt);
    void processMouseInput();
    void processMouseWheelInput();

private:
    glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 0.3f);
    glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camZ = glm::normalize(camPosition - camTarget); //FRONT
    glm::vec3 camX = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camZ)); //RIGHT
    glm::vec3 camY = glm::cross(camZ, camX); // UP
    glm::vec3 WorldUp;

    glm::vec4 view;

    const float yaw = -90.0f;
    const float pitch = 0.0f;
    const float speed = 2.5f;
    const float sensitivity = 0.1f;
    const float zoom = 45.0f;

    static constexpr float travelSpeed = 12.0f;
    static constexpr float rotationSpeed = 0.004f;
};