#pragma once
#include "Ray.h"

#pragma once
#include "localGeo.h"

class Shape{
public:
	Shape(void);
	~Shape(void);
	virtual bool intersect(Ray&, float*, LocalGeo*){return false;};
	virtual bool intersect(Ray&){return false;};
};

