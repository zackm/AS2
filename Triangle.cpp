#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <iostream>

#pragma once
#include "Transformation.h"

using namespace std;

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c,glm::vec3 ka,glm::vec3 d,glm::vec3 s,glm::vec3 r,float sp,Transformation tr){
	a = arg_a;
	b = arg_b;
	c = arg_c;
	brdf.ka = ka;
	brdf.kd = d;
	brdf.ks = s;
	brdf.kr = r;
	brdf.shiny = sp;
	trans = tr;
}

BRDF Triangle::get_brdf() {
	return brdf;
}

bool Triangle::intersect(Ray& ray_arg, float* thit, LocalGeo* local){
	glm::vec4 pos;
	pos[0] = ray_arg.position[0];
	pos[1] = ray_arg.position[1];
	pos[2] = ray_arg.position[2];
	pos[3] = 1;

	glm::vec4 dir;
	dir[0] = ray_arg.direction[0];
	dir[1] = ray_arg.direction[1];
	dir[2] = ray_arg.direction[2];
	dir[3] = 0;	

	pos = trans.minv * pos;
	dir = trans.minv * dir;

	glm::vec3 pos2(pos[0],pos[1],pos[2]);
	glm::vec3 dir2(dir[0],dir[1],dir[2]);

	Ray ray(pos2,dir2,ray_arg.t_min,ray_arg.t_max);

	glm::vec3 vec1 = b-a;
	glm::vec3 vec2 = c-a;
	glm::vec3 vec3 = -ray.direction;

	float beta, gamma;

	glm::vec3 matrix_point = ray.position-a; //these formulae from slides

	glm::mat3 bary_matrix(vec1[0],vec1[1],vec1[2],vec2[0],vec2[1],vec2[2],vec3[0],vec3[1],vec3[2]);
	
	float determ = glm::determinant(bary_matrix);
	if (determ==0){
		return false;
	}

	glm::mat3 matrix_inv = glm::inverse(bary_matrix);

	glm::vec3 sol = matrix_inv*matrix_point;
	beta = sol[0];
	gamma = sol[1];
	*thit = sol[2];

	//cout<<beta<<','<<gamma<<','<<*thit<<endl;

	if (*thit<ray.t_min){
		return false;
	}

	if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
		glm::vec3 normal = glm::cross(vec1,vec2);

		glm::vec4 normal_world(normal[0],normal[1],normal[2],0);
		normal_world = trans.minvt * normal_world;
		normal[0] = normal_world[0];
		normal[1] = normal_world[1];
		normal[2] = normal_world[2];

		normal /= glm::sqrt(glm::dot(normal,normal));

		local->normal = normal;

		glm::vec3 point_obj = ray.position+(*thit)*ray.direction;
		glm::vec4 point_world(point_obj[0],point_obj[1],point_obj[2],1);
		point_world = trans.m * point_world;
		point_obj[0] = point_world[0];
		point_obj[1] = point_world[1];
		point_obj[2] = point_world[2];

		local->point = point_obj;

		//cout<<normal[0]<<','<<normal[1]<<','<<normal[2]<<endl;
		return true;
	}else{
		return false;
	}
}

bool Triangle::intersect(Ray& ray_arg){
	glm::vec4 pos;
	pos[0] = ray_arg.position[0];
	pos[1] = ray_arg.position[1];
	pos[2] = ray_arg.position[2];
	pos[3] = 1;

	glm::vec4 dir;
	dir[0] = ray_arg.direction[0];
	dir[1] = ray_arg.direction[1];
	dir[2] = ray_arg.direction[2];
	dir[3] = 0;	

	pos = trans.minv * pos;
	dir = trans.minv * dir;

	glm::vec3 pos2(pos[0],pos[1],pos[2]);
	glm::vec3 dir2(dir[0],dir[1],dir[2]);

	Ray ray(pos2,dir2,ray_arg.t_min,ray_arg.t_max);

	float thit = 0;
	glm::vec3 vec1 = b-a;
	glm::vec3 vec2 = c-a;
	glm::vec3 vec3 = -ray.direction;

	float beta, gamma;

	glm::vec3 matrix_point = ray.position-a; //these formulae from slides

	glm::mat3 bary_matrix(vec1[0],vec1[1],vec1[2],vec2[0],vec2[1],vec2[2],vec3[0],vec3[1],vec3[2]);

	float determ = glm::determinant(bary_matrix);
	if (determ==0){
		return false;
	}

	glm::mat3 matrix_inv = glm::inverse(bary_matrix);

	glm::vec3 sol = matrix_inv*matrix_point;

	beta = sol[0];
	gamma = sol[1];
	thit = sol[2];

	if (thit<ray.t_min){
		return false;
	}

	if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
		return true;
	}else{
		return false;
	}
}

//int main (int argc, char* argv[]){
//	glm::vec3 v1(-1,0.0f,0);
//	glm::vec3 v2(1.0f,0,0);
//	glm::vec3 v3(0.0f,1.0f,0.0f);
//
//	glm::vec3 pos(0.0f,0.0f,5.0f);
//	glm::vec3 dir(0.0f,0.0f,-1);
//
//	Ray testRay(pos,dir,0,10);
//
//	Triangle testTri(v1,v2,v3,v1,v1,v1,v1,5);
//	float thit = 0;
//	LocalGeo localTest;
//	cout<<testTri.intersect(testRay,&thit,&localTest);
//
//	cin.get();
//	return 0;
//}