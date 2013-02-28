#include "DirectionalLight.h"

#include <limits>

#include <iostream>;

#include "Sphere.h"
using namespace std;

void DirectionalLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){
	lray->position = local.point;
	lray->direction = -direction;
	lray->t_min = t_min;
	lray->t_max = t_max;

	(*lcolor)[0] = color[0];
	(*lcolor)[1] = color[1];
	(*lcolor)[2] = color[2];
}

DirectionalLight::DirectionalLight(glm::vec3 dir,glm::vec3 col,float t1,float t2){
	direction = dir; 
	color = col;
	t_min = t1;
	t_max = t2;
}

DirectionalLight::DirectionalLight(glm::vec3 dir,glm::vec3 col){
	direction = dir; 
	color = col;
	t_min = .001; //might need to be 0
	t_max = 100000;//needs to be inf
}

//int main(char argc, char* argv[]){
//	DirectionalLight test1(glm::vec3(0,0,1), glm::vec3(0,0,1));
//	Ray lray;
//	LocalGeo testGeo(glm::vec3(0,0,1.0f),glm::vec3(0,0,1));
//	glm::vec3 lcolor(0,0,0);
//
//	test1.generateLightRay(testGeo,&lray,&lcolor);
//
//	Sphere testSphere(glm::vec3(0,0,0),1.0f);
//
//	float thit;
//	LocalGeo geo2;
//	cout<<testSphere.intersect(lray);
//	cin.get();
//	return 0;
//}
