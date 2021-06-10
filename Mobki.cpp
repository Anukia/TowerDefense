#include "Mobki.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shaderprogram.h"
#include "allmodels.h"

using namespace glm;

Mobki::Mobki(int fhp, float fx, float fy, float fz, float fspeed) {
	hp = fhp;
	speed = fspeed;
	x = fx;
	y = fy;
	z = fz;
}

void Mobki::rysuj() {
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(x, y, z));
	M = scale(M, vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::sphere.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::sphere.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::sphere.texCoords);

	glDrawArrays(GL_TRIANGLES, 0, Models::sphere.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
}

void Mobki::poruszanie(float time, std::vector<Mobki>& mobki, int index, int& hp_baza) {
	if (x > 1.2f && z == -0.8f)
	{
		x -= speed * time;
		if (x < 1.2f) x = 1.2f;
	}
	else if (x == 1.2f && z < 1.6f) {
		z += speed * time;
		if (z > 1.6f) z = 1.6f;
	}
	else if (x > 0.2f && z == 1.6f) {
		x -= speed * time;
		if (x < 0.2f) x = 0.2f;
	}
	else if (x == 0.2f && z > -1.6f) {
		z -= speed * time;
		if (z < -1.6f) z = -1.6f;
	}
	else if (x > -0.8f && z == -1.6f) {
		x -= speed * time;
		if (x < -0.8f) x = -0.8f;
	}
	else if (x == -0.8f && z < 1.0f) {
		z += speed * time;
		if (z > 1.0f) z = 1.0f;
	}
	else if (x > -2.5f && z == 1.0f) {
		x -= speed * time;
		if (x < -2.5f) x = -2.5f;
	}
	else if (x == -2.5f && z == 1.0f) {
		doszedl(mobki, index, hp_baza);
	}
	rysuj();
}

void Mobki::doszedl(std::vector<Mobki>& mobki, int index, int& hp_baza)
{
	hp_baza -= hp;
	std::cout << hp_baza << std::endl;
	mobki.erase(mobki.begin() + index);
}