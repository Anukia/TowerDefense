#pragma once
#include <Mobki.h>
#include <vector>
class OgarniaczMobkow
{
	std::vector<Mobki> mobki;
	float czasOdOstatniegoMobka;
	int mobkiDoWyslania;
	int hpMobkow;
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
			Mobki mobek = Mobki(hpMobkow, 2.4f, 0.195f, -0.8f, speedMobkow);
			mobki.push_back(mobek);
			mobki[mobki.size() - 1].poruszanie(czasOdOstatniegoMobka, mobki, mobki.size() - 1, hp_baza);
			mobkiDoWyslania--;
		}
	}

	void rundax() {
		switch (runda)
		{
		case 1:
			mobkiDoWyslania = 30;
			hpMobkow = 1;
			speedMobkow = 0.3f;
			czasPomiedzyMobkami = 1.0f;
			break;
		default:
			mobkiDoWyslania = 0;
			hpMobkow = 1;
			speedMobkow = 0.15f;
			czasPomiedzyMobkami = 0.6f;
			break;
		}
	}

	void robiSwoje(float time, int &hp_baza)
	{
		if (czasNaNowaRunde)
		{
			rundax();
			runda++;
			czasNaNowaRunde = false;
			czasOdOstatniejRundy = 0.0f;
		}
		if (mobkiDoWyslania > 0)
		{
			wysylanieMobkow(time, hp_baza);
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
		}
	}
};

