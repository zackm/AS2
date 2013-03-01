#include "BRDF.h"

BRDF::BRDF(glm::vec3 arg_ka,glm::vec3 arg_kd,glm::vec3 arg_ks,glm::vec3 arg_kr,float s){
	ka = arg_ka;
	kd = arg_kd;
	ks = arg_ks;
	kr = arg_kr;
	shiny = s;
}

BRDF::BRDF(void){
	glm::vec3 v(0,0,0);
	ka = kd = ks = kr = v;
	shiny = 50;
}