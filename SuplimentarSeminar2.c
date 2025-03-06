#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
	int id;
	int pagini;
	char* titlu;
	char* autor;
	float pret;
	char* categorie;
};

struct Carte initializare(int id, int pagini, const char* titlu, const char* autor, float pret, const char* categorie) {
	struct Carte c;
	c.id = id;
	c.pagini = pagini;
	c.titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy_s(c.titlu, strlen(titlu) + 1, titlu);
	c.autor = (char*)malloc(strlen(autor) + 1);
	strcpy_s(c.autor, strlen(autor) + 1, autor);
	c.pret = pret;
	c.categorie = (char*)malloc(strlen(categorie) + 1);
	strcpy_s(c.categorie, strlen(categorie) + 1, categorie);
	return c;
}


void afisare(struct Carte c) {
	printf("ID: %d\n", c.id);
	printf("NrPagini: %d\n", c.pagini);
	printf("Titlu: %s\n", c.titlu);
	printf("Autor: %s\n", c.autor);
	printf("Pret: %.2f\n", c.pret);
	printf("Categorie: %s\n\n", c.categorie);
}

void afisareVector(struct Carte* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Carte* copiazaPrimeleNElemente(struct Carte* vector, int nrElemente, int nrElementeCopiate) {
	struct Carte* vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = initializare(vector[i].id, vector[i].pagini, vector[i].titlu, vector[i].autor, vector[i].pret, vector[i].categorie);
	}
	return vectorNou;
}

void dezalocare(struct Carte** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].titlu);
		free((*vector)[i].autor);
		free((*vector)[i].categorie);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

struct Carte getPrimulElementConditionat(struct Carte* vector, int nrElemente, const char* titluCautat) {
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].titlu, titluCautat) == 0) {
			return vector[i];
		}
	}
	struct Carte empty = { 0, 0, NULL, NULL, 0.0, NULL };
	return empty;
}

int main() {
	int nrElemente = 3;
	struct Carte* bibliotecă = (struct Carte*)malloc(sizeof(struct Carte) * nrElemente);

	bibliotecă[0] = initializare(1, 200, "Clean Code", "Robert C. Martin", 50.0, "Programming");
	bibliotecă[1] = initializare(2, 350, "The Pragmatic Programmer", "Andrew Hunt", 45.0, "Software Development");
	bibliotecă[2] = initializare(3, 500, "Design Patterns", "Erich Gamma", 60.0, "Computer Science");

	printf("Cartile din biblioteca:\n");
	afisareVector(bibliotecă, nrElemente);

	// Copiere primele 2 elemente
	int nrCopiate = 2;
	struct Carte* cartiCopiate = copiazaPrimeleNElemente(bibliotecă, nrElemente, nrCopiate);

	printf("\nCarti copiate:\n");
	afisareVector(cartiCopiate, nrCopiate);

	// Cautare carte
	const char* titluCautat = "Clean Code";
	struct Carte gasita = getPrimulElementConditionat(bibliotecă, nrElemente, titluCautat);

	if (gasita.titlu != NULL) {
		printf("\nCarte gasita:\n");
		afisare(gasita);
	}
	else {
		printf("\nCarte cu titlul '%s' nu a fost gasita.\n", titluCautat);
	}

	// Dezalocare memorie
	dezalocare(&bibliotecă, &nrElemente);
	dezalocare(&cartiCopiate, &nrCopiate);

	return 0;
}
