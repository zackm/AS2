
#include "glm/glm.hpp"

#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__
#pragma once
#include "Ray.h"
#endif

class Camera {
public:
	glm::vec3 position, direction, up; //Assuming up is the up direction for the camera.
	float fov;

	void generateRay(float[], Ray*, glm::vec3);
	Camera(){};
	Camera(glm::vec3,glm::vec3,glm::vec3,float fov);
	void cornerVectors(glm::vec3*,glm::vec3*,glm::vec3*,glm::vec3*, float,float);
};