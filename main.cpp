/*
CS184 Assignment 2 - Ray Tracing
Tyler Brabham cs184-ej
Zack Mayeda cs184-bg
*/

#include "glm/glm.hpp"
// OpenGL Math Library
// http://glm.g-truc.net/code.html
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include <list>

// look into pragma once
#ifndef __FILM_H__
#define __FILM_H__
#include "Film.h"
#endif

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Camera.h"
#endif

#include "Scene.h"

#pragma once
#include "Sphere.h"

#pragma once
#include "DirectionalLight.h"

#pragma once
#include "PointLight.h"

#pragma once
#include <vector>

#pragma once
#include "Triangle.h"

#pragma once
#include "Transformation.h"

using namespace std;

const float pi = 3.14159265359;

/*
Simply creates the 4 by 4 rotation matrix where [x,y,z]
is the axis of rotation and angle is the angle in degrees
to rotate.
*/
glm::mat4 create_rotate(float x, float y, float z, float angle) {
	glm::mat4 rotate_mat;
	angle = angle * pi / 180.0f;
	glm::mat4 id(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	glm::mat4 cos_mat(x*x,x*y,x*z,0,x*y,y*y,y*z,0,x*z,y*z,z*z,0,0,0,0,0);
	glm::mat4 sin_mat(0,z,-y,0,-z,0,x,0,y,-x,0,0,0,0,0,0);
	rotate_mat = glm::cos(angle) * id + (1 - glm::cos(angle)) * cos_mat + glm::sin(angle) * sin_mat;
	rotate_mat[3][3] = 1;
	return rotate_mat;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "No filname given. Terminating" << endl;
		exit(1);
	}
	std::string filename = argv[1];
	cout << "Filename " << filename << " found." << endl;
	int WIDTH = 400;
	int HEIGHT = 400;

	Scene s;
	Camera c;
	int maxdepth = 5;
	std::string output_name;
	vector<glm::vec3> vertices;
	vector<glm::vec3> vertexnorm_v;
	vector<glm::vec3> vertexnorm_n;
	vector<glm::mat4> mat_stack;
	glm::mat4 current_mat;

	//default material properties
	glm::vec3 ka(.2f, .2f, .2f);
	glm::vec3 kd(0,0,0);
	glm::vec3 ks(0,0,0);
	glm::vec3 kr(0,0,0);
	glm::vec3 ke(0,0,0);
	float sp = 1;

	bool has_reflect_coeff = false;

	// Push identity matrix onto stack
	glm::mat4 id_mat(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	mat_stack.push_back(id_mat); // top of the stack is the end of the list

	// Arg Parser
	std::ifstream inpfile(filename.c_str());
	if(!inpfile.is_open()) {
		std::cout << "Unable to open file" << std::endl;
	} else {
		std::string line;
		//MatrixStack mst;
		while(inpfile.good()) {
			std::vector<std::string> splitline;
			std::string buf;
			std::getline(inpfile,line);
			std::stringstream ss(line);
			while (ss >> buf) {
				splitline.push_back(buf);
			}
			//Ignore blank lines
			if(splitline.size() == 0) {
				continue;
			}
			//Ignore comments
			if(splitline[0][0] == '#') {
				continue;
			}
			//Valid commands:
			//size width height
			//  must be first command of file, controls image size
			else if(!splitline[0].compare("size")) {
				WIDTH = atoi(splitline[1].c_str());
				HEIGHT = atoi(splitline[2].c_str());
			}

			//maxdepth depth
			//  max # of bounces for ray (default 5)
			else if(!splitline[0].compare("maxdepth")) {
				maxdepth = atoi(splitline[1].c_str());
			}

			//output filename
			//  output file to write image to 
			else if(!splitline[0].compare("output")) {
				output_name = splitline[1];
			}	

			//sphere x y z radius
			//  Deﬁnes a sphere with a given position and radius.
			else if(!splitline[0].compare("sphere")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float r = atof(splitline[4].c_str());
				// Create new sphere:
				//   Store 4 numbers
				//   Store current property values
				//   Store current top of matrix stack

				//make transformation matrix

				Transformation sphere_trans(mat_stack);
				Sphere* sph = new Sphere(glm::vec3(x,y,z),r,ka,kd,ks,kr,ke,sp,sphere_trans);
				s.add_shape(sph);
			}

			//vertex x y z
			//  Deﬁnes a vertex at the given location.
			//  The vertex is put into a pile, starting to be numbered at 0.
			else if(!splitline[0].compare("vertex")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				// Create a new vertex with these 3 values, store in some array
				glm::vec3 vert(x,y,z);
				vertices.push_back(vert);
			}

			//vertexnormal x y z nx ny nz
			//  Similar to the above, but deﬁne a surface normal with each vertex.
			//  The vertex and vertexnormal set of vertices are completely independent
			//  (as are maxverts and maxvertnorms).
			else if(!splitline[0].compare("vertexnormal")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float nx = atof(splitline[4].c_str());
				float ny = atof(splitline[5].c_str());
				float nz = atof(splitline[6].c_str());
				// Create a new vertex+normal with these 6 values, store in some array
				glm::vec3 norm_v(x,y,z);
				glm::vec3 norm_n(nx,ny,nz);
				vertexnorm_v.push_back(norm_v);
				vertexnorm_n.push_back(norm_n);
			}

			//tri v1 v2 v3
			//  Create a triangle out of the vertices involved (which have previously been speciﬁed with
			//  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
			//  should internally compute a face normal for this triangle.
			else if(!splitline[0].compare("tri")) {
				int v1 = atoi(splitline[1].c_str());
				int v2 = atoi(splitline[2].c_str());
				int v3 = atoi(splitline[3].c_str());
				// Create new triangle:
				//   Store pointer to array of vertices
				//   Store 3 integers to index into array
				//   Store current property values
				//   Store current top of matrix stack

				Transformation tri_trans(mat_stack);
				glm::vec3 vert_1 = tri_trans.world_point(vertices[v1]);
				glm::vec3 vert_2 = tri_trans.world_point(vertices[v2]);
				glm::vec3 vert_3 = tri_trans.world_point(vertices[v3]);

				Triangle *t = new Triangle(vert_1,vert_2,vert_3,ka,kd,ks,kr,ke,sp);
				s.add_shape(t);
			}

			//trinormal v1 v2 v3
			//  Same as above but for vertices speciﬁed with normals.
			//  In this case, each vertex has an associated normal, 
			//  and when doing shading, you should interpolate the normals 
			//  for intermediate points on the triangle.
			else if(!splitline[0].compare("trinormal")) {
				int v1 = atoi(splitline[1].c_str());
				int v2 = atoi(splitline[2].c_str());
				int v3 = atoi(splitline[3].c_str());
				// Create new triangle:
				//   Store pointer to array of vertices (Different array than above)
				//   Store 3 integers to index into array
				//   Store current property values
				//   Store current top of matrix stack

				Transformation tri_trans(mat_stack);
				glm::vec3 vert_1 = tri_trans.world_point(vertexnorm_v[v1]);
				glm::vec3 vert_2 = tri_trans.world_point(vertexnorm_v[v2]);
				glm::vec3 vert_3 = tri_trans.world_point(vertexnorm_v[v3]);

				glm::vec3 norm_1 = tri_trans.world_normal(vertexnorm_n[v1]);
				glm::vec3 norm_2 = tri_trans.world_normal(vertexnorm_n[v2]);
				glm::vec3 norm_3 = tri_trans.world_normal(vertexnorm_n[v3]);

				Triangle *t = new Triangle(vert_1,vert_2,vert_3,ka,kd,ks,kr,ke,sp,
										   norm_1,norm_2,norm_3);
				s.add_shape(t);
			}

			//directional x y z r g b
			//  The direction to the light source, and the color, as in OpenGL.
			else if(!splitline[0].compare("directional")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float r = atof(splitline[4].c_str());
				float g = atof(splitline[5].c_str());
				float b = atof(splitline[6].c_str());

				Transformation directional_trans(mat_stack);
				DirectionalLight* dl = new DirectionalLight(glm::vec3(x,y,z),glm::vec3(r,g,b),directional_trans);
				s.add_light(dl);
			}

			//point x y z r g 
			//  The location of a point source and the color, as in OpenGL.
			else if(!splitline[0].compare("point")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float r = atof(splitline[4].c_str());
				float g = atof(splitline[5].c_str());
				float b = atof(splitline[6].c_str());

				Transformation point_trans(mat_stack);
				PointLight* pt = new PointLight(glm::vec3(x,y,z),glm::vec3(r,g,b),point_trans);
				s.add_light(pt);
			}

			// camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
			else if(!splitline[0].compare("camera")){
				float from_x = atof(splitline[1].c_str());
				float from_y = atof(splitline[2].c_str());
				float from_z = atof(splitline[3].c_str());
				float to_x = atof(splitline[4].c_str());
				float to_y = atof(splitline[5].c_str());
				float to_z = atof(splitline[6].c_str());
				float up_x = atof(splitline[7].c_str());
				float up_y = atof(splitline[8].c_str());
				float up_z = atof(splitline[9].c_str());
				float fov = atof(splitline[10].c_str());

				Camera cam(glm::vec3(from_x,from_y,from_z),glm::vec3(to_x,to_y,to_z),glm::vec3(up_x,up_y,up_z),fov);
				c = cam;
			}

			//ambient r g b
			//  The global ambient color to be added for each object 
			//  (default is .2,.2,.2)
			else if(!splitline[0].compare("ambient")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				ka = glm::vec3(r,g,b);
			}

			//diﬀuse r g b
			//  speciﬁes the diﬀuse color of the surface.
			else if(!splitline[0].compare("diffuse")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				kd = glm::vec3(r,g,b);
			}

			//specular r g b 
			//  speciﬁes the specular color of the surface.
			else if(!splitline[0].compare("specular")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				ks = glm::vec3(r,g,b);

				if (!has_reflect_coeff){
					kr = ks;
				}
			}

			//reflection r g b 
			//  speciﬁes the reflective color of the surface.
			else if(!splitline[0].compare("reflect")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				kr = glm::vec3(r,g,b);
				has_reflect_coeff = true;
				// unsure of command line arg ??
			}			

			else if(!splitline[0].compare("emission")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				ke = glm::vec3(r,g,b);
			}

			//shininess s
			//  speciﬁes the shininess of the surface.
			else if(!splitline[0].compare("shininess")) {
				sp = atof(splitline[1].c_str());
			} 

			//translate x y z
			//  A translation 3-vector
			else if(!splitline[0].compare("translate")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				// Update top of matrix stack
				// Matrix of form
				// 1 0 0 tx
				// 0 1 0 ty
				// 0 0 1 tz
				// 0 0 0 1

				glm::mat4 translate_mat(1,0,0,0,0,1,0,0,0,0,1,0,x,y,z,1);
				current_mat = mat_stack.back();

				mat_stack.pop_back();
				current_mat = current_mat * translate_mat;

				mat_stack.push_back(current_mat);
			}

			//rotate x y z angle
			//  Rotate by angle (in degrees) about the given axis as in OpenGL.
			else if(!splitline[0].compare("rotate")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float angle = atof(splitline[4].c_str());
				// Update top of matrix stack

				glm::mat4 rotate_mat = create_rotate(x,y,z,angle);
				current_mat = mat_stack.back();

				mat_stack.pop_back();
				current_mat = current_mat * rotate_mat;

				mat_stack.push_back(current_mat);
			}

			//scale x y z
			//  Scale by the corresponding amount in each axis (a non-uniform scaling).
			else if(!splitline[0].compare("scale")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				// Update top of matrix stack
				// Matrix of form
				// sx 0  0  0
				// 0  sy 0  0
				// 0  0  sz 0
				// 0  0  0  1
				glm::mat4 scale_mat(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);
				current_mat = mat_stack.back();

				mat_stack.pop_back();
				current_mat = current_mat * scale_mat;

				mat_stack.push_back(current_mat);
			}

			//pushTransform
			//  Push the current modeling transform on the stack as in OpenGL. 
			//  You might want to do pushTransform immediately after setting 
			//   the camera to preserve the “identity” transformation.
			else if(!splitline[0].compare("pushTransform")) {
				//Transformation t_copy(current_trans.m);

				current_mat = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
				mat_stack.push_back(current_mat);
			}

			//popTransform
			//  Pop the current transform from the stack as in OpenGL. 
			//  The sequence of popTransform and pushTransform can be used if 
			//  desired before every primitive to reset the transformation 
			//  (assuming the initial camera transformation is on the stack as 
			//  discussed above).
			else if(!splitline[0].compare("popTransform")) {
				mat_stack.pop_back();
			} else {
				std::cerr << "Unknown command: " << splitline[0] << std::endl;
			}

		}
	}
	// End Arg Parser

	int BitsPerPixel = 24;
	Film canvas = Film(WIDTH, HEIGHT, BitsPerPixel, output_name);

	glm::vec3 UL,UR,LL,LR;

	c.cornerVectors(&UL,&UR,&LL,&LR,WIDTH,HEIGHT);
	s.set_params(c.position,UL,UR,LL,LR,WIDTH,HEIGHT,maxdepth);
	s.render(c,canvas);

	return 0;
}