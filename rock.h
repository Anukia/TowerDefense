#pragma once
#ifndef Rock_H
#define Rock_H

//Cube model made out of triangles
//Contains arrays:
//vertices - vertex positions in homogenous coordinates
//normals - vertex normals in homogenous coordinates
//texCoords - texturing coordinates
//colors - vertex colors (rgba)
//Culling GL_CW
//TBN friendly

#include "model.h"

namespace Models {
	namespace RockInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class Rock : public Model {
	public:
		Rock();
		virtual ~Rock();
		virtual void drawSolid(bool smooth = false);
	};

	extern Rock rock;
}




#endif
