#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <iostream>

using namespace std;

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c){
	a = arg_a;
	b = arg_b;
	c = arg_c;
}

bool Triangle::intersect(Ray& ray, float* thit, LocalGeo* local){
	if (!intersect(ray)){
		return false;
	}else{

		glm::vec3 plane_vec1 = b-a;
		glm::vec3 plane_vec2 = c-a;

		glm::vec3 normal = glm::cross(plane_vec1,plane_vec2);
		normal /= glm::sqrt(glm::dot(normal,normal));
		float denom = glm::dot(ray.direction,normal);

		float t = -(glm::dot(ray.position-a,normal))/(denom);

		float beta, gamma;

		glm::vec3 point = ray.position+t*ray.direction;

		//beta,gamma, t.
		glm::vec3 matrix_point = ray.position-a; //these formulae from slides

		//I believe this to be correct for mat3 constructor
		glm::mat3 bary_matrix(plane_vec1[0],plane_vec2[0],-ray.direction[0],plane_vec1[1],
			plane_vec2[1],-ray.direction[1],plane_vec1[2],plane_vec2[2],
			-ray.direction[2]); //test matrix for row or column first constructor

		glm::mat3 matrix_inv = bary_matrix._inverse();

		glm::vec3 sol = matrix_inv*matrix_point;
		beta = sol[0];
		gamma = sol[1];

		if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
			*thit = sol[2];
			local->normal = normal;
			local->point = ray.position+*thit*ray.direction;
			return true;
		}else{
			return false;
		}
	}
}

bool Triangle::intersect(Ray& ray){
	/* First get the t value associated with the ray intersecting the plane the
	plane the triangle lies in. Then check with t value if the ray lies in the triangle*/
	glm::vec3 plane_vec1 = b-a;
	glm::vec3 plane_vec2 = c-a;

	glm::vec3 normal = glm::cross(plane_vec1,plane_vec2);
	normal /= glm::sqrt(glm::dot(normal,normal));

	float denom = glm::sqrt(glm::dot(ray.direction,normal));
	if (denom==0){
		return false;
	}else{
		float t = -(glm::dot(ray.position-a,normal))/(denom);
		if (t<0){ //negative solution is not what we want I believe
			return false;
		}else{
			float beta, gamma, thit;

			glm::vec3 point = ray.position+t*ray.direction;

			//beta,gamma, t.
			glm::vec3 matrix_point = ray.position-a; //these formulae from slides

			//I believe this to be correct for mat3 constructor
			glm::mat3 bary_matrix(plane_vec1[0],plane_vec2[0],-ray.direction[0],plane_vec1[1],
				plane_vec2[1],-ray.direction[1],plane_vec1[2],plane_vec2[2],
				-ray.direction[2]); //test matrix for row or column first constructor
			glm::mat3 matrix_inv = bary_matrix._inverse();

			glm::vec3 sol = matrix_inv*matrix_point;
			beta = sol[0];
			gamma = sol[1];
			thit = sol[2];

			if (beta>=0 && gamma>=0 && (beta+gamma<=1)){ //<= is to ensure alpha is also >=0.
				return true;
			}else{
				return false;
			}
		}
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