#include "PointLight.h"


void PointLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3 lcolor){
	lray->position = local.point;
	lray->direction = position-local.point;
	lray->t_min = t_min;
	lray->t_max = t_max;
}
