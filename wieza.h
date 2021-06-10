#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Wieza {
private:

	bool kupionaWieza;
	float x, y, z;
	float miejsceXScale, miejsceYScale, miejsceZScale;
	float wiezaXScale, wiezaYScale, wiezaZScale;
	int id;
	GLuint metal;
	GLuint chosen;
	GLuint tower;
	int kosztWiezy = 50;
	int kosztUlepszeniaZasieg[4] = { 50,75,100,200 };
	int idUlepszenieZasieg = 0;
	int kosztUlepszeniaObrazen[4] = { 100,175,250,500 };
	int idUlepszenieObrazen = 0;

public:

	Wieza();
	Wieza(float x, float y, float z, float miejsceXScale, float miejsceYScale, float miejsceZScale,
		float wiezaXScale, float wiezaYScale, float wiezaZScale, int id,
		GLuint metal, GLuint chosen, GLuint tower);
	void rysuj(int wybranaWieza);
	int getterKosztUlepszeniaZasieg();
	void setterIdUlepszeniaZasieg();
	int getterKosztUlepszeniaObrazen();
	void setterIdUlepszeniaObrazen();
	int getterIdZasieg();
	int getterIdObrazen();
	int getterKosztWiezy();
	void setterKupionaWieza();
	bool getterKupionaWieza();

};