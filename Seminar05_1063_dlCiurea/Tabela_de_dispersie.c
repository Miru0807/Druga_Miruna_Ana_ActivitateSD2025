#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>

typedef struct {
	int varsta;
	char* nume;
	float medie;
} Student;

typedef struct {
	Student inf;
	struct nodLS* next;
} nodLS;

typedef struct {
	struct nodLS** vect;
	int nrElem;
} hashT;

int functieHash1(int cheie, hashT tabela) {

	return cheie % tabela.nrElem;
}

int functieHash2(char* nume, hashT tabela) {

	return nume[0] % tabela.nrElem;
}

void inserareLS(nodLS** capLS, Student s) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->inf.varsta = s.varsta;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, s.nume);
	nou->inf.medie = s.medie;
	nou->next = NULL;
	if (*capLS == NULL) {
		*capLS = nou;
	}
	else {
		nodLS* temp = *capLS;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareHash(hashT tabela, Student s) {
	//int poz = functieHash1(s.varsta, tabela);
	int poz = functieHash2(s.nume, tabela);
	inserareLS(&tabela.vect[poz], s);
}

void traversareLS(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f", temp->inf.varsta,
			temp->inf.nume, temp->inf.medie);
		temp = temp->next;
	}
}

void traversareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			printf("\nPozitia = %d", i);//ff important sa punem acest printf ca sa stim pozitia
			traversareLS(tabela.vect[i]);
		}
	}
}

void dezalocareLS(nodLS* capLS) {
	nodLS* temp = capLS;
	while (temp) {
		nodLS* temp2 = temp->next;
		free(temp->inf.nume);
		free(temp);
		temp = temp2;
	}
}

void dezalocareHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			dezalocareLS(tabela.vect[i]);
		}

	}
	free(tabela.vect);
}


void main() {
	hashT tabela;
	tabela.nrElem = 23;
	tabela.vect = (nodLS*)malloc(tabela.nrElem * sizeof(nodLS));
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vect[i] = NULL;
	}

	Student s;
	int nrStud;
	char buffer[20];
	FILE* f = fopen("fisierText.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++) {
		fscanf(f, "%d", &s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		inserareHash(tabela, s);
		free(s.nume);
	}
	fclose(f);

	traversareHash(tabela);

	dezalocareHash(tabela);
}

// sa se determine studentul cu media cea mai mare
//pentru studentul cu varsta 23 sa se afiseze media
//studentii cu varstele 21 si 23 sa se determine media medilor
//cand sunt info desore cheia de cautare ne legam de functia hash, cand nu parcurgem general


//pentru test tot ce am facut pana in sapt 7, inclusiv conversiile