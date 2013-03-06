#pragma once
#include "Transformation.h"

#pragma once
#include "glm/glm.hpp"

#include <iostream>

using namespace std;

Transformation::Transformation(glm::mat4 m_arg) {
	m = m_arg;
	update_invt();
}

Transformation::Transformation(vector<glm::mat4> mat_vec){
	//multiply all matrices in the stack to make the transformation object.
	glm::mat4 obj_to_world(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	int n = mat_vec.size();
	for (int i = 0; i<n ; i++){
		obj_to_world = obj_to_world * mat_vec[i];
	}
	m = obj_to_world;
}

void Transformation::left_mult(glm::mat4 m_arg){
	m = m_arg * m;
	update_invt();
}

void Transformation::right_mult(glm::mat4 m_arg){
	m = m*m_arg;
	update_invt();
}

void Transformation::update_invt() {
	// minvt = glm::transpose(glm::inverse(m));
}