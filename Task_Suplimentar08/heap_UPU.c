//Trebuie sa fac Max-Heap!!!

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraPacient {
	int id;
	int gradUrgenta;
	char* nume;
	char* diagnostic;
};
typedef struct StructuraPacient Pacient;

struct Heap {
	Pacient* vector;
	int nrPacienti;
	int capacitate;
};
typedef struct Heap Heap;

Pacient citirePacientDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	Pacient p;
	char* aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.gradUrgenta = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy(p.nume, aux);

	aux = strtok(NULL, sep);
	p.diagnostic = malloc(strlen(aux) + 1);
	strcpy(p.diagnostic, aux);

	return p;
}

void afisarePacient(Pacient p) {
	printf("ID: %d\nGrad urgenta: %d\nNume: %s\nDiagnostic: %s\n\n", p.id, p.gradUrgenta, p.nume, p.diagnostic);
}

Heap initializareHeap(int capacitate) {
	Heap h;
	h.capacitate = capacitate;
	h.nrPacienti = 0;
	h.vector = (Pacient*)malloc(sizeof(Pacient) * capacitate);
	return h;
}

void filtreazaHeap(Heap heap, int poz) {
	int st = 2 * poz + 1;
	int dr = 2 * poz + 2;
	int max = poz;

	if (st < heap.nrPacienti && heap.vector[st].gradUrgenta > heap.vector[max].gradUrgenta) {
		max = st;
	}
	if (dr < heap.nrPacienti && heap.vector[dr].gradUrgenta > heap.vector[max].gradUrgenta) {
		max = dr;
	}
	if (max != poz) {
		Pacient aux = heap.vector[poz];
		heap.vector[poz] = heap.vector[max];
		heap.vector[max] = aux;

		filtreazaHeap(heap, max);
	}
}

Heap citireHeapPacienti(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(20);
	while (!feof(f)) {
		h.vector[h.nrPacienti++] = citirePacientDinFisier(f);
	}
	fclose(f);

	for (int i = (h.nrPacienti - 2) / 2; i >= 0; i--) {
		filtreazaHeap(h, i);
	}
	return h;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPacienti; i++) {
		afisarePacient(heap.vector[i]);
	}
}

Pacient extragePacient(Heap* heap) {
	if (heap->nrPacienti > 0) {
		Pacient p = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrPacienti - 1];
		heap->nrPacienti--;

		for (int i = (heap->nrPacienti - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return p;
	}
	Pacient gol;
	gol.nume = NULL;
	gol.diagnostic = NULL;
	return gol;
}

struct Nod {
	Pacient info;
	struct Nod* next;
};
typedef struct Nod Nod;

void inserareInLista(Nod** lista, Pacient p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = *lista;
	*lista = nou;
}

void afisareLista(Nod* lista) {
	while (lista) {
		afisarePacient(lista->info);
		lista = lista->next;
	}
}

Nod* pacientiUrgenti(Heap* h, int pragUrgenta) {
	Nod* lista = NULL;
	while (h->nrPacienti > 0 && h->vector[0].gradUrgenta >= pragUrgenta) {
		Pacient p = extragePacient(h);
		inserareInLista(&lista, p);
	}
	return lista;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrPacienti; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].diagnostic);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrPacienti = 0;
	heap->capacitate = 0;
}

int main() {
	Heap h = citireHeapPacienti("pacienti.txt");
	printf("Pacientii din heap:\n");
	afisareHeap(h);

	Nod* listaUrgenta = pacientiUrgenti(&h, 7);
	printf("\nPacientii cu grad de urgenta mare:\n");
	afisareLista(listaUrgenta);

	dezalocareHeap(&h);
	while (listaUrgenta) {
		Nod* aux = listaUrgenta;
		listaUrgenta = listaUrgenta->next;
		free(aux);
	}

	return 0;
}
