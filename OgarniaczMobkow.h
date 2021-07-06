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
	int runda;
	float czasMiedzyRundami;
	bool czasNaNowaRunde;
	float czasOdOstatniejRundy;


public:

	OgarniaczMobkow()
	{
		czasOdOstatniegoMobka = 0;
		runda = 1;
		czasMiedzyRundami = 2.0f;
		czasNaNowaRunde = true;
		czasOdOstatniejRundy = 0.0f;
	}

	void wysylanieMobkow(float time, int hp_baza)
	{
		czasOdOstatniegoMobka += time;
		if (czasOdOstatniegoMobka > czasPomiedzyMobkami)
		{
			czasOdOstatniegoMobka -= czasPomiedzyMobkami;
			Mobki mobek = Mobki(hpMobkow[0], 2.4f, 0.195f, -0.8f, speedMobkow);
			mobki.push_back(mobek);
			mobki[mobki.size() - 1].poruszanie(czasOdOstatniegoMobka, mobki, mobki.size() - 1, hp_baza);
			mobkiDoWyslania[0]--;
		}
	}

	void rundax() {
		switch (runda)
		{
		case 1:
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(1);
			speedMobkow = 0.3f;
			czasPomiedzyMobkami = 1.0f;
			break;

		case 2:
			speedMobkow = 0.3f;
			czasPomiedzyMobkami = 1.0f;
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(1);
			mobkiDoWyslania.push_back(10);
			hpMobkow.push_back(2);
			break;

		default:
			mobkiDoWyslania.push_back(0);
			hpMobkow.push_back(1);
			speedMobkow = 0.15f;
			czasPomiedzyMobkami = 0.6f;
			break;
		}
	}

	void robiSwoje(float time, int& hp_baza, int& zloto)
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
			wysylanieMobkow(time, hp_baza);
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
			mobki[i].poruszanie(time, mobki, i, hp_baza);
			smierc(zloto);
		}
	}

	void smierc(int& zloto) {
		for (int i = 0; i < mobki.size(); i++) {
			if (mobki[i].umarl == true) {
				zloto += mobki[i].poziom * 15;
				mobki.erase(mobki.begin() + i);
				i--;
			}
		}
	}
};
