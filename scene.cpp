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
	- attenuation const linear quadratic : sets the constant, linear, and quadratic
	  attenuations (default 1, 0 0) as in OpenGL. By default there is no attenuation
	  (the constant term is 1)
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
#include "Film.h"

int main(int argc, char *argv[]) {
	Film text;
	return 0;
}