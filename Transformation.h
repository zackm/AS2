#pragma once
#include "glm/glm.hpp"

class Transformation {
public:
	glm::mat4 m;
	glm::mat4 minvt;

	Transformation(){};
	Transformation(glm::mat4);
	glm::mat4 left_mult(glm::mat4);
	void update_invt();
};