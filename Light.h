#pragma once
#include "LocalGeo.h"

#pragma once
#include "Ray.h"

#include "Transformation.h"

class Light{
public:
	float t_min,t_max;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	Transformation trans;
	
	virtual void generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor) =0;
	Light(glm::vec3, glm::vec3);
	Light(void){};
	~Light(void){};
};

