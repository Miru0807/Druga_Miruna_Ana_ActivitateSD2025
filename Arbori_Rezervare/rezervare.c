#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Am incercat sa fac aceasta structura dupa tutorialul cu arbore
typedef struct Rezervare {
	unsigned int idRezervare;
	char* denumireHotel;
	unsigned char numarCamere;
	char* numeClient;
	float pret;
}Rezervare;

typedef struct Arbore {
	Rezervare rezervare;
	struct Arbore* stanga, * dreapta;
}Arbore;

Arbore* inserareArbore(Arbore* r, Rezervare rezervare) {
	if (r) {
		if (r->rezervare.idRezervare < rezervare.idRezervare) {
			r->dreapta = inserareArbore(r->dreapta, rezervare);
		}
		else {
			r->stanga = inserareArbore(r->stanga, rezervare);
		}
		return r;
	}
	else {
		Arbore* nou = (Arbore*)malloc(sizeof(Arbore) * 1);
		nou->rezervare = rezervare;
		nou->stanga = nou->dreapta = NULL;

		return nou;
	}
}

void parcurgereInordine(Arbore* r) {
	if (r) {
		parcurgereInordine(r->stanga);
		printf("%d %s\n", r->rezervare.idRezervare, r->rezervare.numeClient);
		parcurgereInordine(r->dreapta);
	}
}
 
//logica - ma duc maxim pe stanga pana gasesc null - ala va fi minimul de cheie posibil in arbore pe logica de insert
Arbore* stergere_minim(Arbore* r) {
	Arbore* copie = r;
	//parintele este important pentru restabilirea legaturilor in arbore !!!!
	Arbore* parinte = NULL;
	while (copie->stanga != NULL) {
		parinte = copie;
		copie = copie->stanga;
	}

	//cazuri de stergere ! 
	//1. nodul cu cheie minima este chiar radacina arborelui
	if (copie == r) {
		//inlocuiesc root-ul cu subarborele drept complet
		r = r->dreapta;

		free(copie->rezervare.denumireHotel);
		free(copie->rezervare.numeClient);
		free(copie);
		copie = NULL;
	}
	else {
		 
		if (copie->stanga == NULL && copie->dreapta == NULL) {
			
			parinte->stanga = NULL;

			free(copie->rezervare.denumireHotel);
			free(copie->rezervare.numeClient);
			free(copie);
			copie = NULL;
		}
		else {
			if (copie->dreapta != NULL) {
				parinte->stanga = copie->dreapta;
				free(copie->rezervare.numeClient);
				free(copie->rezervare.denumireHotel);
				free(copie);
				copie = NULL;
			}
		}
	}

	return r;
}

// vector cu numarul de noduri pe fiecare nivel -----> deci un int*
int maxim(int x, int y) {
	return (x > y) ? x : y;
}

int inaltime(Arbore* root) {
	if (root == NULL) {
		return 0;
	}
	else {
		return 1 + max(inaltime(root->stanga), inaltime(root->dreapta));
	}
}

int determinare_noduri_nivel(Arbore* root, int nivel) {
	if (root == NULL) {
		return 0;
	}
	if (nivel == 0) {
		return 1;
	}
	//calculez recursiv, nu nivel - 1 ajung pe cazurile de mai sus unde se aduna efectiv  0 + 1 noduri in functie de arbore
	return determinare_noduri_nivel(root->stanga, nivel - 1) + determinare_noduri_nivel(root->dreapta, nivel - 1);
}

int* vector_numar_noduri_nivel(Arbore* root, int* dim) {
	*dim = inaltime(root);
	printf("\nChecking value = %d levels\n", *dim);
	//aloc spatiu 
	int* vector = (int*)malloc(sizeof(int) * *dim);

	//unde contor tine locul de nivelul pentru care calculez si de asemenea index pentru vectorul 	
	int contor = 0;
	while (contor < *dim) {
		vector[contor] = determinare_noduri_nivel(root, contor);
		contor++;
	}
	return vector;
}

void main() {
	Arbore* r = NULL;
	Rezervare rezervare;

	FILE* f;
	f = fopen("Data.txt", "r");
	char* token, buffer[128], sep_list[] = ",\n";
	while (fgets(buffer, sizeof(buffer), f)) {

		token = strtok(buffer, sep_list);
		rezervare.idRezervare = atoi(token);

		token = strtok(NULL, sep_list);
		rezervare.denumireHotel = (char*)malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(rezervare.denumireHotel, token);

		token = strtok(NULL, sep_list);
		rezervare.numarCamere = atoi(token);

		token = strtok(NULL, sep_list);
		rezervare.numeClient = (char*)malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(rezervare.numeClient, token);

		token = strtok(NULL, sep_list);
		rezervare.pret = atof(token);

		/*token = strtok(NULL, sep_list);
		if (token)
			printf("\nEroare preluare date!\n");*/

		r = inserareArbore(r, rezervare);

	}
	fclose(f);

	parcurgereInordine(r);

	printf("\n Stergere minim din structura arbore............\n");
	r = stergere_minim(r);
	parcurgereInordine(r);

	int dim = 0;
	int* vector_noduri = vector_numar_noduri_nivel(r, &dim);
	
	printf("\n Afisare vector cu numarul de noduri din structura  / nivel.........\n");
	for (int i = 0; i < dim; i++) {
		printf("\n Nivel [%d] = [%d] noduri", i, vector_noduri[i]);
	}
}