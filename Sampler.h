#include "Sample.h"

class Sampler {
public:
	int width, height;

	Sampler(){};
	Sampler(int,int);
	bool generateSample(Sample);
};