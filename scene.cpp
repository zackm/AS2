#include "Scene.h"

#pragma once
#include <iostream>

#pragma once
#include <limits>

#pragma once
#include "BRDF.h"

#include "Shape.h"

#pragma once
#include <typeinfo>

#include "DirectionalLight.h"

using namespace std;

Scene::Scene(glm::vec3 eye,glm::vec3 UL_arg,glm::vec3 UR_arg, glm::vec3 LL_arg,
	glm::vec3 LR_arg, int w,int h,int d) {

	eye_position = eye;
	UL = UL_arg;
	UR = UR_arg;
	LL = LL_arg;
	LR = LR_arg;
	width = w;
	height = h;
	maxdepth = d;
}

void Scene::set_params(glm::vec3 eye,glm::vec3 UL_arg,glm::vec3 UR_arg, glm::vec3 LL_arg,
	glm::vec3 LR_arg, int w,int h,int d) {

	eye_position = eye;
	UL = UL_arg;
	UR = UR_arg;
	LL = LL_arg;
	LR = LR_arg;
	width = w;
	height = h;
	maxdepth = d;
}

void Scene::render(Camera c, Film kodak) {
	float u,v;
	Ray ray;
	glm::vec3 color;
	glm::vec3 pix_pos;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			u = (float(i-(width/2))/width)+.5;
			v = (float(j-(height/2))/height)+.5;

			pix_pos = u*(v*LL+(1-v)*UL)+(1-u)*(v*LR+(1-v)*UR);

			c.generateRay(pix_pos, &ray, eye_position);
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			trace(ray,0,&color);
			kodak.commit(i, j, color);
		}
	}
	kodak.writeImage();
}

void Scene::trace(Ray &r, int depth, glm::vec3 *color) {
	//for each primitive in list of primitives
	float thit = std::numeric_limits<float>::infinity();
	LocalGeo local;
	Shape* best_shape;
	//Intersection in;
	if (depth > maxdepth) {
		color->x = 0;
		color->y = 0;
		color->z = 0;
		return;
	}

	bool no_hit = true;
	for (std::list<Shape*>::iterator iter=shapes.begin(); iter != shapes.end(); ++iter) {
		Shape* s = *iter;
		float current_T;
		LocalGeo current_local;
		bool hit = s->intersect(r, &current_T, &current_local);
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
		return;
	}

	color->x = 1;
	color->y = 0;
	color->z = .5;
	return;

	// obtain BRDF at intersection point
	// BRDF brdf = best_shape.brdf;
	glm::vec3 ka(1,1,1);
	glm::vec3 kd(1,0,0);
	glm::vec3 ks(1,1,0);
	glm::vec3 kr(0,0,0);
	BRDF brdf(ka,kd,ks,kr);

	//in.primitive->getBRDF(in.local, &brdf);

	//// There is an intersection, loop through all light sources
	//Ray lray;
	//glm::vec3 lcolor(0.0f,0.0f,0.0f);
	//for (std::list<Light*>::iterator iter=lights.begin(); iter != lights.end(); ++iter) {
	//	Light* l = *iter;
	//	l->generateLightRay();
	//	
	//	if (!intersect_checker(lray)) {
	//		// If not, do shading calculation for this light source
	//		*color += shading(local, brdf, lray, lcolor);
	//	}
	//}

	// Handle mirror reflection
	// if (brdf.kr > 0) {
	// 	reflectRay = createReflectRay(in.local, ray);
	// 	// Make a recursive call to trace the reflected ray
	// 	trace(reflectRay, depth+1, &tempColor);
	// 	*color += brdf.kr * tempColor;
	// }
}

bool Scene::intersect_checker(Ray & r) {
	for (std::list<Shape*>::iterator iter=shapes.begin(); iter != shapes.end(); ++iter) {
		Shape* s =  *iter;
		if ((*s).intersect(r)) {
			return true;
		}
	}
	return false;
}

glm::vec3 Scene::shading(LocalGeo local, BRDF brdf, Ray lray, glm::vec3 lcolor){
	glm::vec3 normal = local.normal;
	glm::vec3 light_direction = lray.direction;

	//make sure vectors are normal
	normal = normal*(1/glm::sqrt(glm::dot(normal,normal)));
	light_direction = light_direction*(1/glm::sqrt(glm::dot(light_direction,light_direction)));

	glm::vec3 r_vec = -light_direction+2*glm::dot(light_direction,normal)*normal;
	float r_norm = glm::dot(r_vec,r_vec);
	if (r_norm>0){
		r_vec = r_vec/glm::sqrt(r_norm);
	}

	//Calculate the diffuse component
	float diffuse = glm::dot(normal,light_direction);
	diffuse = glm::max(diffuse,0.0f);

	//Calculate the specular component
	//still need viewer vector. I'm just going to pick (0,0,1)
	//Need to change this later.
	//Change view to direction to camera
	glm::vec3 view(0,0,1);
	float specular = glm::dot(r_vec,view);
	specular = glm::max(specular,0.0f);
	specular = glm::pow(specular,20.f);//need to change 20 to p coefficient.

	glm::vec3 out_color;
	out_color[0] = (brdf.ka[0]+brdf.kd[0]*diffuse+brdf.ks[0]*specular)*lcolor[0];
	out_color[1] = (brdf.ka[1]+brdf.kd[1]*diffuse+brdf.ks[1]*specular)*lcolor[1];
	out_color[2] = (brdf.ka[2]+brdf.kd[2]*diffuse+brdf.ks[2]*specular)*lcolor[2];
	return out_color;
}

void Scene::add_shape(Shape* s) {
	shapes.push_front(s);
}

void Scene::add_light(Light* l) {
	lights.push_front(l);
}