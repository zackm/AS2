#pragma once
#include "glm/glm.hpp"

#pragma once
#include "Ray.h"

#pragma once
#include "AggregatePrimitive.h"

class Raytracer{
public:
	int maxdepth;
	AggregatePrimitive primitives;

	Raytracer(){maxdepth = 5;};
	Raytracer(int);
	void trace(Ray&, int, glm::vec3*);
};