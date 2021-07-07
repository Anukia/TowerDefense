#pragma once
#include <Mobki.h>
#include <vector>
class OgarniaczMobkow
{
public:
	std::vector<Mobki> mobki;

private:
	float czasOdOstatniegoMobka;
	std::vector<int> mobkiDoWyslania;
	std::vector<int> hpMobkow;
	float speedMobkow;
	float czasPomiedzyMobkami;
	int runda, punkty;
	float czasMiedzyRundami;
	bool czasNaNowaRunde;
	float czasOdOstatniejRundy;
	int endlessGameMobki[5] = { 0,0,0,15,20 };
	float endlessGameUstawienia[2] = { 0.5f, 1.0f};


public:

	OgarniaczMobkow()
	{
		czasOdOstatniegoMobka = 0;
		runda = 1;
		punkty = 0;
		czasMiedzyRundami = 2.0f;
		czasNaNowaRunde = true;
		czasOdOstatniejRundy = 0.0f;
	}

	void wysylanieMobkow(float time, int hp_baza, GLuint* colors)
	{
		czasOdOstatniegoMobka += time;
		if (czasOdOstatniegoMobka > czasPomiedzyMobkami)
		{
			czasOdOstatniegoMobka -= czasPomiedzyMobkami;
			Mobki mobek = Mobki(hpMobkow[0], 2.4f, 0.195f, -0.8f, speedMobkow);
			mobki.push_back(mobek);
			mobki[mobki.size() - 1].poruszanie(czasOdOstatniegoMobka, mobki, mobki.size() - 1, hp_baza, colors);
			mobkiDoWyslania[0]--;
		}
	}

	void rundax() {
		switch (runda)
		{
		case 1:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(1);
			break;

		case 2:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(1);
			mobkiDoWyslania.push_back(5);
			hpMobkow.push_back(2);
			break;

		case 3:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(5);
			hpMobkow.push_back(2);
			mobkiDoWyslania.push_back(5);
			hpMobkow.push_back(3);
			break;

		case 4:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(3);
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(2);
			break;

		case 5:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(3);
			mobkiDoWyslania.push_back(5);
			hpMobkow.push_back(4);
			break;

		case 6:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(4);
			mobkiDoWyslania.push_back(5);
			hpMobkow.push_back(5);
			break;

		case 7:
			speedMobkow = 1.0f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(1);
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(2);
			break;

		case 8:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(20);
			hpMobkow.push_back(4);
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(5);
			break;

		case 9:
			speedMobkow = 0.5f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(15);
			hpMobkow.push_back(4);
			mobkiDoWyslania.push_back(15);
			hpMobkow.push_back(5);
			break;

		case 10:
			speedMobkow = 1.0f;
			czasPomiedzyMobkami = 0.3f;
			mobkiDoWyslania.push_back(12);
			hpMobkow.push_back(3);
			mobkiDoWyslania.push_back(12);
			hpMobkow.push_back(2);
			break;

		default:

			if (runda % 5 == 1) {
				speedMobkow = endlessGameUstawienia[0];
				czasPomiedzyMobkami = endlessGameUstawienia[1];
			}
			else if (runda % 5 == 2) {
				speedMobkow = endlessGameUstawienia[0] + 0.03f;
				czasPomiedzyMobkami = endlessGameUstawienia[1] - 0.02f;
			}
			else if (runda % 5 == 3) {
				speedMobkow = endlessGameUstawienia[0] + 0.06f;
				czasPomiedzyMobkami = endlessGameUstawienia[1] - 0.04f;
			}
			else if (runda % 5 == 4) {
				speedMobkow = endlessGameUstawienia[0] + 0.09f;
				czasPomiedzyMobkami = endlessGameUstawienia[1] - 0.06f;
			}
			else {
				speedMobkow = endlessGameUstawienia[0] + 0.12f;
				czasPomiedzyMobkami = endlessGameUstawienia[1] - 0.08f;
			}

			for (int i = 0; i <= 4; i++) {

				mobkiDoWyslania.push_back(endlessGameMobki[i]);
				hpMobkow.push_back(i + 1);
				if (runda % 3 == 1 && i >= 2) {
					endlessGameMobki[i]++;
				}
				if (runda % 3 == 2 && i >= 3) {
					endlessGameMobki[i]++;
				}
				if (runda % 3 == 0 && i >= 4) {
					endlessGameMobki[i]++;
				}

			}

			endlessGameUstawienia[0] += 0.04f;
			endlessGameUstawienia[0] -= 0.03f;

			break;
		}
	}

	void robiSwoje(float time, int& hp_baza, int& zloto, GLuint * colors)
	{
		if (czasNaNowaRunde)
		{
			rundax();
			runda++;
			czasNaNowaRunde = false;
			czasOdOstatniejRundy = 0.0f;
		}
		if (mobkiDoWyslania.size() > 0 && mobkiDoWyslania[0] > 0)
		{
			wysylanieMobkow(time, hp_baza, colors);
		}
		else if (mobkiDoWyslania.size() > 0)
		{
			mobkiDoWyslania.erase(mobkiDoWyslania.begin());
			hpMobkow.erase(hpMobkow.begin());
		}
		else if (mobki.size() == 0) {
			czasOdOstatniejRundy += time;
			if (czasOdOstatniejRundy > czasMiedzyRundami)
			{
				czasNaNowaRunde = true;
			}
		}
		for (int i = 0; i < mobki.size(); i++) {
			if (hp_baza <= 0) time = 0;
			mobki[i].poruszanie(time, mobki, i, hp_baza, colors);
			smierc(zloto);
		}
	}

	void smierc(int& zloto) {
		for (int i = 0; i < mobki.size(); i++) {
			if (mobki[i].umarl == true) {
				zloto += mobki[i].poziom * 2 + 1;
				punkty += mobki[i].getPunkty();
				mobki.erase(mobki.begin() + i);
				i--;
			}
		}
	}
	int getRunda() {
		return runda;
	}
	int getPunkty() {
		return punkty;
	}
};
