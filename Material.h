#pragma once
#include "BRDF.h"
class Material
{
public:
	BRDF constantBRDF;

	Material(void);
	~Material(void);
	BRDF getBRDf(){
		return constantBRDF;
	}
};

