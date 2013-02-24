#pragma once
#include "glm/glm.hpp"

class BRDF{
public:
	glm::vec3 ka,kd,ks,kr;

	BRDF(void);
	BRDF(glm::vec3,glm::vec3,glm::vec3,glm::vec3);
	~BRDF(void){};
};