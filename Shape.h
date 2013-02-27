#pragma once
#include "Ray.h"

#pragma once
#include "localGeo.h"

#pragma once
#include <iostream>

using namespace std;

class Shape {
public:
	Shape(void){};
	~Shape(void){};
	virtual bool intersect(Ray&, float*, LocalGeo*) =0;
	virtual bool intersect(Ray&) =0;
};

