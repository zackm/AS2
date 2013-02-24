#include "Scene.h"
#include "Sample.h"

using namespace std;

Scene::Scene(glm::vec3 eye,glm::vec3 UL_arg,glm::vec3 UR_arg, glm::vec3 LL_arg, glm::vec3 LR_arg, int w,int h) {
	eye_position = eye;
	UL = UL_arg;
	UR = UR_arg;
	LL = LL_arg;
	LR = LR_arg;
	width = w;
	height = h;
}

Scene::render(Camera c, Film kodak) { // add raytracer arg back
	Sample sample(0,0);
	Ray ray;
	glm::vec3 color;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			sample.x = i + 0.5f;
			sample.y = j + 0.5f;
			float centerx = width / 2;
			float centery = height / 2;
			// alternative formula in lecture slides
			float pos[2] = { ((sample.x - centerx) / width ) * (UR - UL),
							 ((sample.y - centery) / height) * (UR - LR) };
			c.generateRay(pos, &ray, eye_position);
			color[0] = 1.0f;
			color[1] = 0.0f;
			color[2] = 1.0f;
			//r.trace(ray,&color);
			kodak.commit(i, j, color);
		}
	}

	kodak.writeImage();
}