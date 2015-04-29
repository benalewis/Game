#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Collision.h"

// WASD Enum
enum Camera_Movement {
    W,
	S,
	A,
	D
};

// Default camera values
const GLfloat YAW        = -90.0f; //Y Axis Roll
const GLfloat PITCH      =  0.0f; //X Axis Roll
const GLfloat SPEED      =  2.0f;
const GLfloat SENSITIVTY =  0.15f;

// Camera Class
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera Sensitivity
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;

    // Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVTY)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
  
    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    // Processes the input keys and works out movement speeds 
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == W)
            this->Position += this->Front * velocity;
        if (direction == S)
            this->Position -= this->Front * velocity;
        if (direction == A)
            this->Position -= this->Right * velocity;
        if (direction == D)
            this->Position += this->Right * velocity;
    }

    // Processes input from mouse
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw   += xoffset;
        this->Pitch += yoffset;

        // Can't look above head or below feet
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // Update yaw and pitch values
        this->updateCameraVectors();
    }

	
void ProcessCollision(glm::vec3 currentPosition)
	{
		if (currentPosition.x < -1.0f)
			this->Position.x = -1.0f;

		if (currentPosition.x > 1.0f)
			this->Position.x = 1.0f;

		if (currentPosition.y > 2.0f)
			this->Position.y = 2.0f;

		if (currentPosition.y < -0.5f)
			this->Position.y = -0.5f;

		if (currentPosition.z < -3.0f)
			this->Position.z = -3.0f;
	}


private:
    // Updates camera total
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); 
        this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
	}
};