#pragma once
#include "Sphere.h"
#include "Ray.h"
#include "LocalGeo.h"
#include "glm/glm.hpp"


Sphere::Sphere(void){
	radius = 1.0;
	center = glm::vec3(0,0,0);
}


bool Sphere::intersect(Ray& ray, float* thit,LocalGeo* local){
	if (!intersect(ray)){
		return false;
	}else{
		float a = glm::dot(ray.direction, ray.direction);

		glm::vec3 b_vec = ray.position-center;

		float b = 2*glm::dot(b_vec,ray.direction);
		float c = glm::dot(b_vec,b_vec)-radius*radius;

		float discrim = b*b-4*a*c;

		float addit = glm::sqrt(discrim);
		float root1 = -b+addit;
		float root2 = -b-addit;

		if (root1<0 || root2<0){ //is it possible for both to be negative?
			*thit = glm::max(root1,root2)/(2*a);
		}else{
			*thit = glm::min(root1,root2)/(2*a);

		local->point = ray.position+(*thit)*ray.direction;
		glm::vec3 sphere_vec = local->point-center;
		local->normal = sphere_vec/glm::sqrt(glm::dot(sphere_vec,sphere_vec));
		return true;
		}
	}
}

bool Sphere::intersect(Ray& ray){
	float a = glm::dot(ray.direction, ray.direction);

	glm::vec3 b_vec = ray.position-center;

	float b = 2*glm::dot(b_vec,ray.direction);
	float c = glm::dot(b_vec,b_vec)-radius*radius;

	float discrim = b*b-4*a*c;

	if (discrim<0){
		return false;
	}else{
		return true;
	}
}