#pragma once
#include "Shape.h"

#pragma once
#include "Transformation.h"

class Triangle : public Shape{
public:
	//Three vertices denote the triangle
	glm::vec3 a,b,c;
	Transformation trans;

	Triangle(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,float,Transformation tr);
	bool intersect(Ray&, float*, LocalGeo*);
	bool intersect(Ray&);
	Triangle(void){};
	~Triangle(void){};
	BRDF get_brdf();
};