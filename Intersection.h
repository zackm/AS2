#pragma once
#include "LocalGeo.h"
#include "Primitive.h"
class Intersection
{
public:
	LocalGeo localGeo;
	Primitive* primitive;
	Intersection(void){};
	Intersection(LocalGeo,Primitive*);
	~Intersection(void){};
};

