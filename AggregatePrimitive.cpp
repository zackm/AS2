#include "AggregatePrimitive.h"

AggregatePrimitive::AggregatePrimitive(std::vector<Primitive*> p_vector) {
	list = p_vector;
}

AggregatePrimitive::intersect(Ray& r, float* thit, Intersection* in) {
	float least_t = MAX_FLOAT;
	for (std::vector<Primitive*>::iterator it = list.begin; it != list.end(); ++it) {
		p = *it;
		p.intersect(r, thit, in);
		if (thit <= least_t) {
			least_t = thit;
			in->primitive = p;

		}
 	}
}

AggregatePrimitive::intersectP(Ray& r) {
	for (std::vector<Primitive*>::iterator it = list.begin; it != list.end(); ++it) {
		p = *it;
		if (p.intersectP(r)) {
			return true;
		}
	}
	return false;
}

AggregatePrimitive::getBRDF(LocalGeo* geo, BRDF* brdf) {
	exit(1); // should never be called
}