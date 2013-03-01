#include "GeometricPrimitive.h"


bool GeometricPrimitive::intersect(Ray& ray,float* thit, Intersection* in){
	return false;
}

bool GeometricPrimitive::intersect(Ray&ray){
	Ray oray;

	glm::vec3 v_proj;
	glm::vec4 v(ray.position[0],ray.position[1],ray.position[2],1.0f);

	v = world_to_obj*(v);
	v_proj[0] = v[0];
	v_proj[1] = v[1];
	v_proj[2] - v[2];

	oray.position = v_proj;

	//glm::vec3 dir_proj;
	//glm::vec4

	return false;
}