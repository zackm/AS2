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
#include <list>
using namespace std;

#pragma once
#include "BRDF.h"

class Scene {
public:
	glm::vec3 eye_position;
	glm::vec3 UL, UR, LL, LR;
	glm::vec3 ka, kd, ks, kr;
	float shiny;
	int width, height, maxdepth;
	list<Light*> lights;
	list<Shape*> shapes;

	Scene(){};
	Scene(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int,int);
	void set_params(glm::vec3,glm::vec3,glm::vec3,glm::vec3,glm::vec3,int,int,int);
	void render(Camera, Film);
	void add_shape(Shape*);
	void add_light(Light*);
	void trace(Ray &, int, glm::vec3 *);
	bool intersect_checker(Ray&);
	glm::vec3 shading(LocalGeo, BRDF, Ray, glm::vec3);
	void add_ambient(glm::vec3);
	void add_diffuse(glm::vec3);
	void add_specular(glm::vec3);
	void add_shininess(float);
};