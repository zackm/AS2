#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <iostream>

#pragma once
#include <limits>

#pragma once
#include "Transformation.h"

using namespace std;

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c,glm::vec3 ka,glm::vec3 d,glm::vec3 s,
				   glm::vec3 r,glm::vec3 e,float sp,Transformation tr){
					   a = arg_a;
					   b = arg_b;
					   c = arg_c;
					   brdf.ka = ka;
					   brdf.kd = d;
					   brdf.ks = s;
					   brdf.kr = r;
					   brdf.ke = e;
					   brdf.shiny = sp;
					   trans = tr;
					   trinormal = false;
}

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c,glm::vec3 ka,glm::vec3 d,glm::vec3 s,
				   glm::vec3 r,glm::vec3 e,float sp,Transformation tr,glm::vec3 a_norm_arg, glm::vec3 b_norm_arg, glm::vec3 c_norm_arg){
					   //Triangle(arg_a,arg_b,arg_c,ka,d,s,r,e,sp,tr); //this doesn't work for some reason
					   trinormal = true;
					   a = arg_a;
					   b = arg_b;
					   c = arg_c;
					   brdf.ka = ka;
					   brdf.kd = d;
					   brdf.ks = s;
					   brdf.kr = r;
					   brdf.ke = e;
					   brdf.shiny = sp;
					   trans = tr;
					   a_norm = a_norm_arg;
					   b_norm = b_norm_arg;
					   c_norm = c_norm_arg;
}

BRDF Triangle::get_brdf() {
	return brdf;
}

bool Triangle::intersect(Ray& ray_arg, float* thit, LocalGeo* local){
	//transform ray to object space and make a new ray
	glm::vec3 pos = trans.object_point(ray_arg.position);
	glm::vec3 dir = trans.object_vector(ray_arg.direction);
	Ray ray(pos,dir,ray_arg.t_min,ray_arg.t_max);

	glm::vec3 vec1 = b-a;
	glm::vec3 vec2 = c-a;
	glm::vec3 vec3 = -ray.direction;

	float alpha, beta, gamma;

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
	alpha = 1-beta-gamma;

	//cout<<beta<<','<<gamma<<','<<*thit<<endl;

	if (*thit<ray.t_min || *thit>ray.t_max){
		return false;
	}

	//if (*thit<ray.t_min){
	//	return false;
	//}

	if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
		glm::vec3 normal;
		if (trinormal){
			normal = alpha*a_norm+beta*b_norm+gamma*c_norm; //need to apply transformation here I believe.
		}else{
			normal = glm::cross(vec1,vec2);
		}

		//change back into world coordinates
		glm::vec3 normal_world = trans.world_normal(normal);

		glm::vec3 point_obj = ray.position+(*thit)*ray.direction;
		glm::vec3 point_world = trans.world_point(point_obj);

		LocalGeo temp_local(point_world, normal_world);

		*local = temp_local;
		//cout<<normal[0]<<','<<normal[1]<<','<<normal[2]<<endl;
		return true;
	}else{
		return false;
	}
}

bool Triangle::intersect(Ray& ray_arg){
	//transform ray to object space and make a new ray
	glm::vec3 pos = trans.object_point(ray_arg.position);
	glm::vec3 dir = trans.object_vector(ray_arg.direction);
	Ray ray(pos,dir,ray_arg.t_min,ray_arg.t_max);

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

	if (thit<ray.t_min || thit>ray.t_max){
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