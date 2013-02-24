#pragma once
#include "Light.h"

class DirectionalLight : public Light{
public:
	//t_max needs to be infinity
	glm::vec3 direction;
	void generateLightRay(LocalGeo& local,Ray* lray,glm::vec3 lcolor);
	DirectionalLight(void){t_min = 0;t_max = 10000000;};
	~DirectionalLight(void){};
};

