#pragma once
#include "glm/glm.hpp"

class Transformation {
public:
	glm::mat4 m;
	glm::mat4 minvt;

	Transformation(){};
};