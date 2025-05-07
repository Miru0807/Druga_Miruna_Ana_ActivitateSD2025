#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
	Masina info;
	struct Nod* dr;
	struct Nod* st;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = (float)atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
	if ((*nod) != NULL) {
		if (masinaNoua.id > (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
		}
		else if (masinaNoua.id < (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
		}
	}
	else {
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului.\n");
		return NULL;
	}
	Nod* nod = NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&nod, citireMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->st);
		afisarePreOrdineRSD(radacina->dr);
	}
}

void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dr);
	}
}

void afisareMasiniDinArbore(Nod* radacina) {
	printf("\n--- Parcurgere Preordine (RSD) ---\n");
	afisarePreOrdineRSD(radacina);

	printf("\n--- Parcurgere Inordine (SRD) ---\n");
	afisareInOrdineSRD(radacina);

	printf("\n--- Parcurgere Postordine (SDR) ---\n");
	if (radacina != NULL) {
		afisareMasiniDinArbore(radacina->st);
		afisareMasiniDinArbore(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void dezalocareArboreDeMasini(Nod* radacina) {
	if (radacina != NULL) {
		dezalocareArboreDeMasini(radacina->st);
		dezalocareArboreDeMasini(radacina->dr);
		free(radacina->info.model);
		free(radacina->info.numeSofer);
		free(radacina);
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	if (radacina != NULL) {
		if (id < radacina->info.id) {
			return getMasinaByID(radacina->st, id);
		}
		else if (id > radacina->info.id) {
			return getMasinaByID(radacina->dr, id);
		}
		else {
			return radacina->info;
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	int nrNoduri = 0;
	if (radacina != NULL) {
		nrNoduri += determinaNumarNoduri(radacina->st);
		nrNoduri += determinaNumarNoduri(radacina->dr);
		nrNoduri += 1;
	}
	return nrNoduri;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	else {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	return radacina->info.pret +
		calculeazaPretTotal(radacina->st) +
		calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL)
		return 0;
	float suma = 0;
	if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
		suma += radacina->info.pret;
	}
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
	return suma;
}


// DE AICI REZOLV TASK-UL, PANA AICI ESTE CEEA CE AM LUCRAT LA SEMINAR!
//Sa se stearga un nod pentru care se primeste id-ul de cautare. Dupa stergere arborele
//trebuie sa isi pastreze proprietatea de Arbore Binar de Cautare.

Nod* stergeNod(Nod* radacina, int id) {
	if (radacina == NULL)
		return radacina;

	if (id < radacina->info.id) {
		radacina->st = stergeNod(radacina->st, id);
	}
	else if (id > radacina->info.id) {
		radacina->dr = stergeNod(radacina->dr, id);
	}
	else {
		if (radacina->st == NULL) {
			Nod* temp = radacina->dr;
			free(radacina->info.model);
			free(radacina->info.numeSofer);
			free(radacina);
			return temp;
		}
		else if (radacina->dr == NULL) {
			Nod* temp = radacina->st;
			free(radacina->info.model);
			free(radacina->info.numeSofer);
			free(radacina);
			return temp;
		}

		
		Nod* temp = radacina->dr;
		while (temp && temp->st != NULL)
			temp = temp->st;

		radacina->info = temp->info;
		radacina->dr = stergeNod(radacina->dr, temp->info.id);
	}
	return radacina;
}

//Sa se implementeze o functie care pentru o radacina primita ca parametru, returneaza
//radacina subarborelui stang sau drept care are inaltimea mai mare.

Nod* subarboreMaiInalt(Nod* radacina) {
	if (radacina == NULL)
		return NULL;

	int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
	int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

	if (inaltimeSt > inaltimeDr) {
		return radacina->st;
	}
	return radacina->dr;
}


//Sa se realizeze o functie care pentru un nod radacina primit, determina numarul de noduri aflate in subordine.

int numarNoduriInSubarbore(Nod* radacina) {
	if (radacina == NULL)
		return 0;

	return determinaNumarNoduri(radacina);
}

//Sa se realizeze o functie care pentru un nod radacina primit ca parametru, returneaza radacina subarborelui stang sau drept care are mai multe noduri in subordine.

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
	if (radacina == NULL)
		return NULL;

	int noduriSt = determinaNumarNoduri(radacina->st);
	int noduriDr = determinaNumarNoduri(radacina->dr);

	if (noduriSt > noduriDr) {
		return radacina->st;
	}
	return radacina->dr;
}




int main() {
	Nod* rad = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("\n--- Afisare PreOrdine RSD ---\n");
	afisarePreOrdineRSD(rad);

	printf("\n--- Afisare InOrdine SRD ---\n");
	afisareInOrdineSRD(rad);

	printf("\n--- Afisare Toate Cele 3 Parcurgeri ---\n");
	afisareMasiniDinArbore(rad);

	printf("\n--- Cautare Masina cu ID = 6 ---\n");
	Masina m = getMasinaByID(rad, 6);
	if (m.id != -1) {
		afisareMasina(m);
	}
	else {
		printf("Masina cu ID 6 nu a fost gasita.\n");
	}

	printf("\n--- Numar Noduri in Arbore ---\n");
	printf("Numar noduri: %d\n", determinaNumarNoduri(rad));

	printf("\n--- Inaltimea Arborelui ---\n");
	printf("Inaltime: %d\n", calculeazaInaltimeArbore(rad));

	printf("\n--- Pretul Total al Masinilor ---\n");
	printf("Pret total: %.2f\n", calculeazaPretTotal(rad));

	printf("\n--- Pretul Masinilor pentru soferul 'Ionescu' ---\n");
	printf("Pret total pentru sofer: %.2f\n", calculeazaPretulMasinilorUnuiSofer(rad, "Ionescu"));

	printf("\n--- Subarbore cu inaltimea mai mare ---\n");
	Nod* subarbore = subarboreMaiInalt(rad);
	if (subarbore != NULL) {
		afisareMasina(subarbore->info);
	}
	else {
		printf("Subarbore inexistent.\n");
	}

	printf("\n--- Numar noduri in subarbore ---\n");
	printf("Numar noduri: %d\n", numarNoduriInSubarbore(rad));

	printf("\n--- Subarbore cu mai multe noduri ---\n");
	Nod* subarboreMaiMulteNoduri = subarboreCuMaiMulteNoduri(rad);
	if (subarboreMaiMulteNoduri != NULL) {
		afisareMasina(subarboreMaiMulteNoduri->info);
	}
	else {
		printf("Subarbore inexistent.\n");
	}

	rad = stergeNod(rad, 6);
	printf("\n--- Arbore dupa stergerea nodului cu ID 6 ---\n");
	afisareMasiniDinArbore(rad);

	printf("\n--- Dezalocare Arbore ---\n");
	dezalocareArboreDeMasini(rad);

	return 0;
}
