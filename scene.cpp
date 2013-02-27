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
	trace_machine.maxdepth = 5;
}

void Scene::render(Camera c, Film kodak) { // add raytracer arg back
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