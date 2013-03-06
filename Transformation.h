#pragma once
#include "glm/glm.hpp"

#include <vector>

using namespace std;

class Transformation {
public:
	glm::mat4 m;
	glm::mat4 minvt;
	glm::mat4 minv;

	Transformation(){};
	Transformation(vector<glm::mat4>);
};