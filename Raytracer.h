#pragma once
#include "glm/glm.hpp"

#pragma once
#include "Ray.h"

#pragma once
#include <list>

#pragma once
#include "Light.h"

#pragma once
#include "Shapes.h"

class Raytracer{
public:
	int maxdepth;
	list<Light> lights;
	list<Shapes> shapes;

	Raytracer(){maxdepth = 5;};
	Raytracer(int,list,list);
	void trace(Ray&, int, glm::vec3*);
};