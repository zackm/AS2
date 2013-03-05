/*
CS184 Assignment 2 - Ray Tracing
Tyler Brabham cs184-ej
Zack Mayeda cs184-bg
*/

/*
	Required Features
	-----------------
	- render arbitrarily oriented ellipsoids (spheres are sufficient
	primitives as long as you properly apply scales and rotations)
	- render polygons (read .obj file)
	- simple Phong Shading (in color)
	- compute shadows
	- compute reflections
	- apply linear transformations to objects
	- use point and directional lights
	- write output to standard image format

	Input Format
	-----------------
	- # comments : ignore lines starting with #
	- blank line : ignore blank lines
	+ General Commands
	- size width height : first command, controls image size
	- maxdepth depth : max number of bounces for a ray (default 5)
	- output filename : output file where image should be written.
	  Can be required or have default filename.
	+ Camera Command
	- camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov : 
	  Notes: fov stands for field of view in y direction. The field of view
	  in the x direction will be determined by image size. The world aspect
	  ratio (distinct from the widtha and height that determine aspect ratio) is
	  always 1; a sphere at the center of a screen will look like a circle, not an
	  ellipse, independent of image aspect ratio.
	+ Geometry Commands
	- sphere x y z radius : defines sphere with given position and radius
	- maxverts number : defines a max number of vertices for later triangle
	  specifications. It must be set before vertices defined. (Your program
	  may not need this, it is a convenience to allocate arrays accordingly.)
	- maxvertnorms number : defines a max number of vertices with normals for
	  later specification. It must be set before vertices with normals are
	  defined. (Same discussion as maxverts command.)
	- vertex x y z : defines vertex at given location. The vertext is put
	  into a pile, starting to be numbered at 0.
	- vertexnormal x y z nx ny nz : Similar to above, but define a surface
	  normal with each vertex. The vertext and vertexnormal set of vertices are
	  completely independent (as are maxverts and maxvertnorms).
	- tri v1 v2 v3 : create a triangle out of the vertices involved (which have
	  previously been specified with the vertext commmand). The vertices are assumed
	  to be specified in counter-clockwise order. Your code should internally compute
	  a face normal for this triangle.
	- trinormal v1 v2 v3 : Same as above but for vertices specified with normals. In
	  this case, each vertext has an associated normal, and when doing shading, you should
	  interpolate the normals for intermediate points on the triangle.
	+ Transformation Commands
	- translate x y z : a translation 3-vector
	- rotate x y z angle : rotate by angle (in degrees) about the given axis as in
	  OpenGL
	- scale x y z : scale by the corresponding amount in each axis (a nonuniform scaling).
	- pushTransform : push the current modeling transform on the stack as in OpenGL.
	  You might want to do pushTransform immediately after setting the camera to preserve
	  the "identity" transformation.
	- popTransform : pop the current transform from the stack as in OpenGL. The sequence
	  of popTransform and pushTransform can be used if desired before every primitive to
	  reset the transformation (assuming the initial camera transformation is on the
	  stack as discussed above).
	+ Light Commands
	- directional x y z r g b : direction to light source and color
	- point x y z r g b : location of point source and color
	- ambient r g b : the global ambient color to be added to each object (defaul .2, .2, .2)
	+ Material Commands
	- diffuse r g b : diffuse color of surface
	- specular r g b : specular color of surface
	- shininess s : shininess of surface
	- emission r g b : emmisive color of surface
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

#include <typeinfo>

using namespace std;

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
	vector<Transformation> t_stack;
	Transformation current_trans;
	glm::vec3 ka(.2f, .2f, .2f);
	glm::vec3 kd(0,0,0);
	glm::vec3 ks(0,0,0);
	glm::vec3 kr(0,0,0);
	float sp = 1;

	// Push identity matrix onto stack
	Transformation id; // need to initialize
	t_stack.push_back(id); // top of the stack is the end of the list

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
				Sphere* sph = new Sphere(glm::vec3(x,y,z),r,ka,kd,ks,kr,sp,current_trans);
				s.add_shape(sph);
			}

			//maxverts number
			//  Deﬁnes a maximum number of vertices for later triangle speciﬁcations. 
			//  It must be set before vertices are deﬁned.
			else if(!splitline[0].compare("maxverts")) {
				// ignore
			}

			//maxvertnorms number
			//  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
			//  It must be set before vertices with normals are deﬁned.
			else if(!splitline[0].compare("maxvertnorms")) {
				// ignore
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
				Triangle *t = new Triangle(vertices[v1],vertices[v2],vertices[v3],ka,kd,ks,kr,sp,current_trans);
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

				// Triangle *t = new Triangle(vertexnorm_v[v1],vertexnorm_v[v2],vertexnorm_v[v3],
				// 						   vertexnorm_n[v1],vertexnorm_n[v2],vertexnorm_n[v3]);
				// s.add_shape(t);
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
				DirectionalLight* dl = new DirectionalLight(glm::vec3(x,y,z),glm::vec3(r,g,b));
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
				PointLight* pt = new PointLight(glm::vec3(x,y,z),glm::vec3(r,g,b));
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
			}

			//reflection r g b 
			//  speciﬁes the reflective color of the surface.
			else if(!splitline[0].compare("reflect")) {
				float r = atof(splitline[1].c_str());
				float g = atof(splitline[2].c_str());
				float b = atof(splitline[3].c_str());
				kr = glm::vec3(r,g,b);
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
				glm::mat4 new_mat(1,0,0,x,0,1,0,y,0,0,1,z,0,0,0,1);
				// current_trans *= new_mat; // need to overload * operator in transformation
			}

			//rotate x y z angle
			//  Rotate by angle (in degrees) about the given axis as in OpenGL.
			else if(!splitline[0].compare("rotate")) {
				float x = atof(splitline[1].c_str());
				float y = atof(splitline[2].c_str());
				float z = atof(splitline[3].c_str());
				float angle = atof(splitline[4].c_str());
				// Update top of matrix stack
				// Matrix of form
				// Rodriguez Formula
				glm::mat4 new_mat; // need to construct
				// current_trans *= new_mat;
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
				glm::mat4 new_mat(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);
				// current_trans *= new_mat;
			}

			//pushTransform
			//  Push the current modeling transform on the stack as in OpenGL. 
			//  You might want to do pushTransform immediately after setting 
			//   the camera to preserve the “identity” transformation.
			else if(!splitline[0].compare("pushTransform")) {
				t_stack.push_back(current_trans);
				// reset current_trans?
			}

			//popTransform
			//  Pop the current transform from the stack as in OpenGL. 
			//  The sequence of popTransform and pushTransform can be used if 
			//  desired before every primitive to reset the transformation 
			//  (assuming the initial camera transformation is on the stack as 
			//  discussed above).
			else if(!splitline[0].compare("popTransform")) {
				current_trans = t_stack.back();
				t_stack.pop_back();
				// should current_trans hold the transformation or need another var?
			} else {
				std::cerr << "Unknown command: " << splitline[0] << std::endl;
			}

		}
	}
	// End Arg Parser

	//Primitive collection pc(primitive_list);
	int BitsPerPixel = 24;
	Film canvas = Film(WIDTH, HEIGHT, BitsPerPixel, output_name);

	glm::vec3 UL,UR,LL,LR;

	c.cornerVectors(&UL,&UR,&LL,&LR,WIDTH,HEIGHT);
	s.set_params(c.position,UL,UR,LL,LR,WIDTH,HEIGHT,maxdepth);
	s.render(c,canvas);

	return 0;
}