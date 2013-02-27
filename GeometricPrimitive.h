#pragma once
#include "glm/glm.hpp"
#include "Primitive.h"
#include "Shape.h"
#include "Material.h"

class GeometricPrimitive : public Primitive{

public:
	glm::mat4 obj_to_world, world_to_obj;
	Shape* shape;
	Material* mat;

	bool intersect(Ray& ray, float* thit, Intersection*in);
	bool intersect(Ray&ray);
	GeometricPrimitive(void){};
	~GeometricPrimitive(void){};
};

