#include "Sampler.h"

Sampler::Sampler(int w, int h) {
	width = w;
	height = h;
}

Sampler::generateSample(Sample s) {
	if (s.x == width) {
		s.x = 0;
		s.y += 1;
	} else {
		s.x += 1;
	}

	if (s.y > height) {
		return false;
	} else {
		return true;
	}
}