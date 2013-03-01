#pragma once
#include "LocalGeo.h"

#pragma once
#include "Primitive.h"

class Intersection{
public:
	LocalGeo localGeo;
	Primitive* primitive;

	Intersection(void);
	Intersection(LocalGeo,Primitive*);
	~Intersection(void);
};

