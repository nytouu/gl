#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "util.h"

const float DEFAULT_SPEED = 3.50f;

class Camera
{
  public:
    Camera(glm::vec3 pos, glm::mat4 proj)
    {
        position = pos;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);

        view = glm::lookAt(pos, pos + front, up);
        projection = proj;

        speed = DEFAULT_SPEED;

        sensitivity = 1.0f;
    }

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 position;

    glm::vec3 front, up, right;
    float speed;
    float sensitivity;

    float yaw, pitch;

    float lastX, lastY;

    void updateView()
    {
        view = glm::lookAt(position, position + front, up);
    }

    void setSpeed(float newValue)
    {
        speed = newValue;
    }

    void moveFromMouse(float xpos, float ypos)
    {
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; 
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw   += xoffset;
		pitch += yoffset;

		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        updateView();
    }
};

#endif
