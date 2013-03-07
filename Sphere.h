#include "glm/glm.hpp"

#pragma once
#include "Ray.h"

#pragma once
#include "LocalGeo.h"
#include "Shape.h"

#pragma once
#include "Transformation.h"

class Sphere: public Shape{
public:
	glm::vec3 center; //assume these are in world coordinates, not pixel coordinates.
	float radius;

	Sphere(void);
	Sphere(glm::vec3,float,glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,float,Transformation);
	~Sphere(void){};
	bool intersect(Ray& ray, float* thit, LocalGeo* local);
	bool intersect(Ray& ray);
	BRDF get_brdf();
};

