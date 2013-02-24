#include "Triangle.h"
#include "glm/glm.hpp"
#include <iostream>
#include "glm/gtc/matrix_inverse.hpp"

using namespace std;

Triangle::Triangle(glm::vec3 arg_a,glm::vec3 arg_b,glm::vec3 arg_c){
	a = arg_a;
	b = arg_b;
	c = arg_c;
}

bool Triangle::intersect(Ray& ray, float* thit, LocalGeo* local){
	return false;
}

bool Triangle::intersect(Ray& ray){
	/* First get the t value associated with the ray intersecting the plane the
	plane the triangle lies in. Then check with t value if the ray lies in the triangle*/
	glm::vec3 plane_vec1 = b-a;
	glm::vec3 plane_vec2 = c-a;

	glm::vec3 normal = glm::cross(plane_vec1,plane_vec2);
	normal /= glm::dot(normal,normal);

	float denom = glm::dot(ray.direction,normal);
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
			glm::mat3 bary_matrix(plane_vec1[0],plane_vec1[1],plane_vec1[2],plane_vec2[0],
								  plane_vec2[1],plane_vec2[2],-ray.direction[0],-ray.direction[1],
								  -ray.direction[2]); //test matrix for row or column first constructor
			glm::mat3 matrix_inv = bary_matrix._inverse();

			//cout<<matrix_inv[1][2]<<'\n';
			//cout<<bary_matrix[0][0];

			glm::vec3 sol = matrix_inv*matrix_point;
			beta = sol[0];
			gamma = sol[1];
			thit = sol[2];

			if (beta>=0 && gamma>=0 && (beta+gamma<=1)){
				return true;
			}else{
				return false;
			}
		}
	}
}

//int main (int argc, char* argv[]){
//	glm::vec3 v1(0,0,0);
//	glm::vec3 v2(1,0,0);
//	glm::vec3 v3(0,1,0);
//
//	glm::vec3 pos(.5,0,5);
//	glm::vec3 dir(0,0,-1);
//
//	Ray testRay(pos,dir,0,10);
//
//	Triangle testTri(v1,v2,v3);
//	cout<<testTri.intersect(testRay);
//
//	cin.get();
//	return 0;
//}