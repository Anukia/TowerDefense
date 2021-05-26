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
float camX = 1.2;
float camY = 3.0;
float camZ = 0.0;
float speedCamX = 0;
float speedCamZ = 0;
float fov = 120;
float camRotateX = 0;
float camRotateZ = 0;  //raczej mozna usunac camRotateZ 
GLuint grass;
GLuint pavement;
GLuint stone;

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
	grass = readTexture("grass.jpg");
	stone = readTexture("stone.jpg");
	pavement = readTexture("pavement.png");
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, key_callback); // zainicjowanie funkcji
	glfwSetScrollCallback(window, scroll_callback); // zainicjowanie funkcji
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	glDeleteTextures(1, &grass);
	glDeleteTextures(1, &stone);
	glDeleteTextures(1, &pavement);
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Kamera
	glm::mat4 V = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	V = glm::rotate(V, camRotateX, glm::vec3(0.0f, 1.0f, 0.0f)); // tutaj odbywa sie obrot
	glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f); // tutaj odbywa sie zoom

	spLambert->use();
	//spLambertTextured->use();

	// Podstawa

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(2.5f, 0.2f, 2.5f));

	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 1.16f, 1.16f, 0.76f, 1.0f);
	Models::cube.drawSolid();

	/*
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::cube.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::cube.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::cube.texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pavement);
	glUniform1i(spLambertTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	*/


	//Drzewo
	//spLambert->use();
	glm::mat4 M1 = M;
	M1 = glm::rotate(M1, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M1 = glm::translate(M1, glm::vec3(0.65f, 0.5f, -0.65f));
	M1 = glm::scale(M1, glm::vec3(0.15f, 1.875f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
	glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);
	Models::tree.drawSolid();

	//Skała

	glm::mat4 M2 = M;
	M2 = glm::rotate(M2, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M2 = glm::translate(M2, glm::vec3(0.65f, 0.5f, 0.45f));
	M2 = glm::scale(M2, glm::vec3(0.10f, 1.25f, 0.10f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M2));
	glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);
	Models::rock.drawSolid();

	//Skała v2

	glm::mat4 M3 = M;
	M3 = glm::rotate(M3, 90.0f * PI / 180.0f, glm::vec3(0.01f, 1.0f, 0.01f));
	M3 = glm::translate(M3, glm::vec3(-0.55f, 0.0f, -0.45f));
	M3 = glm::scale(M3, glm::vec3(0.10f, 1.25f, 0.10f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M3));
	glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);
	Models::rock.drawSolid();

	//Skała v3

	glm::mat4 M4 = M;
	M4 = glm::rotate(M4, 90.0f * PI / 180.0f, glm::vec3(0.01f, 1.0f, 0.01f));
	M4 = glm::translate(M4, glm::vec3(0.55f, 0.5f, -0.62f));
	M4 = glm::scale(M4, glm::vec3(0.05f, 0.625f, 0.05f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M4));
	glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 1.0f, 1.0f);
	Models::rock.drawSolid();

	//Drzewo v2

	glm::mat4 M5 = M;
	M5 = glm::rotate(M5, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M5 = glm::translate(M5, glm::vec3(-0.70f, -0.5f, 0.70f));
	M5 = glm::scale(M5, glm::vec3(0.15f, 1.875f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M5));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::tree.drawSolid();

	// Droga --

	glm::mat4 M6 = glm::mat4(1.0f);
	M6 = glm::rotate(M6, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M6 = glm::translate(M6, glm::vec3(0.8f, 0.195f, 1.90f));
	M6 = glm::scale(M6, glm::vec3(0.1f, 0.01f, 0.6));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M6));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v2

	glm::mat4 M7 = glm::mat4(1.0f);
	M7 = glm::rotate(M7, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M7 = glm::translate(M7, glm::vec3(-1.0f, 0.195f, -1.70f));
	M7 = glm::scale(M7, glm::vec3(0.1f, 0.01f, 0.8));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M7));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v3 --

	glm::mat4 M8 = glm::mat4(1.0f);
	M8 = glm::rotate(M8, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M8 = glm::translate(M8, glm::vec3(-1.6f, 0.195f, 0.60f));
	M8 = glm::scale(M8, glm::vec3(0.1f, 0.01f, 0.5f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M8));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v4 --

	glm::mat4 M9 = glm::mat4(1.0f);
	M9 = glm::rotate(M9, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M9 = glm::translate(M9, glm::vec3(0.2f, 0.195f, -0.10f));
	M9 = glm::scale(M9, glm::vec3(0.1f, 0.01f, 1.60f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M9));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v5 --

	glm::mat4 M10 = glm::mat4(1.0f);
	M10 = glm::rotate(M10, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M10 = glm::translate(M10, glm::vec3(1.2f, 0.195f, 0.4f));
	M10 = glm::scale(M10, glm::vec3(0.1f, 0.01f, 1.3f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M10));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v6 --

	glm::mat4 M15 = glm::mat4(1.0f);
	M15 = glm::rotate(M15, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M15 = glm::translate(M15, glm::vec3(1.6f, 0.195f, -0.4f));
	M15 = glm::scale(M15, glm::vec3(0.1f, 0.01f, 0.5f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M15));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Droga v7 --

	glm::mat4 M16 = glm::mat4(1.0f);
	M16 = glm::rotate(M16, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M16 = glm::translate(M16, glm::vec3(-0.8f, 0.195f, -0.2f));
	M16 = glm::scale(M16, glm::vec3(0.1f, 0.01f, 1.3f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M16));
	glUniform4f(spLambert->u("color"), 0.0f, 0.5f, 0.0f, 1.0f);
	Models::cube.drawSolid();

	// Miejsce na mape

	glm::mat4 M11 = glm::mat4(1.0f);
	M11 = glm::rotate(M11, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M11 = glm::translate(M11, glm::vec3(1.5f, 0.21f, -0.5f));
	M11 = glm::scale(M11, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M11));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

	// Miejsce na mape v2

	glm::mat4 M12 = glm::mat4(1.0f);
	M12 = glm::rotate(M12, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M12 = glm::translate(M12, glm::vec3(-1.1f, 0.21f, 0.7f));
	M12 = glm::scale(M12, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M12));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

	// Miejsce na mape v3

	glm::mat4 M13 = glm::mat4(1.0f);
	M13 = glm::rotate(M13, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M13 = glm::translate(M13, glm::vec3(0.9f, 0.21f, 1.3f));
	M13 = glm::scale(M13, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M13));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

	// Miejsce na mape v4

	glm::mat4 M14 = glm::mat4(1.0f);
	M14 = glm::rotate(M14, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M14 = glm::translate(M14, glm::vec3(0.48f, 0.21f, 1.3f));
	M14 = glm::scale(M14, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M14));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

	// Miejsce na mape v5

	glm::mat4 M17 = glm::mat4(1.0f);
	M17 = glm::rotate(M17, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M17 = glm::translate(M17, glm::vec3(-0.5f, 0.21f, -1.3f));
	M17 = glm::scale(M17, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M17));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

	// Miejsce na mape v6

	glm::mat4 M18 = glm::mat4(1.0f);
	M18 = glm::rotate(M18, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M18 = glm::translate(M18, glm::vec3(-0.08f, 0.21f, -1.3f));
	M18 = glm::scale(M18, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M18));
	glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	Models::placeForTower.drawSolid();

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
