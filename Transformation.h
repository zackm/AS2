#pragma once
#include "glm/glm.hpp"

#include <vector>

using namespace std;

class Transformation {
public:
	glm::mat4 m;
	glm::mat4 minvt;

	Transformation(){};
	Transformation(glm::mat4);
	Transformation(vector<glm::mat4>);
	void Transformation::left_mult(glm::mat4);
	void Transformation::right_mult(glm::mat4);
	void update_invt();
};