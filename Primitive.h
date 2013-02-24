#pragma once
#include "Ray.h"
#include "Intersection.h"
#include "localGeo.h"
#include "BRDF.h"

class Primitive
{
public:
	Primitive(void);
	virtual bool intersect(Ray&,float*,Intersection*);
	virtual bool intersect(Ray&);
	void getRBDF(LocalGeo&,BRDF* brdf);
	~Primitive(void);
};

