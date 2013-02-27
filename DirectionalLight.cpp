#include "DirectionalLight.h"

#include <limits>

void DirectionalLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){
	lray->position = local.point;
	lray->direction = direction;
	lray->t_min = t_min;
	lray->t_max = t_max;
}

