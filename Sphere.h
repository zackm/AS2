#include "glm/glm.hpp"

class Sphere
{
public:
	glm::vec3 center; //assume these are in world coordinates, not pixel coordinates.
	float radius;

	Sphere(void);
	~Sphere(void){};
	bool intersect(Ray& ray, float* thit,LocalGeo* local);
	bool intersect(Ray& ray);
};

