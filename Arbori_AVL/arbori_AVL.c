#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod {
	Masina* info;
	struct Nod* stg;
	struct Nod* dr;
	int gradEchilibru;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
		Masina m;
		m.id = 0;
		m.nrUsi = 0;
		m.pret = 0.0f;
		m.model = NULL;
		m.numeSofer = NULL;
		m.serie = 0;
		return m;
	}
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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

//int calculeazaInaltimeArbore(/*arbore de masini*/) {
//	//calculeaza inaltimea arborelui care este data de 
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
//	return 0;
//}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (!arbore) return 0;
	int inaltimeSt = calculeazaInaltimeArbore(arbore->stg);
	int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
	return (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr) + 1;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

void rotireStanga(Nod** arbore) {
	Nod* aux = (*arbore)->dr;
	(*arbore)->dr = aux->stg;
	aux->stg = (*arbore);
	(*arbore) = aux;
	(*arbore)->gradEchilibru--;
}

void rotireDreapta(Nod** arbore) {
	Nod* aux = (*arbore)->stg;
	(*arbore)->stg = aux->dr;
	aux->dr = (*arbore);
	(*arbore) = aux;
	(*arbore)->gradEchilibru++;
}


void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if ((*radacina) != NULL) {
		if ((*radacina)->info->id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->stg, masinaNoua);
			(*radacina)->gradEchilibru++;
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
			(*radacina)->gradEchilibru--;
		}
		if ((*radacina)->gradEchilibru == 2) {
			if ((*radacina)->stg->gradEchilibru == 1) {
				//rotire la dreapta
				rotireDreapta(radacina);
			}
			else {
				rotireStanga(&((*radacina)->stg));
				rotireDreapta(radacina);
			}
		}
		if ((*radacina)->gradEchilibru == -2) {
			//rotire stanga
			if ((*radacina)->dr->gradEchilibru == 1) {
				rotireStanga(&((*radacina)->dr));
			}
			rotireStanga(radacina);
		}
	}
	else {
		Nod* radacinaNoua = malloc(sizeof(Nod));
		radacinaNoua->info = malloc(sizeof(Masina));
		*radacinaNoua->info = masinaNoua;
		radacinaNoua->dr = NULL;
		radacinaNoua->stg = NULL;
		radacinaNoua->gradEchilibru = 0;
		(*radacina) = radacinaNoua;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model != NULL)
			adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisarePreOrdine(Nod* arbore) {
	if (arbore) {
		afisareMasina(*arbore->info);
		afisarePreOrdine(arbore->stg);
		afisarePreOrdine(arbore->dr);
	}
}

void afisareInOrdine(Nod* arbore) {
	if (arbore) {
		afisareInOrdine(arbore->stg);
		afisareMasina(*arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePostOrdine(Nod* arbore) {
	if (arbore) {
		afisarePostOrdine(arbore->stg);
		afisarePostOrdine(arbore->dr);
		afisareMasina(*arbore->info);
	}
}

void afisareMasiniDinArbore(Nod* arbore) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	printf("PREORDINE:\n");
	afisarePreOrdine(arbore);
	printf("INORDINE:\n");
	afisareInOrdine(arbore);
	printf("POSTORDINE:\n");
	afisarePostOrdine(arbore);
}

void dezalocareArboreDeMasini(Nod* arbore) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (arbore) {
		dezalocareArboreDeMasini(arbore->stg);
		dezalocareArboreDeMasini(arbore->dr);
		free(arbore->info->model);
		free(arbore->info->numeSofer);
		free(arbore->info);
		free(arbore);
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(Nod* arbore, int id) {
	if (!arbore) {
		Masina m;
		m.id = 0;
		m.nrUsi = 0;
		m.pret = 0.0f;
		m.model = NULL;
		m.numeSofer = NULL;
		m.serie = 0;
		return m;
	}
	if (arbore->info->id == id)
		return *(arbore->info);
	else if (arbore->info->id > id)
		return getMasinaByID(arbore->stg, id);
	else
		return getMasinaByID(arbore->dr, id);
}

int determinaNumarNoduri(Nod* arbore) {
	if (!arbore) return 0;
	return 1 + determinaNumarNoduri(arbore->stg) + determinaNumarNoduri(arbore->dr);
}

float calculeazaPretTotal(Nod* arbore) {
	if (!arbore) return 0;
	return arbore->info->pret + calculeazaPretTotal(arbore->stg) + calculeazaPretTotal(arbore->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (!arbore) return 0;
	float pret = 0;
	if (strcmp(arbore->info->numeSofer, numeSofer) == 0)
		pret += arbore->info->pret;
	return pret +
		calculeazaPretulMasinilorUnuiSofer(arbore->stg, numeSofer) +
		calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Nr noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Pret total: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pret masini Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	Masina m = getMasinaByID(arbore, 3);
	printf("\nMasina cu ID 3:\n");
	afisareMasina(m);

	dezalocareArboreDeMasini(arbore);
	return 0;
}
