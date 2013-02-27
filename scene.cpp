#include "Scene.h"
#include "Sample.h"

#pragma once
#include <limits>

using namespace std;

Scene::Scene(glm::vec3 eye,glm::vec3 UL_arg,glm::vec3 UR_arg, glm::vec3 LL_arg,
	glm::vec3 LR_arg, int w,int h,int d,list<Light> l, list<Shape> s) {
	eye_position = eye;
	UL = UL_arg;
	UR = UR_arg;
	LL = LL_arg;
	LR = LR_arg;
	width = w;
	height = h;
	lights = l;
	shapes = s;
	maxdepth = d;
}

void Scene::render(Camera c, Film kodak) {
	float u,v;
	Ray ray;
	glm::vec3 color;
	glm::vec3 pix_pos;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			u = i+.5;
			v = j+.5;

			pix_pos = u*(v*LL+(1-v)*UL)+(1-u)*(v*LR+(1-v)*UR);

			c.generateRay(pix_pos, &ray, eye_position);
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			//trace_machine.trace(ray,trace_machine.maxdepth,&color);
			kodak.commit(i, j, color);
		}
	}

	kodak.writeImage();
}

void Scene::trace(Ray &r, int depth, glm::vec3 *color) {
	//for each primitive in list of primitives
	float thit = std::numeric_limits<float>::infinity();
	LocalGeo local;
	Shape best_shape;
	//Intersection in;
	if (depth > maxdepth) {
		color->x = 0;
		color->y = 0;
		color->z = 0;
		return
	}

	bool no_hit = true;
	for (std::list<Shape>::iterator iter=shapes.begin(); iter != shapes.end(); ++iter) {
		Shape s = *iter;
		float current_T;
		LocalGeo current_local;
		bool hit = s.intersect(r, &current_T, &current_local);
		if (current_T < thit && hit) {
			thit = current_T;
			local = current_local;
			no_hit = false;
			best_shape = s;
		}
	}

	if (no_hit) {
		color->x = 0;
		color->y = 0;
		color->z = 0;
		return
	}

	// obtain BRDF at intersection point
	BRDF brdf = s.brdf;
	//in.primitive->getBRDF(in.local, &brdf);

	//// There is an intersection, loop through all light sources
	Ray lray;
	glm::vec3 lcolor(0.0f,0.0f,0.0f);
	for (std::list<Light>::iterator iter=lights.begin(); iter != lights.end(); ++iter) {
		Light l = *iter;
		l.generateLightRay(in.local, &lray, &lcolor);
		// check if the light is blocked or not


		if (!intersect_checker(lray)) {
			// If not, do shading calculation for this light source
			*color += shading(in.local, brdf, lray, lcolor);
		}
	}

	// Handle mirror reflection
	// if (brdf.kr > 0) {
	// 	reflectRay = createReflectRay(in.local, ray);
	// 	// Make a recursive call to trace the reflected ray
	// 	trace(reflectRay, depth+1, &tempColor);
	// 	*color += brdf.kr * tempColor;
	// }
}

bool Scene::intersect_checker(Ray & r) {
	for (std::list<Shape>::iterator iter=shapes.begin(); iter != shapes.end(); ++iter) {
		Shape s = *iter;
		if (s.intersect(r)) {
			return true;
		}
	}
	return false;
}

void Scene::add_shape(Shape *s) {
	shapes.push_front(s);
}

void Scene::add_light(Light *l) {
	lights.push_front(l);
}