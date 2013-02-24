#include "glm/glm.hpp"

#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__
#include "Ray.h"
#endif

class Camera {
public:
	void generateRay(float[], Ray*, glm::vec3);
	Camera(){};
};