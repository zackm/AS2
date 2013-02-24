#include "glm/glm.hpp"

class Raytracer{
public:
	int maxdepth;

	Raytracer(){maxdepth = 5;};
	Raytracer(int);
	void trace(Ray, int, glm::vec3);
};