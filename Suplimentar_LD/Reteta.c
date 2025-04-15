#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura de reteta
typedef struct Reteta {
	unsigned int nb_reteta;
	unsigned char nb_medicamente;
	char** lista_denumiri_medicamente;
	unsigned char* cantitati_medicamente;
	float* preturi_medicamente;
	unsigned char* procente_compensare;
	char* medic;
} Reteta;

typedef struct Nod {
	struct Nod* next, * prev;
	Reteta reteta;
} Nod;

typedef struct ListaDubla {
	Nod* primul, * ultimul;
} ListaDubla;

Nod* create_nod(Reteta reteta) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = nou->prev = NULL;
	nou->reteta = reteta;
	return nou;
}

ListaDubla inserare_lista(ListaDubla lst, Reteta reteta) {
	if (!lst.primul) {
		lst.primul = lst.ultimul = create_nod(reteta);
	}
	else {
		Nod* aux = create_nod(reteta);
		aux->prev = lst.ultimul;
		lst.ultimul->next = aux;
		lst.ultimul = aux;
	}
	return lst;
}

void afisare_ordine(ListaDubla lst) {
	Nod* temp = lst.primul;
	while (temp) {
		printf("\n[%u] - Medic [%s]", temp->reteta.nb_reteta, temp->reteta.medic);
		for (int i = 0; i < temp->reteta.nb_medicamente; i++) {
			printf("\n  %s | Cant: %hhu | Pret: %.2f | Comp: %hhu%%",
				temp->reteta.lista_denumiri_medicamente[i],
				temp->reteta.cantitati_medicamente[i],
				temp->reteta.preturi_medicamente[i],
				temp->reteta.procente_compensare[i]);
		}
		printf("\n");
		temp = temp->next;
	}
}

void afisare_invers(ListaDubla lst) {
	Nod* temp = lst.ultimul;
	while (temp) {
		printf("\n[%u] - Medic [%s]", temp->reteta.nb_reteta, temp->reteta.medic);
		for (int i = 0; i < temp->reteta.nb_medicamente; i++) {
			printf("\n  %s | Cant: %hhu | Pret: %.2f | Comp: %hhu%%",
				temp->reteta.lista_denumiri_medicamente[i],
				temp->reteta.cantitati_medicamente[i],
				temp->reteta.preturi_medicamente[i],
				temp->reteta.procente_compensare[i]);
		}
		printf("\n");
		temp = temp->prev;
	}
}

void incasari_medicamente(ListaDubla lst, const char* medicament) {
	double total = 0;
	Nod* temp = lst.primul;
	while (temp) {
		for (int i = 0; i < temp->reteta.nb_medicamente; i++) {
			if (strcmp(temp->reteta.lista_denumiri_medicamente[i], medicament) == 0) {
				total += temp->reteta.cantitati_medicamente[i] * temp->reteta.preturi_medicamente[i];
			}
		}
		temp = temp->next;
	}
	printf("\nIncasari pentru medicamentul [%s] = %.2f\n", medicament, total);
}

int nb_retete(ListaDubla lst, const char* medic) {
	int count = 0;
	Nod* temp = lst.primul;
	while (temp) {
		if (strcmp(temp->reteta.medic, medic) == 0)
			count++;
		temp = temp->next;
	}
	return count;
}

Reteta deep_clone(Reteta r) {
	Reteta nou;
	nou.nb_reteta = r.nb_reteta;
	nou.nb_medicamente = r.nb_medicamente;

	nou.lista_denumiri_medicamente = (char**)malloc(sizeof(char*) * r.nb_medicamente);
	nou.cantitati_medicamente = (unsigned char*)malloc(sizeof(unsigned char) * r.nb_medicamente);
	nou.preturi_medicamente = (float*)malloc(sizeof(float) * r.nb_medicamente);
	nou.procente_compensare = (unsigned char*)malloc(sizeof(unsigned char) * r.nb_medicamente);

	for (int i = 0; i < r.nb_medicamente; i++) {
		nou.lista_denumiri_medicamente[i] = (char*)malloc(strlen(r.lista_denumiri_medicamente[i]) + 1);
		strcpy(nou.lista_denumiri_medicamente[i], r.lista_denumiri_medicamente[i]);
		nou.cantitati_medicamente[i] = r.cantitati_medicamente[i];
		nou.preturi_medicamente[i] = r.preturi_medicamente[i];
		nou.procente_compensare[i] = r.procente_compensare[i];
	}

	nou.medic = (char*)malloc(strlen(r.medic) + 1);
	strcpy(nou.medic, r.medic);

	return nou;
}

Reteta* retete_medic(ListaDubla lst, const char* medic, int* dim) {
	*dim = nb_retete(lst, medic);
	Reteta* vector = (Reteta*)malloc(sizeof(Reteta) * (*dim));
	int index = 0;
	Nod* temp = lst.primul;
	while (temp) {
		if (strcmp(temp->reteta.medic, medic) == 0) {
			vector[index++] = deep_clone(temp->reteta);
		}
		temp = temp->next;
	}
	return vector;
}

void valoare_compensata(Reteta* vector, int dim) {
	for (int i = 0; i < dim; i++) {
		printf("\nReteta [%d] - Medic [%s]", vector[i].nb_reteta, vector[i].medic);
		for (int j = 0; j < vector[i].nb_medicamente; j++) {
			float v = vector[i].cantitati_medicamente[j] * vector[i].preturi_medicamente[j];
			float compensata = v * vector[i].procente_compensare[j] / 100.0f;
			printf("\n - %s | Val. compensata: %.2f", vector[i].lista_denumiri_medicamente[j], compensata);
		}
	}
}

void cantitate_vanduta(ListaDubla lst, const char* medicament) {
	int total = 0;
	Nod* temp = lst.primul;
	while (temp) {
		for (int i = 0; i < temp->reteta.nb_medicamente; i++) {
			if (strcmp(temp->reteta.lista_denumiri_medicamente[i], medicament) == 0)
				total += temp->reteta.cantitati_medicamente[i];
		}
		temp = temp->next;
	}
	printf("\nCantitate totala vanduta pentru [%s] = %d\n", medicament, total);
}

void elibereaza_reteta(Reteta r) {
	for (int i = 0; i < r.nb_medicamente; i++)
		free(r.lista_denumiri_medicamente[i]);
	free(r.lista_denumiri_medicamente);
	free(r.cantitati_medicamente);
	free(r.preturi_medicamente);
	free(r.procente_compensare);
	free(r.medic);
}

void elibereaza_lista(ListaDubla* lst) {
	Nod* temp = lst->primul;
	while (temp) {
		Nod* next = temp->next;
		elibereaza_reteta(temp->reteta);
		free(temp);
		temp = next;
	}
	lst->primul = lst->ultimul = NULL;
}

void elibereaza_vector(Reteta* vector, int dim) {
	for (int i = 0; i < dim; i++)
		elibereaza_reteta(vector[i]);
	free(vector);
}

int main() {
	ListaDubla lst = { NULL, NULL };
	FILE* f = fopen("date.txt", "r");
	char buffer[256], * token, sep[] = ",\n";

	while (fgets(buffer, sizeof(buffer), f)) {
		Reteta r;
		token = strtok(buffer, sep);
		r.nb_reteta = atoi(token);

		token = strtok(NULL, sep);
		r.nb_medicamente = atoi(token);

		r.lista_denumiri_medicamente = (char**)malloc(sizeof(char*) * r.nb_medicamente);
		r.cantitati_medicamente = (unsigned char*)malloc(sizeof(unsigned char) * r.nb_medicamente);
		r.preturi_medicamente = (float*)malloc(sizeof(float) * r.nb_medicamente);
		r.procente_compensare = (unsigned char*)malloc(sizeof(unsigned char) * r.nb_medicamente);

		for (int i = 0; i < r.nb_medicamente; i++) {
			token = strtok(NULL, sep);
			r.lista_denumiri_medicamente[i] = (char*)malloc(strlen(token) + 1);
			strcpy(r.lista_denumiri_medicamente[i], token);
		}
		for (int i = 0; i < r.nb_medicamente; i++) {
			token = strtok(NULL, sep);
			r.cantitati_medicamente[i] = atoi(token);
		}
		for (int i = 0; i < r.nb_medicamente; i++) {
			token = strtok(NULL, sep);
			r.preturi_medicamente[i] = atof(token);
		}
		for (int i = 0; i < r.nb_medicamente; i++) {
			token = strtok(NULL, sep);
			r.procente_compensare[i] = atoi(token);
		}
		token = strtok(NULL, sep);
		r.medic = (char*)malloc(strlen(token) + 1);
		strcpy(r.medic, token);

		lst = inserare_lista(lst, r);
	}
	fclose(f);

	afisare_ordine(lst);
	afisare_invers(lst);
	incasari_medicamente(lst, "Metaxin");

	int dim = 0;
	Reteta* vector = retete_medic(lst, "Ionescu", &dim);
	valoare_compensata(vector, dim);
	cantitate_vanduta(lst, "Paracetamol");

	elibereaza_vector(vector, dim);
	elibereaza_lista(&lst);

	return 0;
}
