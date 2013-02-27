#include "glm/glm.hpp"
#include "Light.h"
#include "Shape.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Camera.h"
#endif

//#include "Raytracer.h"

#ifndef __FILM_H__
#define __FILM_H__
#include "Film.h"
#endif

#pragma once
#include "Raytracer.h"

#pragma once
#include <list>
using namespace std;

class Scene {
public:
	glm::vec3 eye_position;
	glm::vec3 UL, UR, LL, LR;
	int width, height;
	list<Light> lights;
	list<Shape> shapes;
	Raytracer trace_machine;

	Scene(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int);
	void render(Camera, Film); // add raytracer arg
};