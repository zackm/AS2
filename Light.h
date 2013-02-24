#pragma once
#include "LocalGeo.h"
#include "Ray.h"

class Light{
public:
	float t_min,t_max;
	glm::vec3 position;
	virtual void generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){};
	Light(void){};
	~Light(void){};
};

