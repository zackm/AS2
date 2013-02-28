#include "Scene.h"

#pragma once
#include <iostream>

#pragma once
#include <limits>

#pragma once
#include "BRDF.h"

#include "Sphere.h"

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
			kodak.commit(width - i, height - j, color);
		}
	}
	kodak.writeImage();
}

void Scene::trace(Ray &r, int depth, glm::vec3 *color) {
	float thit = std::numeric_limits<float>::infinity();
	LocalGeo local;
	Shape* best_shape;
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
		bool hit = (*s).intersect(r, &current_T, &current_local);
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

	// obtain BRDF at intersection point
	// BRDF brdf = best_shape.brdf;
	// glm::vec3 ka(.2f,.2f,.1f);
	// glm::vec3 kd(1,0,0);
	// glm::vec3 ks(1,0,0);
	// glm::vec3 kr(0,0,0);
	BRDF brdf(ka,kd,ks,kr);

	//in.primitive->getBRDF(in.local, &brdf);

	//// There is an intersection, loop through all light sources
	Ray lray;
	glm::vec3 lcolor(0.0f,0.0f,0.0f);

	for (std::list<Light*>::iterator iter=lights.begin(); iter != lights.end(); ++iter) {
		Light* l = *iter;
		float thit;
		LocalGeo test_geo;

		(*l).generateLightRay(local,&lray,&lcolor);

		if (!intersect_checker(lray)) {
			*color += shading(local, brdf, lray, lcolor);
		}
	}
}

bool Scene::intersect_checker(Ray& r){
	float thit = 0;
	LocalGeo local;
	for (std::list<Shape*>::iterator iter=shapes.begin(); iter != shapes.end(); ++iter) {
		Shape* s =  *iter;
		if ((*s).intersect(r,&thit,&local)) {
			//cout<<thit;
			//cin.get();
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
	glm::vec3 view = eye_position-local.point;
	float view_norm = glm::dot(view,view);
	if (view_norm > 0.0f) {
		view = view*(1/glm::sqrt(glm::dot(view,view)));
	}
	float specular = glm::dot(r_vec,view);
	specular = glm::max(specular,0.0f);
	specular = glm::pow(specular,20.0f);//need to change 20 to p coefficient. (variable called shiny)

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

void Scene::add_ambient(glm::vec3 a) {
	ka = a;
}
void Scene::add_diffuse(glm::vec3 d) {
	kd = d;
}
void Scene::add_specular(glm::vec3 s) {
	ks = s;
}
void Scene::add_shininess(float s) {
	shiny = s;
}

//int main(char argc, char* argv[]){
//	//Generate a light
//	DirectionalLight test1(glm::vec3(0,0,-2), glm::vec3(0,0,1));
//
//	//Generate a Ray
//	glm::vec3 camera_pos(0,0,1.2f);
//	glm::vec3 dir(0,0,-1);
//	Ray init_ray(camera_pos,dir,0,100);
//
//	//Generate a sphere
//	Sphere sphere_test(glm::vec3(0,0,0),1.0f);
//
//	//Test the init_ray with the sphere
//	bool hit;
//	LocalGeo geo_test;
//	float t_hit;
//	hit = sphere_test.intersect(init_ray,&t_hit,&geo_test);
//	cout<<geo_test.point[2]<<endl;
//	cout<<t_hit<<endl;
//	cin.get();
//
//
//	//Generate a light ray for this local geo
//	Ray lray;
//	glm::vec3 lcolor(0,0,0);
//	test1.generateLightRay(geo_test,&lray,&lcolor);
//	cout<<lray.position[0]<<','<<lray.position[1]<<','<<lray.position[2]<<endl;
//	cout<<lray.direction[0]<<','<<lray.direction[1]<<','<<lray.direction[2]<<endl;
//	cout<<lray.t_min<<','<<lray.t_max<<endl;
//	cin.get();
//
//	//Test light ray intersection with the same sphere
//	float t_hit2 = 0;
//	LocalGeo geo_test2;
//	bool hit2;
//	hit2 = sphere_test.intersect(lray,&t_hit2,&geo_test2);
//	cout<<hit2<<endl;
//	cout<<t_hit2;
//	cin.get();
//
//	return 0;
//}
