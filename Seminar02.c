#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	char* model;
	float tonaj;
	char serie;
};
struct Masina initializare(int id, const char* model, float tonaj, char serie) {
	struct Masina m;
	m.id = id;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy_s(m.model, strlen(model) + 1, model);
	m.tonaj = tonaj;
	m.serie = serie;

	return m;
}

void afisare(struct Masina m) {
	printf("ID: %d\n", m.id);
	printf("Model: %s\n", m.model);
	printf("Tonaj: %.2f\n", m.tonaj);
	printf("Serie: %c\n", m.serie);
}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
	}

}

//folosim strlen doar cand avem char*

//facem shallow copy pentru primitive si deep copy pentru pointeri

struct Masina* copiazaPrimeleNMasini(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	struct Masina* vectorNou = malloc(sizeof(struct Masina*) * nrElementeCopiate);

	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = vector[i];
		vectorNou[i].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
		strcpy_s(vectorNou[i].model, strlen(vector[i].model) + 1, vector[i].model);
	}

	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++)
	{
		free((*vector)[i].model);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

void copiazaMasiniUsoare(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	(*dimensiune) = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].tonaj < prag)
		{
			(*dimensiune)++;
		}
	}

	(*vectorNou) = malloc(sizeof(struct Masina) * (*dimensiune));

	int k = 0;

	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].tonaj < prag)
		{
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[k].model, strlen(vector[i].model) + 1, vector[i].model);
			k++;
		}
	}

}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	//ramane ca tema
	for (int i = 0; i < nrElemente; i++) {
		if (strstr(vector[i].model, conditie)) { // verifică dacă modelul conține condiția
			struct Masina rezultat;
			rezultat.id = vector[i].id;
			rezultat.tonaj = vector[i].tonaj;
			rezultat.serie = vector[i].serie;
			rezultat.model = malloc(strlen(vector[i].model) + 1);
			strcpy_s(rezultat.model, strlen(vector[i].model) + 1, vector[i].model);
			return rezultat;
		}

	}

	struct Masina rezultat;
	rezultat.id = -1;
	rezultat.tonaj = 0;
	rezultat.serie = '\0';
	rezultat.model = malloc(1);
	rezultat.model[0] = '\0';
	return rezultat;
}



int main() {
	int nrMasini = 3;
	int nrMasiniCopiate = 2;
	struct Masina masina = initializare(1, "Renault", 40.5, 'C');
	afisare(masina);

	struct Masina* vectorMasina; //mi am declarat un pointer
	//trebuie sa alocam spatiu acum
	vectorMasina = malloc(sizeof(struct Masina) * nrMasini);

	vectorMasina[0] = initializare(2, "Audi", 51.34, 'S');
	vectorMasina[1] = initializare(3, "Peugeot", 70, 'G');
	vectorMasina[2] = initializare(4, "Volkswagen", 35, 'H');

	printf("--------------------------\n");

	afisareVector(vectorMasina, nrMasini);

	struct Masina* vectorMasiniCopiate;
	vectorMasiniCopiate = NULL;
	vectorMasiniCopiate = copiazaPrimeleNMasini(vectorMasina, nrMasini, nrMasiniCopiate);
	printf("-------------MASINI COPIATE---------\n");
	afisareVector(vectorMasiniCopiate, nrMasiniCopiate);

	dezalocare(&vectorMasiniCopiate, &nrMasiniCopiate);

	printf("----------MASINI USOARE----------\n");
	copiazaMasiniUsoare(vectorMasina, nrMasini, 60, &vectorMasiniCopiate, &nrMasiniCopiate);


	return 0;
}