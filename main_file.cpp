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
#include <iostream>
#include<cmath>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

//bool turnOffMenu = false; //jesli true, to wychodzimy z menu

//Procedura obsługi błędów
float aspectRatio = 1;
float camX = 1.0;
float camY = 3.0;
float camZ = 0.0;
float speedCamX = 0;
float speedCamZ = 0;
float fov = 120;
float camRotateX = 0;
float camRotateZ = 0;  //raczej mozna usunac camRotateZ 

// bindowanie klawiszy
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	float radius = 0.10f;
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speedCamX = radius;
		if (key == GLFW_KEY_RIGHT) speedCamX = -radius;
		if (key == GLFW_KEY_UP) speedCamZ = -radius; //potencjalnie do usuniecia
		if (key == GLFW_KEY_DOWN) speedCamZ = radius; //potencjalnie do usuniecia
		if (key == GLFW_KEY_R) { //reset ustawień
			fov = 120;
			camRotateX = 0;
		}
	}
	
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speedCamX = 0;
		if (key == GLFW_KEY_RIGHT)speedCamX = 0;
		if (key == GLFW_KEY_UP) speedCamZ = 0; //potencjalnie do usuniecia
		if (key == GLFW_KEY_DOWN) speedCamZ = 0; //potencjalnie do usuniecia
	}
	
}
// bindowanie scrolla 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset; //yoffset mowi nam o ile przesunelismy scrolla 
	if (fov < 60.0f)  // ograniczenia 
		fov = 60.0f;
	if (fov > 155.0f)
		fov = 155.0f;
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback); // zainicjowanie funkcji
	glfwSetScrollCallback(window, scroll_callback); // zainicjowanie funkcji
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//---

	glm::mat4 V = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	V = glm::rotate(V, camRotateX, glm::vec3(0.0f, 1.0f, 0.0f)); // tutaj odbywa sie obrot
	//V = glm::rotate(V, camRotateZ, glm::vec3(1.0f, 0.0f, 0.0f));
	spLambert->use();
	//glm::mat4 P = glm::perspective(120.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);
	glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f); // tutaj odbywa sie zoom
	glm::mat4 M = glm::mat4(1.0f);
	//M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::scale(M, glm::vec3(2.5f, 0.2f, 2.5f));
	//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	//M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 1.16f, 1.16f, 0.76f, 1.0f);
	Models::cube.drawSolid();

	//--
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
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float radius = 1.00f;
		camRotateX += speedCamX * radius; // o ile zmieniamy rotate
		camRotateZ += speedCamZ * radius;  //raczej mozna usunac camRotateZ 
		drawScene(window);
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);

}
