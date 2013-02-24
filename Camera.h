#include "glm/glm.hpp"
#include "Ray.h"

class Camera {
public:
	void generateRay(float[], Ray, glm::vec3);
	Camera(){};
}