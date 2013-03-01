#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <iostream>

using namespace std;

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c,glm::vec3 a,glm::vec3 d,glm::vec3 s,glm::vec3 r){
	a = arg_a;
	b = arg_b;
	c = arg_c;
	brdf.ka = a;
	brdf.kd = d;
	brdf.ks = s;
	brdf.kr = r;
}

BRDF Triangle::get_brdf() {
	return brdf;
}

bool Triangle::intersect(Ray& ray, float* thit, LocalGeo* local){
	glm::vec3 vec1 = b-a;
	glm::vec3 vec2 = c-a;
	glm::vec3 vec3 = -ray.direction;

	float beta, gamma;

	glm::vec3 matrix_point = ray.position-a; //these formulae from slides

	glm::mat3 bary_matrix(vec1[0],vec2[0],vec3[0],vec1[1],vec2[1],vec3[1],vec1[2],vec2[2],vec3[2]);

	float determ = glm::determinant(bary_matrix);
	if (determ==0){
		return false;
	}

	glm::mat3 matrix_inv = bary_matrix._inverse();

	glm::vec3 sol = matrix_inv*matrix_point;
	beta = sol[0];
	gamma = sol[1];
	*thit = sol[2];

	if (*thit<ray.t_min){
		return false;
	}

	if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
		glm::vec3 normal = glm::cross(vec1,vec2);
		normal /= glm::sqrt(glm::dot(normal,normal));
		local->normal = normal;
		local->point = ray.position+*thit*ray.direction;
		return true;
	}else{
		return false;
	}
}

bool Triangle::intersect(Ray& ray){
	float thit = 0;
	glm::vec3 vec1 = b-a;
	glm::vec3 vec2 = c-a;
	glm::vec3 vec3 = -ray.direction;

	float beta, gamma;

	glm::vec3 matrix_point = ray.position-a; //these formulae from slides

	glm::mat3 bary_matrix(vec1[0],vec2[0],vec3[0],vec1[1],vec2[1],vec3[1],vec1[2],vec2[2],vec3[2]);

	float determ = glm::determinant(bary_matrix);
	if (determ==0){
		return false;
	}

	glm::mat3 matrix_inv = bary_matrix._inverse();

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
//	glm::vec3 v1(0.0f,0.0f,1.0f);
//	glm::vec3 v2(1.0f,1.0f,.5f);
//	glm::vec3 v3(0.0f,1.0f,0.0f);
//
//	glm::vec3 pos(0.0f,0.0f,5.0f);
//	glm::vec3 dir(0.0f,0.0f,-1);
//
//	Ray testRay(pos,dir,0,10);
//
//	Triangle testTri(v1,v2,v3);
//	float thit = 0;
//	LocalGeo localTest;
//	testTri.intersect(testRay,&thit,&localTest);
//	cout<<localTest.normal[2];
//
//	cin.get();
//	return 0;
//}