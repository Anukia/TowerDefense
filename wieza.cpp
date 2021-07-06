#include "wieza.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shaderprogram.h"
#include "allmodels.h"

Wieza::Wieza() {

}

Wieza::Wieza(float hx, float hy, float hz, float hmiejsceXScale, float hmiejsceYScale, float hmiejsceZScale,
	float hwiezaXScale, float hwiezaYScale, float hwiezaZScale, int hid,
	GLuint hmetal, GLuint hchosen, GLuint htower) {

	x = hx;
	y = hy;
	z = hz;
	miejsceXScale = hmiejsceXScale;
	miejsceYScale = hmiejsceYScale;
	miejsceZScale = hmiejsceZScale;
	wiezaXScale = hwiezaXScale;
	wiezaYScale = hwiezaYScale;
	wiezaZScale = hwiezaZScale;
	kupionaWieza = false;
	id = hid;
	metal = hmetal;
	chosen = hchosen;
	tower = htower;

}

void Wieza::rysuj(int wybranaWieza) {

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::scale(M, glm::vec3(miejsceXScale, miejsceYScale, miejsceZScale));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::placeForTower.vertices);
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::placeForTower.normals);
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::placeForTower.texCoords);
	glActiveTexture(GL_TEXTURE0);

	//glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
	//Models::placeForTower.drawSolid();

	if (wybranaWieza == id) glBindTexture(GL_TEXTURE_2D, chosen);
	else glBindTexture(GL_TEXTURE_2D, metal);

	glUniform1i(spLambertTextured->u("tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, Models::placeForTower.vertexCount);
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));

	if (kupionaWieza == true) {
		glm::mat4 MWieza = glm::mat4(1.0f);
		MWieza = glm::rotate(MWieza, 0.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		MWieza = glm::translate(MWieza, glm::vec3(x, y, z));
		MWieza = glm::scale(MWieza, glm::vec3(wiezaXScale, wiezaYScale, wiezaZScale));
		glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(MWieza));

		glEnableVertexAttribArray(spLambertTextured->a("vertex"));
		glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::woodenTowerWatch.vertices);
		glEnableVertexAttribArray(spLambertTextured->a("normal"));
		glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Models::woodenTowerWatch.normals);
		glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
		glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::woodenTowerWatch.texCoords);
		glActiveTexture(GL_TEXTURE0);

		//glUniform4f(spLambert->u("color"), 0.3f, 0.5f, 0.7f, 1.0f);
		//Models::placeForTower.drawSolid();

		//if (wybranaWieza == id) glBindTexture(GL_TEXTURE_2D, tower);
		//else glBindTexture(GL_TEXTURE_2D, tower);
		glBindTexture(GL_TEXTURE_2D, tower);

		glUniform1i(spLambertTextured->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, Models::woodenTowerWatch.vertexCount);
		glDisableVertexAttribArray(spLambertTextured->a("vertex"));
		glDisableVertexAttribArray(spLambertTextured->a("normal"));
		glDisableVertexAttribArray(spLambertTextured->a("texCoord"));

	}

}
int Wieza::getterKosztUlepszeniaZasieg(){
	return kosztUlepszeniaZasieg[idUlepszenieZasieg];
}


void Wieza::setterIdUlepszeniaZasieg() {
	if(idUlepszenieZasieg < 5) idUlepszenieZasieg++;
}

int Wieza::getterKosztUlepszeniaObrazen() {
	return kosztUlepszeniaObrazen[idUlepszenieObrazen];
}

void Wieza::setterIdUlepszeniaObrazen() {
	if (idUlepszenieObrazen < 5) idUlepszenieObrazen++;
}

int Wieza::getterIdZasieg() {
	return idUlepszenieZasieg;
}

int Wieza::getterIdObrazen() {
	return idUlepszenieObrazen;
}

int Wieza::getterKosztWiezy() {
	return kosztWiezy;
}

void Wieza::setterKupionaWieza() {
	kupionaWieza = true;
}

bool Wieza::getterKupionaWieza() {
	return kupionaWieza;
}
int Wieza::getterZasieg() {
	return zasieg[idUlepszenieZasieg];
}

int Wieza::getterObrazenia() {
	return obrazenia[idUlepszenieZasieg];
}


void Wieza::strzelanie(OgarniaczMobkow &ogarniacz, float czas) {
	bool znaleziony = false;
	int indeksCelu;
	czasOdStrzalu += czas;
	if (czasOdStrzalu > czasStrzelania) {
		czasOdStrzalu -= czasStrzelania;
		for (int i = 0; i < ogarniacz.mobki.size(); i++) {
			if (sprawdzZasieg(ogarniacz.mobki[i].getx(), ogarniacz.mobki[i].getz())) {
				indeksCelu = i;
				znaleziony = true;
				break;
			}
		}
		if (znaleziony) {
			ogarniacz.mobki[indeksCelu].odejmij_zycie(obrazenia[idUlepszenieObrazen]);
		}
	}
}

bool Wieza::sprawdzZasieg(float mobekX, float mobekZ) {
	float lewaStrona = (x - mobekX) * (x - mobekX) + (y - mobekZ) * (y - mobekZ);
	float prawaStrona = zasieg[idUlepszenieZasieg] * zasieg[idUlepszenieZasieg];
	if (lewaStrona <= prawaStrona) {
		return true;
	}
	return false;
}