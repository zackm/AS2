#pragma once
#include "Raytracer.h"

#pragma once
#include "Ray.h"

#pragma once
#include "Shape.h"

#pragma once
#include "BRDF.h"

#pragma once
#include "Light.h"

#pragma once
#include "Sphere.h"

#pragma once
#include <list>

#pragma once
#include <iostream>

using namespace std;

Raytracer::Raytracer(int d) {
	maxdepth = d;
}

void Raytracer::trace(Ray &r, int depth, glm::vec3 *color) {
	//for each primitive in list of primitives
	float thit;
	LocalGeo local;
	//Intersection in;
	if (depth > maxdepth) {
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		return
	}
	// I think primitive is referring to aggregate primitive
	if (!primitive.intersect(&r, &thit, &local)) {
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		return
	}

	// obtain BRDF at intersection point
	BRDF brdf;
	//in.primitive->getBRDF(in.local, &brdf);

	//// There is an intersection, loop through all light sources
	//Ray lray;
	//glm::vec3 lcolor;
	//for (std::list<Light>::iterator iter=lights.begin(); iter != lights.end(); ++iter) {
	//	Light l = *iter;
	//	l.generateLightRay(in.local, &lray, &lcolor);
	//	// check if the light is blocked or not
	//	if (!primitive->intersectP(lray)) {
	//		// If not, do shading calculation for this light source
	//		// shading from AS1
	//		//*color += shading(in.local, brdf, lray, lcolor);
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