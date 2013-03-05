#pragma once
#include "glm/glm.hpp"

class Transformation {
public:
	glm::mat4 m;
	glm::mat4 minvt;

	Transformation(){};
	glm::mat4 operator*(Transformation);
};