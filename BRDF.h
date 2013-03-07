#pragma once
#include "glm/glm.hpp"

class BRDF{
public:
	glm::vec3 ka,kd,ks,kr,ke;
	float shiny;

	BRDF(void);
	BRDF(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,float);
	~BRDF(void){};
};