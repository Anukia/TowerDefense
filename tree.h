#pragma once

#ifndef tree_H
#define tree_H

#include "model.h"

namespace Models {
	namespace TreeInternal {
		extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
	}

	class Tree : public Model {
	public:
		Tree();
		virtual ~Tree();
		virtual void drawSolid(bool smooth = false);
	};

	extern Tree tree;
}




#endif
