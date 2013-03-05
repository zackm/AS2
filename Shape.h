#pragma once
#include "Ray.h"

#pragma once
#include "LocalGeo.h"

#pragma once
#include <iostream>

#pragma once
#include "BRDF.h"

using namespace std;

class Shape {
public:
	Shape(void){};
	~Shape(void){};
	virtual bool intersect(Ray&, float*, LocalGeo*) =0;
	virtual bool intersect(Ray&) =0;
	BRDF brdf;
	virtual BRDF get_brdf() =0;
};

