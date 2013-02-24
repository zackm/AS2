#include "glm/glm.hpp"
#include "Camera.h"
//#include "Raytracer.h"
#include "Film.h"

class Scene {
public:
	glm::vec3 eye_position;
	glm::vec3 UL, UR, LL, LR;
	int width, height;

	Scene(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int);
	void render(Camera, Film); // add raytracer arg
}