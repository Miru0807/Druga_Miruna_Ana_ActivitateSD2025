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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

//lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;

};

typedef struct Nod Nod;

//& referentiere, * deferentiere!

//metoda de inserare, la inceput, functia este inserare de lsi
void pushStack(Nod** lista, Masina masina) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*lista);
	(*lista) = nou;
}

//extragere de la inceput
Masina popStack(Nod** lista) {
	if ((*lista) == NULL) {
		return (Masina) { -1, 0, 0, NULL, NULL, '-' };
	}
	Nod* temp = (*lista);
	Masina m = temp->info;
	(*lista) = temp->next;
	free(temp);
	return m;
}


unsigned char isemptyStack(Nod* lista) {
	return lista == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while(!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&lista, m);
	}
	fclose(f);
	return lista;

}

void dezalocareStivaDeMasini(Nod** lista) {
	while ((*lista))
	{
		Masina m = popStack((*lista));
		free(m.model);
		free(m.numeSofer);
	}
}

//cate noduri sunt in stiva respectiva

int size(Nod* lista) {
	int count = 0;
	while (lista) {
		count++;
		lista = lista->next;
	}
	return count;
}

//QUEUE
//facem cu ldi
struct NodDublu {
	Masina info;
	struct NodDublu* next;
	struct NodDublu* prev;
};

typedef struct NodDublu NodDublu;

struct Coada {
	NodDublu* first;
	NodDublu* last;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->last;
	//inserare la sfarsit
	if (coada->last) {
		coada->last->next = nou;
	}
	else {
		coada->first = nou;
	}
	coada->last = nou;
}

//extragere de la inceput
Masina dequeue(Coada* coada) {
	if (coada->first) {
		Masina m = coada->first->info;
		NodDublu* temp = coada->first;
		coada->first = coada->first->next;
		free(temp);
		if (coada->first == NULL) {
			coada->last = NULL;
		}
		else {
			coada->first->prev = NULL;
		}
		return m;
	}
	else {
		Masina m;
		m.id = -1;
		return m;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.first = NULL;
	coada.last = NULL;

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->first) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}


//metode de procesare
Masina getMasinaByID_Stack(Nod** lista, int id) {
	Nod* temp = NULL;
	Masina found = { -1, 0, 0, NULL, NULL, '-' };

	while (!isemptyStack(*lista)) {
		Masina m = popStack(lista);
		if (m.id == id) {
			found = m;
		}
		pushStack(&temp, m);
	}

	// refac stiva originala
	while (!isemptyStack(temp)) {
		pushStack(lista, popStack(&temp));
	}

	return found;
}

Masina getMasinaByID_Coada(Coada* coada, int id) {
	Masina found = { -1, 0, 0, NULL, NULL, '-' };
	Coada temp;
	temp.first = NULL;
	temp.last = NULL;

	while (coada->first) {
		Masina m = dequeue(coada);
		if (m.id == id) {
			found = m;
		}
		enqueue(&temp, m);
	}
	while (temp.first) {
		enqueue(coada, dequeue(&temp));
	}

	return found;
}



float calculeazaPretTotal(Nod** stiva) {
	Nod* temp = NULL;
	float suma = 0;
	while (!isemptyStack(*stiva)) {
		Masina m = popStack(stiva);
		suma += m.pret;
		pushStack(&temp, m);
	}
	while (!isemptyStack(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return suma;
}

int main() {
	Nod* lista = citireStackMasiniDinFisier("masini.txt");

	Masina m = popStack(&lista);
	afisareMasina(m);

	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	Masina m1 = dequeue(&coada);
	afisareMasina(m1);

	printf("Total pret masini in stiva: %.2f\n", calculeazaPretTotal(&lista));

	int id_cautat = 3;
	Masina gasita_stiva = getMasinaByID_Stack(&lista, id_cautat);
	if (gasita_stiva.id != -1) {
		printf("Masina gasita in stiva (ID %d):\n", id_cautat);
		afisareMasina(gasita_stiva);
	}
	else {
		printf("Masina cu ID %d NU a fost gasita in stiva.\n", id_cautat);
	}

	id_cautat = 2;
	Masina gasita_coada = getMasinaByID_Coada(&coada, id_cautat);
	if (gasita_coada.id != -1) {
		printf("Masina gasita in coada (ID %d):\n", id_cautat);
		afisareMasina(gasita_coada);
	}
	else {
		printf("Masina cu ID %d NU a fost gasita in coada.\n", id_cautat);
	}

	dezalocareStivaDeMasini(&lista);
	dezalocareCoadaDeMasini(&coada);
	free(m.model);
	free(m.numeSofer);
	free(m1.model);
	free(m1.numeSofer);

	if (gasita_stiva.id != -1) {
		free(gasita_stiva.model);
		free(gasita_stiva.numeSofer);
	}
	if (gasita_coada.id != -1) {
		free(gasita_coada.model);
		free(gasita_coada.numeSofer);
	}

	return 0;
}

