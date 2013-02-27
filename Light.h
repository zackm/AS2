#pragma once
#include "LocalGeo.h"

#pragma once
#include "Ray.h"

class Light{
public:
	float t_min,t_max;
	glm::vec3 position;
	glm::vec3 color;
	virtual void generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){};
	Light(glm::vec3&, glm::vec3&);
	Light(void){};
	~Light(void){};
};

