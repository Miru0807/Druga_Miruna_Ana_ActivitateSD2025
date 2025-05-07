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

	printf("\n--- Dezalocare Arbore ---\n");
	dezalocareArboreDeMasini(rad);

	return 0;
}
