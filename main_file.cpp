/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
//#include <charconv>
#include <iostream>
#include <vector>
//#include <cstring>
//#include <cstdlib>
#include <cmath>
#include "mobki.h"
#include "wieza.h"
#include "OgarniaczMobkow.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include <imgui/imgui_internal.h>

using namespace std;

//bool turnOffMenu = false; //jesli true, to wychodzimy z menu

//Procedura obsługi błędów
float aspectRatio = 1;
float camX = 1.2f;
float camY = 3.0f;
float camZ = 0.0f;
float speedCamX = 0;
float speedCamZ = 0;
float fov = 120;
float camRotateX = 0;
float camRotateZ = 0;  //raczej mozna usunac camRotateZ
int zloto = 100000;
int hp_baza = 25;
bool showWindow = true;

GLuint grass;
GLuint pavement;
GLuint stone;
GLuint leaf;
GLuint metal;
GLuint chosen;
GLuint tower;

OgarniaczMobkow ogarniacz = OgarniaczMobkow();
Wieza wieze[6] = { Wieza(), Wieza(), Wieza(), Wieza(), Wieza(), Wieza() };

int wybrana_wieza = 1;

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

// bindowanie klawiszy
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	float radius = 0.10f;
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speedCamX = radius;
		if (key == GLFW_KEY_RIGHT) speedCamX = -radius;
		if (key == GLFW_KEY_UP) speedCamZ = 1;
		if (key == GLFW_KEY_DOWN) speedCamZ = -1; 
		if (key == GLFW_KEY_R) { //reset ustawień
			fov = 120;
			camRotateX = 0;
		}
		if (key == GLFW_KEY_W) { //Wyłącz/włącz frame z imgui
			if (showWindow == false) showWindow = true;
			else showWindow = false;
		}
		if (key == GLFW_KEY_1) wybrana_wieza = 1;
		if (key == GLFW_KEY_2) wybrana_wieza = 2;
		if (key == GLFW_KEY_3) wybrana_wieza = 3;
		if (key == GLFW_KEY_4) wybrana_wieza = 4;
		if (key == GLFW_KEY_5) wybrana_wieza = 5;
		if (key == GLFW_KEY_6) wybrana_wieza = 6;
		if (key == GLFW_KEY_Z) {

			std::string helper = "Kup wieze: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztWiezy()) + " zlota"; //zmianic wartosc
			const char* c = helper.c_str();
			if (zloto >= wieze[wybrana_wieza - 1].getterKosztWiezy() && !wieze[wybrana_wieza - 1].getterKupionaWieza()) {
				zloto -= wieze[wybrana_wieza - 1].getterKosztWiezy();
				wieze[wybrana_wieza - 1].setterKupionaWieza();
			}

		}
		if (key == GLFW_KEY_X) {

			if (wieze[wybrana_wieza - 1].getterIdZasieg() < 4) {

				std::string helper = "Ulepsz zasieg: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg()) + " zlota"; //zmianic wartosc
				const char* c = helper.c_str();
				if (wieze[wybrana_wieza - 1].getterKupionaWieza() && zloto >= wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg()) {
					zloto -= wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg();
					wieze[wybrana_wieza - 1].setterIdUlepszeniaZasieg();
				}
			}

		}
		if (key == GLFW_KEY_C) {

			if (wieze[wybrana_wieza - 1].getterIdObrazen() < 4) {
				std::string helper = "Ulepsz obrazenia: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen()) + " zlota"; //zmianic wartosc
				const char* c = helper.c_str();
				if (wieze[wybrana_wieza - 1].getterKupionaWieza() && zloto >= wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen()) {
					zloto -= wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen();
					wieze[wybrana_wieza - 1].setterIdUlepszeniaObrazen();
				}
			}
		}

	}
	
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speedCamX = 0;
		if (key == GLFW_KEY_RIGHT) speedCamX = 0;
		if (key == GLFW_KEY_UP) speedCamZ = 0;
		if (key == GLFW_KEY_DOWN) speedCamZ = 0;
	}
	
}
// bindowanie scrolla 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset; //yoffset mowi nam o ile przesunelismy scrolla 
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_MOUSE_BUTTON_RIGHT) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << xpos <<" cos tam "<< ypos << "\n";
	}
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	grass = readTexture("grass.png");
	stone = readTexture("stone.png");
	pavement = readTexture("pavement.png");
	leaf = readTexture("leafv2.png");
	metal = readTexture("metal.png");
	chosen = readTexture("yellow.png");
	tower = readTexture("tower.png");
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST);

	wieze[0] = Wieza(1.5f, 0.21f, -0.5f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 1, metal, chosen, tower);
	wieze[1] = Wieza(0.9f, 0.21f, 1.3f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 2, metal, chosen, tower);
	wieze[2] = Wieza(0.48f, 0.21f, 1.3f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 3, metal, chosen, tower);
	wieze[3] = Wieza(-0.08f, 0.21f, -1.3f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 4, metal, chosen, tower);
	wieze[4] = Wieza(-0.5f, 0.21f, -1.3f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 5, metal, chosen, tower);
	wieze[5] = Wieza(-1.1f, 0.21f, 0.7f, 0.15f, 0.15f, 0.15f, 0.075f, 0.075f, 0.075f, 6, metal, chosen, metal);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	glfwSetKeyCallback(window, key_callback); // zainicjowanie funkcji
	glfwSetScrollCallback(window, scroll_callback); // zainicjowanie funkcji
	glfwSetMouseButtonCallback(window, mouse_callback);
	ImGui::StyleColorsDark();
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	glDeleteTextures(1, &grass);
	glDeleteTextures(1, &stone);
	glDeleteTextures(1, &pavement);
	glDeleteTextures(1, &leaf);
	glDeleteTextures(1, &metal);
	glDeleteTextures(1, &chosen);

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float time, int &hp_baza) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Kamera
	glm::mat4 V = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	V = glm::rotate(V, camRotateX, glm::vec3(0.0f, 1.0f, 0.0f)); // tutaj odbywa sie obrot
	glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f); // tutaj odbywa sie zoom

	//spLambert->use();
	spLambertTextured->use(); //TODO nie mam pojecia jak korzystać z paru shaderów xD
	// ten shader nie ma mozliwosci pokolorowania obiektu (chyba), tylko tekstury mozna dac

	// Podstawa
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(2.5f, 0.2f, 2.5f));
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
	//glUniform4f(spLambert->u("color"), 1.16f, 1.16f, 0.76f, 1.0f);
	//Models::cube.drawSolid();

	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::cube.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::cube.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::cube.texCoords);

	glActiveTexture(GL_TEXTURE0); //zmieniac numery?
	glBindTexture(GL_TEXTURE_2D, grass);
	glUniform1i(spLambertTextured->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	glDisable(GL_TEXTURE0);
	

	//Drzewo
	glm::mat4 M1 = glm::mat4(1.0f);
	M1 = glm::rotate(M1, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M1 = glm::translate(M1, glm::vec3(1.625f, 0.1f, -1.625f));
	M1 = glm::scale(M1, glm::vec3(0.375, 0.375, 0.375));

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M1));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::tree.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::tree.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::tree.texCoords);
	//glUniform4f(spLambertTextured->u("color"), 0.6f, 0.98f, 0.6f, 1.0f);

	glActiveTexture(GL_TEXTURE0); //zmieniac numery?
	glBindTexture(GL_TEXTURE_2D, leaf);
	glUniform1i(spLambertTextured->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, Models::tree.vertexCount);

	//Models::tree.drawSolid();

	//Drzewo v2

	glm::mat4 M5 = M;
	M5 = glm::rotate(M5, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M5 = glm::translate(M5, glm::vec3(-0.70f, -0.5f, 0.70f));
	M5 = glm::scale(M5, glm::vec3(0.15f, 1.875f, 0.15f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M5));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);

	glDrawArrays(GL_TRIANGLES, 0, Models::tree.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	//Models::tree.drawSolid();

	//Skała

	glm::mat4 M2 = M;
	M2 = glm::rotate(M2, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M2 = glm::translate(M2, glm::vec3(0.65f, 0.5f, 0.45f));
	M2 = glm::scale(M2, glm::vec3(0.10f, 1.25f, 0.10f));
	//glUniform4f(spLambertTextured->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M2));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::rock.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::rock.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::rock.texCoords);

	glActiveTexture(GL_TEXTURE0); //zmieniac numery?
	glBindTexture(GL_TEXTURE_2D, stone);
	glUniform1i(spLambertTextured->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, Models::rock.vertexCount);
	//Models::rock.drawSolid();
	//spLambert->use();

	//Skała v2

	glm::mat4 M3 = M;
	M3 = glm::rotate(M3, 90.0f * PI / 180.0f, glm::vec3(0.01f, 1.0f, 0.01f));
	M3 = glm::translate(M3, glm::vec3(-0.55f, 0.0f, -0.45f));
	M3 = glm::scale(M3, glm::vec3(0.10f, 1.25f, 0.10f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M3));
	//glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);

	glDrawArrays(GL_TRIANGLES, 0, Models::rock.vertexCount);
	//Models::rock.drawSolid();

	//Skała v3

	glm::mat4 M4 = M;
	M4 = glm::rotate(M4, 90.0f * PI / 180.0f, glm::vec3(0.01f, 1.0f, 0.01f));
	M4 = glm::translate(M4, glm::vec3(0.55f, 0.5f, -0.62f));
	M4 = glm::scale(M4, glm::vec3(0.05f, 0.625f, 0.05f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M4));
	//glUniform4f(spLambertTextured->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, Models::rock.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	//Models::rock.drawSolid();

	// Droga --
		glm::mat4 M6 = glm::mat4(1.0f);
	M6 = glm::translate(M6, glm::vec3(1.90f, 0.195f, -0.8f));
	M6 = glm::rotate(M6, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M6 = glm::scale(M6, glm::vec3(0.1f, 0.05f, 0.6));
	glUniform4f(spLambertTextured->u("color"), 1.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M6));
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::cube.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::cube.normals);

	//glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	//glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::cube.texCoords);
	//glActiveTexture(GL_TEXTURE0); //zmieniac numery?
	//glBindTexture(GL_TEXTURE_2D, pavement);
	//glUniform1i(spLambertTextured->u("tex"), 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);
	//Models::cube.drawSolid();

	// Droga v2
		glm::mat4 M10 = glm::mat4(1.0f);
	M10 = glm::translate(M10, glm::vec3(1.2f, 0.195f, 0.4f));
	M10 = glm::scale(M10, glm::vec3(0.1f, 0.05f, 1.3f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M10));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	//Models::cube.drawSolid();
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	// Droga v3 
	glm::mat4 M8 = glm::mat4(1.0f);
	M8 = glm::translate(M8, glm::vec3(0.60f, 0.195f, 1.6f));
	M8 = glm::rotate(M8, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M8 = glm::scale(M8, glm::vec3(0.1f, 0.05f, 0.5f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M8));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	//Models::cube.drawSolid();
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);
	
	// Droga v4 --
		glm::mat4 M9 = glm::mat4(1.0f);
	M9 = glm::translate(M9, glm::vec3(0.2f, 0.195f, -0.10f));
	M9 = glm::scale(M9, glm::vec3(0.1f, 0.05f, 1.60f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M9));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	//Models::cube.drawSolid();
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);
	
	// Droga v5 --
		glm::mat4 M15 = glm::mat4(1.0f);
	M15 = glm::translate(M15, glm::vec3(-0.4f, 0.195f, -1.6f));
	M15 = glm::rotate(M15, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M15 = glm::scale(M15, glm::vec3(0.1f, 0.05f, 0.5f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M15));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	//Models::cube.drawSolid();
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	// Droga v6 --
	glm::mat4 M16 = glm::mat4(1.0f);
	M16 = glm::translate(M16, glm::vec3(-0.8f, 0.195f, -0.2f));
	M16 = glm::scale(M16, glm::vec3(0.1f, 0.05f, 1.3f));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M16));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	// Droga v7 --
	glm::mat4 M7 = glm::mat4(1.0f);
	M7 = glm::translate(M7, glm::vec3(-1.70f, 0.195f, 1.0f));
	M7 = glm::rotate(M7, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M7 = glm::scale(M7, glm::vec3(0.1f, 0.05f, 0.8));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M7));
	//glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	//Models::cube.drawSolid();
	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));

	ogarniacz.robiSwoje(time, hp_baza, zloto);

	for (int i = 0; i < 6; i++) {
		if(wieze[i].getterKupionaWieza()) wieze[i].strzelanie(ogarniacz, time);
	}

	wieze[0].rysuj(wybrana_wieza);
	wieze[1].rysuj(wybrana_wieza);
	wieze[2].rysuj(wybrana_wieza);
	wieze[3].rysuj(wybrana_wieza);
	wieze[4].rysuj(wybrana_wieza);
	wieze[5].rysuj(wybrana_wieza);


	ImGui_ImplGlfwGL3_NewFrame();
	if (showWindow == true) {
		{
			std::string helper;
			const char* c;

			ImGui::Begin("Menu", nullptr); // Nazwa Frame'u
			ImGui::SetWindowSize(ImVec2(430, 180));
			ImGui::Text("Zycie: %d", hp_baza);
			ImGui::Text("Zloto: %d", zloto);
			ImGui::Text("Wybrana pozycja: %d", wybrana_wieza);

			if (!wieze[wybrana_wieza - 1].getterKupionaWieza()) {

				helper = "Kup wieze: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztWiezy()) + " zlota"; //zmianic wartosc
				c = helper.c_str();
				if (ImGui::Button(c, ImVec2(200, 20)) && zloto >= wieze[wybrana_wieza - 1].getterKosztWiezy() && !wieze[wybrana_wieza - 1].getterKupionaWieza()) {
					//warunek czy mozna wgl ulepszyc + zwiekszenie progressbar o 25%, max 100%
					zloto -= wieze[wybrana_wieza - 1].getterKosztWiezy();
					wieze[wybrana_wieza - 1].setterKupionaWieza();
				}
			}

			if (wieze[wybrana_wieza - 1].getterIdZasieg() < 4) {

				helper = "Ulepsz zasieg: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg()) + " zlota"; //zmianic wartosc
				c = helper.c_str();
				if (ImGui::Button(c, ImVec2(200, 20)) && wieze[wybrana_wieza - 1].getterKupionaWieza() && zloto >= wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg()) {
					zloto -= wieze[wybrana_wieza - 1].getterKosztUlepszeniaZasieg();
					wieze[wybrana_wieza - 1].setterIdUlepszeniaZasieg();
				}
				ImGui::SameLine();
				ImGui::ProgressBar(wieze[wybrana_wieza - 1].getterIdZasieg() * 0.25f, ImVec2(200, 0), NULL); //"zasieg: x"

			}
			else {
				ImGui::ProgressBar(wieze[wybrana_wieza - 1].getterIdZasieg() * 0.25f, ImVec2(407, 0), NULL); //"zasieg: x"
			}

			if (wieze[wybrana_wieza - 1].getterIdObrazen() < 4) {
				helper = "Ulepsz obrazenia: " + std::to_string(wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen()) + " zlota"; //zmianic wartosc
				c = helper.c_str();
				if (ImGui::Button(c, ImVec2(200, 20)) && wieze[wybrana_wieza - 1].getterKupionaWieza() && zloto >= wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen()) {
					zloto -= wieze[wybrana_wieza - 1].getterKosztUlepszeniaObrazen();
					wieze[wybrana_wieza - 1].setterIdUlepszeniaObrazen();
				}
				ImGui::SameLine();
				ImGui::ProgressBar(wieze[wybrana_wieza - 1].getterIdObrazen() * 0.25, ImVec2(200, 0), NULL); // dac tekst "obrazenia: x"
			}
			else {
				ImGui::ProgressBar(wieze[wybrana_wieza - 1].getterIdObrazen() * 0.25, ImVec2(407, 0), NULL); // dac tekst "obrazenia: x"

			}

			ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}


	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1800, 950, "Tower Defence", NULL, NULL);  //Utwórz okno.
	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	//while (!glfwWindowShouldClose(window) && !turnOffMenu) { // Menu

	//}
	float time = 0;
	float totaltime = 0;
	glfwSetTime(0);

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float radius = 1.00f;
		camRotateX += speedCamX * radius; // o ile zmieniamy rotate
		fov += (float)speedCamZ * radius;
		if (fov < 60.0f)  // ograniczenia 
			fov = 60.0f;
		if (fov > 155.0f)
			fov = 155.0f;
		time = glfwGetTime() - totaltime;
		totaltime += time;
		drawScene(window, time, hp_baza);
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);

}
