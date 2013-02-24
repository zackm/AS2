#include "glm/glm.h"

class Raytracer {
public:
	int maxdepth;

	Raytracer(){};
	Raytracer(int);
	void trace(Ray, int, glm::vec3);
};