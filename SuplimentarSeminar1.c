#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char cod_produs;
	float greutate;
};



struct Produs initializare(int id, int stoc, char* denumire, float pret, char cod_produs) {
	struct Produs p;
	p.id = id;
	p.stoc = stoc;
	p.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy_s(p.denumire, strlen(denumire) + 1, denumire);
	p.pret = pret;
	p.cod_produs = cod_produs;
	return p;
}

void afisare(struct Produs p) {
	printf("ID: %d \n", p.id);
	printf("Stoc: %d \n", p.stoc);
	printf("Denumire: %s \n", p.denumire);
	printf("Pret : %.2f \n", p.pret);
	printf("Cod produs: %c \n", p.cod_produs);
}

void modifica_Denumire(struct Produs* p, const char* nouDenumire) {
	if (strlen(nouDenumire) > 10) {
		if (p->denumire != NULL) {
			free(p->denumire);
		}
		p->denumire = (char*)malloc(strlen(nouDenumire) + 1);
		strcpy_s(p->denumire, strlen(nouDenumire) + 1, nouDenumire);
	}
}

void dezalocare(struct Produs* p) {
	//dezalocare campuri alocate dinamic
	if (p->denumire != NULL) {
		free(p->denumire);
		p->denumire = NULL;
	}
}

int main() {
	struct Produs p;
	p = initializare(1, 120, "Lapte", 2000, 'P');
	p = initializare(2, 100, "Corn", 2000, 'P');
	p = initializare(3, 180, "Paine", 2000, 'P');
	p = initializare(4, 200, "Biscuite", 2000, 'P');
	p = initializare(5, 150, "Apa", 2000, 'P');
	afisare(p);
	modifica_Denumire(&p, "Lapte");
	afisare(p);
	dezalocare(&p);
	afisare(p);
	return 0;
}