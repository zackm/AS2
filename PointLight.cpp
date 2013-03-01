#include "PointLight.h"


void PointLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){
	lray->position = local.point;
	lray->direction = position-local.point;
	lray->t_min = t_min;
	lray->t_max = t_max; //max t should be whatever puts the ray at the location of the light.

	(*lcolor)[0] = color[0];
	(*lcolor)[1] = color[1];
	(*lcolor)[2] = color[2];
}

PointLight::PointLight(glm::vec3 dir,glm::vec3 col,float t1,float t2){
	direction = dir; 
	color = col;
	t_min = t1;
	t_max = t2;
}

PointLight::PointLight(glm::vec3 pos,glm::vec3 col){
	position = pos;
	color = col;
	t_min = .001; //might need to be 0
	t_max = 100000;//needs to be set later I think
}