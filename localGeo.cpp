#include "LocalGeo.h"
#include "glm/glm.hpp"

LocalGeo::LocalGeo(glm::vec3 pt,glm::vec3 nm){
	point = pt;
	normal = nm/glm::sqrt(glm::dot(nm,nm));
}
