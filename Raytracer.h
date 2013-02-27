#pragma once
#include "glm/glm.hpp"

#pragma once
#include "Ray.h"

class Raytracer{
public:
	int maxdepth;

	Raytracer(){maxdepth = 5;};
	Raytracer(int);
	void trace(Ray&, int, glm::vec3*);
};