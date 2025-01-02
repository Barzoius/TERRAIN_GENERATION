#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
    void processMouseInput(glm::vec2);
    void processMouseWheelInput();

    glm::mat4x4 GetViewMatrix();

    void SetPosition(float x, float y, float z);

    glm::vec3 GetPosition();

    const float zoom = 45.0f;

    float GetSensitivity();

    glm::vec3 U = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 N = glm::vec3(0.0f, 0.0f, 1.0f);

private:
    glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 camZ = glm::normalize(camPosition - camTarget); //FRONT
    glm::vec3 camX = glm::normalize(glm::cross(camUp, camZ)); //RIGHT
    glm::vec3 camY = glm::cross(camZ, camX); // UP
    glm::vec3 WorldUp;


    glm::vec2 lastMousePos = glm::vec2(0.0, 0.0);

    glm::mat4x4 ViewMatrix;

    float yaw = -90.0f;
    float pitch = 0.0f;
    const float speed = 2.5f;
    float sensitivity = 0.1f;
    //const float zoom = 45.0f;

    static constexpr float travelSpeed = 12.0f;
    static constexpr float rotationSpeed = 0.004f;
};