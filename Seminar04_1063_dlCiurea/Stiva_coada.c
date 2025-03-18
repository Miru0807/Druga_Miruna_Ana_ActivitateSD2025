#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdio.h> //treb neaparat pt FILE* f!!!
#include<stdlib.h>

typedef struct {
	int zi, luna, an; //4b+4b+4b
}dataReceptionare;


typedef struct {
	dataReceptionare data;  //12B
	char* denumireAplicatie; //4B
	float durata; //4B
}notificare;

typedef struct {
	notificare info; //20B
	struct nodStiva* next; //4B
}nodStiva;
 
typedef struct {
	notificare info; 
	struct nodCoada* next; 
}nodCoada;




//functia de inserare nod in stiva

void push(nodStiva** varf, notificare notif) {
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	//aici am facut deep copy
	nou->info.data.zi = notif.data.zi;
	nou->info.data.luna = notif.data.luna;
	nou->info.data.an = notif.data.an;
	nou->info.denumireAplicatie = (char*)malloc((strlen(notif.denumireAplicatie) + 1) * sizeof(char));
	strcpy(nou->info.denumireAplicatie, notif.denumireAplicatie);
	nou->info.durata = notif.durata;
	nou->next = NULL;
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

//functie pt extragere nod din stiva
//pop = extragem nodul, adica stergere

int pop(nodStiva** varf, notificare* notif) {
	if (*varf == NULL) 
		return -1;
	else
	{
		(*notif).data.zi = (*varf)->info.data.zi;
		(*notif).data.luna = (*varf)->info.data.luna;
		(*notif).data.an = (*varf)->info.data.an;
		(*notif).denumireAplicatie = (char*)malloc((strlen((*varf)->info.denumireAplicatie) + 1) * sizeof(char));
		strcpy((*notif).denumireAplicatie, (*varf)->info.denumireAplicatie);
		(*notif).durata = (*varf)->info.durata;
		nodStiva* temp = *varf; //nodul care treb sters
		*varf = (*varf)->next;
		//sterg tempul, adica nodul vechi
		free(temp->info.denumireAplicatie);
		free(temp); //spatiul de 24B
		return 0;
	}
}

void afisareStiva(nodStiva* varf) {
	nodStiva* temp = varf;
	while (temp) {
		printf("\nData:%d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f",
		temp->info.data.zi, temp->info.data.luna, temp->info.data.an,
		temp->info.denumireAplicatie, temp->info.durata);
	temp = temp->next;
	}
}

void dezalocareStiva(nodStiva* varf) {
	notificare notif;
	while (pop(&varf, &notif) == 0)
		free(notif.denumireAplicatie);
}

//functia de inserare nod in coada
void put(nodCoada** prim, nodCoada** ultim, notificare notif) {
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	//aici am facut deep copy
	nou->info.data.zi = notif.data.zi;
	nou->info.data.luna = notif.data.luna;
	nou->info.data.an = notif.data.an;
	nou->info.denumireAplicatie = (char*)malloc((strlen(notif.denumireAplicatie) + 1) * sizeof(char));
	strcpy(nou->info.denumireAplicatie, notif.denumireAplicatie);
	nou->info.durata = notif.durata;
	nou->next = NULL;
	if (*prim == NULL || *ultim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

//functie pentru extragere nod din coada

int get(nodCoada** prim, nodCoada** ultim, notificare* notif) {
	if (*prim != NULL && *ultim != NULL) {
		(*notif).data.zi = (*prim)->info.data.zi;
		(*notif).data.luna = (*prim)->info.data.luna;
		(*notif).data.an = (*prim)->info.data.an;
		(*notif).denumireAplicatie = (char*)malloc((strlen((*prim)->info.denumireAplicatie) + 1) * sizeof(char));
		strcpy((*notif).denumireAplicatie, (*prim)->info.denumireAplicatie);
		(*notif).durata = (*prim)->info.durata;
		nodCoada* temp = *prim; //nodul care treb sters
		*prim = (*prim)->next;
		//sterg tempul, adica nodul vechi
		free(temp->info.denumireAplicatie);
		free(temp); //spatiul de 24B
		return 0;
	}

	if (*prim == NULL) {
		*ultim = NULL;
		return -1;
	}
}

void afisareCoada(nodCoada* prim) {
	nodCoada* temp = prim;
	while (temp) {
		printf("\nData:%d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f",
			temp->info.data.zi, temp->info.data.luna, temp->info.data.an,
			temp->info.denumireAplicatie, temp->info.durata);
		temp = temp->next;
	}
}

void dezalocareCoada(nodCoada* prim, nodCoada* ultim) {
	notificare notif;
	while (get(&prim, &ultim, &notif) == 0)
		free(notif.denumireAplicatie);
}

//functie de conversie

void conversieStivaCoada(nodStiva* varf, nodCoada** prim, nodCoada** ultim) {
	notificare notif;
	while (pop(&varf, &notif) == 0)
		put(prim, ultim, notif);
}

void determinareNotificariDurata(nodCoada* prim, nodCoada* ultim, notificare* vect, int* nr, float prag) {
	notificare notif;
	while (get(&prim, &ultim, &notif) == 0) {
		if (notif.durata >= prag) {
			vect[*nr] = notif;
			(*nr)++;
		}
	}
}

void main() {
	int nrNotif;
	nodStiva* varf = NULL;
	nodCoada* prim = NULL, *ultim = NULL;
	notificare notif;
	char buffer[30];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrNotif);
	for (int i = 0; i < nrNotif;i++) {
		fscanf(f, "%d", &notif.data.zi);
		fscanf(f, "%d", &notif.data.luna);
		fscanf(f, "%d", &notif.data.an);
		fscanf(f, "%s", buffer);
		notif.denumireAplicatie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(notif.denumireAplicatie, buffer);
		fscanf(f, "%f", &notif.durata);
		//inserare stiva
		push(&varf, notif);
		//inserare coada
		//put(&prim, &ultim, notif);
		free(notif.denumireAplicatie);


	}
	fclose(f);
	print("\n---------------Stiva----------------\n");
	//afisareStiva(varf);
	dezalocareStiva(varf);
	print("\n---------------Coada----------------\n");
	conversieStivaCoada(varf, &prim, &ultim);
	afisareCoada(prim);
	//dezalocareCoada(prim, ultim);
	print("\n---------------Vector----------------\n");

	notificare* vect = (notificare*)malloc(nrNotif * sizeof(notificare));
	int nr = 0;
	determinareNotificariDurata(prim, ultim, vect, &nr, 2);
	for (int i = 0 ; i < nr; i++) {
		printf("\nData:%d/%d/%d, Denumire aplicatie: %s, Durata: %5.2f",
			vect[i].data.zi, vect[i].data.luna, vect[i].data.an,
			vect[i].denumireAplicatie, vect[i].durata);
		for (int i = 0; i < nr;i++) {
			free(vect);
		}
	}
}

