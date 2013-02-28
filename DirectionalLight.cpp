#include "DirectionalLight.h"

#include <limits>

#include <iostream>

using namespace std;

void DirectionalLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){
	lray->position = local.point;
	lray->direction = -direction;
	lray->t_min = t_min;
	lray->t_max = t_max;
	lcolor->x = color[0];
	lcolor->y = color[1];
	lcolor->z = color[2];
}


