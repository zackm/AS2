#pragma once
#include "Ray.h"
#include "Intersection.h"

class Primitive
{
public:
	Primitive(void);
	virtual bool intersect(Ray&,float*,Intersection*);
	virtual bool intersect(Ray&);
	void getRBDF();
	~Primitive(void);
};

