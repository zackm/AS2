#include "Camera.h"
#include "glm/glm.hpp"

float pi = 3.14159265359;

void Camera::generateRay(glm::vec3 pos, Ray *r, glm::vec3 eye) {
	glm::vec3 ray_vec(pos[0] - eye[0], pos[1] - eye[1], pos[2] - eye[2]);
	r->position = pos;
	r->direction = ray_vec;
	r->t_min = .001; //make a small number, maybe could be 0.
	r->t_max = 10; // UNFINISHED
}

Camera::Camera(glm::vec3 pos,glm::vec3 dir,glm::vec3 up_arg,float fov_arg){
	position = pos;
	direction = dir-pos;
	up = up_arg;
	fov = fov_arg;
}

void Camera::cornerVectors(glm::vec3* UL,glm::vec3* UR,glm::vec3* LL,glm::vec3* LR,float scene_width,float scene_height){
	float world_height = 2*glm::tan(pi*(fov/2)/180); //convert to radians
	float world_width = world_height*(scene_width/scene_height);

	glm::vec3 nm_dir = glm::sqrt(1/glm::dot(direction,direction))*direction;

	glm::vec3 screen_center = position+nm_dir;

	glm::vec3 w = glm::cross(nm_dir,up);
	w = w*(1/(2*glm::sqrt(glm::dot(w,w))))*world_width;

	//we have 2, so now need to make sure we get an orthonormal basis.

	glm::vec3 up_onb = glm::cross(w,nm_dir);
	float up_norm = glm::dot(up_onb,up_onb);
	if (up_norm>0.0f){
		up_onb /= glm::sqrt(up_norm);
	}
	
	glm::vec3 v = up_onb*(.5f)*world_height;

	*UL = screen_center+v-w;
	*UR = screen_center+v+w;
	*LL = screen_center-v-w;
	*LR = screen_center-v+w;
}