#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OgarniaczMobkow.h"

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
	int kosztWiezy = 100;
	int kosztUlepszeniaZasieg[4] = { 50, 75, 100, 200 };
	int idUlepszenieZasieg = 0;
	int kosztUlepszeniaObrazen[4] = { 100, 250, 350, 500 };
	int idUlepszenieObrazen = 0;
	float zasieg[5] = { 0.75f, 1.0f, 1.25f, 1.5f, 1.75f };
	int obrazenia[5] = { 1, 2, 3, 4, 5 };

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
	int getterZasieg();
	int getterObrazenia();
	void setterZasieg();
	void setterObrazenia();
	void strzelanie(OgarniaczMobkow& ogarniacz, float czas);
	bool sprawdzZasieg(float mobekX, float mobekZ);
	float czasStrzelania = 1.1f;
	float czasOdStrzalu = 0.0f;
};