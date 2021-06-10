#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Mobki
{
	int hp;
	float x, y, z, speed;

public:

	Mobki(int fhp, float fx, float fy, float fz, float fspeed);
	void rysuj();
	void poruszanie(float time, std::vector<Mobki>& mobki, int index, int& hp_baza);
	void doszedl(std::vector<Mobki>& mobki, int index, int& hp_baza);
};