#pragma once
#include "Transformation.h"

#pragma once
#include "glm/glm.hpp"

Transformation::Transformation(glm::mat4 m_arg) {
	m = m_arg;
	update_invt();
}

glm::mat4 Transformation::left_mult(glm::mat4 m_arg){
	m = m_arg * m;
	update_invt();
}

void Transformation::update_invt() {
	// minvt = glm::transpose(glm::inverse(m));
}