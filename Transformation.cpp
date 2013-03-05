#pragma once
#include "Transformation.h"

#pragma once
#include "glm/glm.hpp"

glm::mat4 Transformation::operator*(Transformation t2){
	return t2.m * m;
}