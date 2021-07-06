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
	int poziom;
	bool umarl = false;
	Mobki(int fhp, float fx, float fy, float fz, float fspeed);
	void rysuj(GLuint* colors);
	void poruszanie(float time, std::vector<Mobki>& mobki, int index, int& hp_baza, GLuint* colors);
	void doszedl(std::vector<Mobki>& mobki, int index, int& hp_baza);
	void odejmij_zycie(int ile);
	float getx() { return x; };
	float gety() { return y; };
	float getz() { return z; };
};