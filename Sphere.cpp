#pragma once
#include "Sphere.h"
#include "Ray.h"
#include "LocalGeo.h"
#include "glm/glm.hpp"
#include <iostream>
#include "DirectionalLight.h"

using namespace std;


Sphere::Sphere(void){
	radius = 1.0;
	center = glm::vec3(0,0,0);
}

Sphere::Sphere(glm::vec3 arg_center, float arg_radius){
	center = arg_center;
	radius = arg_radius;
}


bool Sphere::intersect(Ray& ray, float* thit,LocalGeo* local){
	float a = glm::dot(ray.direction, ray.direction);

	glm::vec3 b_vec = ray.position-center;

	float b = 2*glm::dot(b_vec,ray.direction);
	float c = glm::dot(b_vec,b_vec)-radius*radius;

	float discrim = b*b-4*a*c;

	if (discrim<0){
		return false;
	}

	float addit = glm::sqrt(discrim);
	float root1 = (-b+addit)/(2.0f*a);
	float root2 = (-b-addit)/(2.0f*a);

	if (root1<ray.t_min){
		if (root2<ray.t_min){
			return false; //negative solutions
		}else{
			*thit = root2;
		}
	}else if(root2<ray.t_min){
		*thit = root1;
	}else{
		*thit = glm::min(root1,root2);
	}

	local->point = ray.position+(*thit)*ray.direction;
	glm::vec3 sphere_vec = local->point-center;
	local->normal = sphere_vec/glm::sqrt(glm::dot(sphere_vec,sphere_vec));
	return true;
}

bool Sphere::intersect(Ray& ray){
	float a = glm::dot(ray.direction, ray.direction);

	glm::vec3 b_vec = ray.position-center;

	float b = 2*glm::dot(b_vec,ray.direction);
	float c = glm::dot(b_vec,b_vec)-radius*radius;

	float discrim = b*b-4*a*c;

	if (discrim<0){
		return false;
	}

	float addit = glm::sqrt(discrim);
	float root1 = (-b+addit)/(2.0f*a);
	float root2 = (-b-addit)/(2.0f*a);

	if (root1<ray.t_min){
		if (root2<ray.t_min){
			return false; //negative solutions
		}
	}
	return true;
}














//int main (int argc, char* argv[]){
//	glm::vec3 v(0,0,0);
//	Sphere testSphere(v,1.0f);
//
//	glm::vec3 pos(0,0,1);
//	glm::vec3 dir(0,0,1);
//
//	DirectionalLight test(glm::vec3(0,0,-2),glm::vec3(1,1,1));
//
//	LocalGeo testGeo;
//	float thit = 0;
//	Ray initRay(glm::vec3(0,0,2),glm::vec3(0,0,-1),0,10);
//	testSphere.intersect(initRay,&thit,&testGeo);
//
//	Ray testRay;
//	glm::vec3 color(0,0,0);
//
//	test.generateLightRay(testGeo,&testRay,&color);
//
//	//cout<<testRay.t_min;
//	//cout<<testGeo.point[1];
//
//	testRay.t_min = 1;
//	bool hit = testSphere.intersect(testRay,&thit,&testGeo);
//
//	cout<<hit;
//	cin.get();
//	return 0;
//}