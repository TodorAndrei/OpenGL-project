//
//  Camera.cpp
//  Lab5
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "Camera.hpp"

namespace gps {
    
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget)
    {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, this->cameraUpDirection));
    }
    
    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraDirection , glm::vec3(0.0f, 1.0f, 0.0f));
    }

	glm::vec3 Camera::getCameraTarget()
	{
		return cameraTarget;
	}

	glm::vec3 Camera::getCameraPosition() {
		return cameraPosition;
	}
    
    void Camera::move(MOVE_DIRECTION direction, float speed)
    {
        switch (direction) {
            case MOVE_FORWARD:
                cameraPosition += cameraDirection * speed;
                cameraTarget += cameraDirection * speed;
                break;
                
            case MOVE_BACKWARD:
                cameraPosition -= cameraDirection * speed;
                cameraTarget -= cameraDirection * speed;
                break;
                
            case MOVE_RIGHT:
                cameraPosition += cameraRightDirection * speed;
                cameraTarget += cameraRightDirection * speed;
                break;
                
            case MOVE_LEFT:
                cameraPosition -= cameraRightDirection * speed;
                cameraTarget -= cameraRightDirection * speed;
                break;
			
            case MOVE_UP:
                cameraPosition += cameraUpDirection * speed;
                cameraTarget += cameraUpDirection * speed;
                break;

            case MOVE_DOWN:
                cameraPosition -= cameraUpDirection * speed;
                cameraTarget -= cameraUpDirection * speed;
                break;
        }
    }
    
    /*void Camera::rotate(float pitch, float yaw)
    {
		cameraDirection = glm::vec3(
			cos(pitch) * sin(yaw),
			sin(pitch),
			cos(pitch) * cos(yaw)
		);
		cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 1.0f)));

    }*/

    void Camera::rotate(float pitch, float yaw) {
        this->cameraTarget.x = this->cameraPosition.x + sin(yaw) * cos(pitch);
        this->cameraTarget.y = this->cameraPosition.y + sin(pitch);
        this->cameraTarget.z = this->cameraPosition.z - cos(yaw) * cos(pitch);
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, this->cameraUpDirection));
    }
   
}
