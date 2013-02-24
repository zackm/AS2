#include "Camera.h"

Camera::generateRay(float[] pos, Ray r, glm::vec3 eye) {
	glm::vec3 ray_vec(pos[0] - eye[0], pos[1] - eye[1], - eye[2]);
	r.position = eye;
	r.direction = ray_vec;
	r.t_min = 1;
	r.t_max = 10; // UNFINISHED
}