#include "glm/glm.hpp"

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Camera.h"
#endif

//#include "Raytracer.h"

#ifndef __FILM_H__
#define __FILM_H__
#include "Film.h"
#endif

class Scene {
public:
	glm::vec3 eye_position;
	glm::vec3 UL, UR, LL, LR;
	int width, height;

	Scene(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int);
	void render(Camera, Film); // add raytracer arg
};