#pragma once
#include "Shape.h"

class Triangle : public Shape{
public:
	//Three vertices denote the triangle
	glm::vec3 a,b,c;

	Triangle(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3);
	bool intersect(Ray&, float*, LocalGeo*);
	bool intersect(Ray&);
	Triangle(void){};
	~Triangle(void){};
	BRDF get_brdf();
};