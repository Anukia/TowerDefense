#pragma once

#ifndef woodenTowerWatch_H
#define woodenTowerWatch_H

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
	namespace WoodenTowerWatchInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class WoodenTowerWatch : public Model {
	public:
		WoodenTowerWatch();
		virtual ~WoodenTowerWatch();
		virtual void drawSolid(bool smooth = false);
	};

	extern WoodenTowerWatch woodenTowerWatch;
}




#endif
