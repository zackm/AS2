#pragma once
#include "Ray.h"

#pragma once
#include "Intersection.h"

#pragma once
#include "localGeo.h"

#pragma once
#include "BRDF.h"

class Primitive{

public:
	Primitive(void);
	virtual bool intersect(Ray&,float*,Intersection*);
	virtual bool intersect(Ray&);
	void getBRDF(LocalGeo&,BRDF* brdf);
	~Primitive(void);
};

