#include "DirectionalLight.h"

#include <limits>

#include <iostream>;

using namespace std;

void DirectionalLight::generateLightRay(LocalGeo& local,Ray* lray,glm::vec3* lcolor){
	glm::vec3 pos = local.point;
	glm::vec3 dir = direction;

	float norm = glm::dot(dir,dir);
	if (norm>0){
		dir /= glm::sqrt(norm);
	}

	Ray temp_ray(pos,dir,t_min,t_max);

	*lray = temp_ray;

	glm::vec3 temp_color(color[0],color[1],color[2]);

	*lcolor = temp_color;
}

DirectionalLight::DirectionalLight(glm::vec3 dir,glm::vec3 col, Transformation tr){
	trans = tr;

	direction = trans.world_vector(dir);
	color = col;
	t_min = .001; //might need to be 0
	t_max = std::numeric_limits<float>::infinity();
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
