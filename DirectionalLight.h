#pragma once
#include "Light.h"

#pragma once
#include <limits>

class DirectionalLight : public Light {
public:
	glm::vec3 direction;
	void generateLightRay(LocalGeo& local,Ray* lray,glm::vec3 lcolor);
	DirectionalLight(void){t_min = 0;t_max = std::numeric_limits<float>::infinity();};
	~DirectionalLight(void){};
};

