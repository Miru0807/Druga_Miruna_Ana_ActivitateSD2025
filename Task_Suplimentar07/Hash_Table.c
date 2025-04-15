#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cladire {
    int id;
    int anConstructie;
    char* strada;
    int nrEtaje;
} Cladire;

typedef struct Nod {
    Cladire cladire;
    struct Nod* next;
} Nod;

typedef struct HashTable {
    int dim;
    Nod** tabela;
} HashTable;


int hash(int an, int dim) {
    return an % dim;
}

HashTable initTabela(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.tabela = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++)
        ht.tabela[i] = NULL;
    return ht;
}

void inserareCladire(HashTable* ht, Cladire c) {
    int poz = hash(c.anConstructie, ht->dim);
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->cladire.id = c.id;
    nou->cladire.anConstructie = c.anConstructie;
    nou->cladire.nrEtaje = c.nrEtaje;
    nou->cladire.strada = (char*)malloc(strlen(c.strada) + 1);
    strcpy(nou->cladire.strada, c.strada);
    nou->next = ht->tabela[poz];
    ht->tabela[poz] = nou;
}

void afisareCladire(Cladire c) {
    printf("ID: %d | An: %d | Etaje: %d | Strada: %s\n", c.id, c.anConstructie, c.nrEtaje, c.strada);
}

void afisareTabela(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        printf("\nCluster [%d]:\n", i);
        Nod* p = ht.tabela[i];
        while (p) {
            afisareCladire(p->cladire);
            p = p->next;
        }
    }
}

void afisareCladiriDinAn(HashTable ht, int an) {
    int poz = hash(an, ht.dim);
    Nod* p = ht.tabela[poz];
    while (p) {
        if (p->cladire.anConstructie == an)
            afisareCladire(p->cladire);
        p = p->next;
    }
}

void stergeCladireID_An(HashTable* ht, int id, int an) {
    int poz = hash(an, ht->dim);
    Nod* p = ht->tabela[poz];
    Nod* prev = NULL;
    while (p) {
        if (p->cladire.id == id && p->cladire.anConstructie == an) {
            if (prev)
                prev->next = p->next;
            else
                ht->tabela[poz] = p->next;
            free(p->cladire.strada);
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}

void stergeCladireID(HashTable* ht, int id) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* p = ht->tabela[i];
        Nod* prev = NULL;
        while (p) {
            if (p->cladire.id == id) {
                if (prev)
                    prev->next = p->next;
                else
                    ht->tabela[i] = p->next;
                free(p->cladire.strada);
                free(p);
                return;
            }
            prev = p;
            p = p->next;
        }
    }
}

Cladire* cladiriDinAn(HashTable ht, int an, int* dim) {
    *dim = 0;
    int poz = hash(an, ht.dim);
    Nod* p = ht.tabela[poz];

    Nod* temp = p;
    while (temp) {
        if (temp->cladire.anConstructie == an)
            (*dim)++;
        temp = temp->next;
    }

    if (*dim == 0) return NULL;

    Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*dim));
    int i = 0;
    while (p) {
        if (p->cladire.anConstructie == an) {
            vector[i].id = p->cladire.id;
            vector[i].anConstructie = p->cladire.anConstructie;
            vector[i].nrEtaje = p->cladire.nrEtaje;
            vector[i].strada = (char*)malloc(strlen(p->cladire.strada) + 1);
            strcpy(vector[i].strada, p->cladire.strada);
            i++;
        }
        p = p->next;
    }

    return vector;
}

void modificaAn(HashTable* ht, int id, int anVechi, int anNou) {
    int pozVechi = hash(anVechi, ht->dim);
    Nod* p = ht->tabela[pozVechi];
    Nod* prev = NULL;

    while (p) {
        if (p->cladire.id == id && p->cladire.anConstructie == anVechi) {
            Cladire copie = p->cladire;
            if (prev) prev->next = p->next;
            else ht->tabela[pozVechi] = p->next;
            free(p);
            copie.anConstructie = anNou;
            inserareCladire(ht, copie);
            return;
        }
        prev = p;
        p = p->next;
    }
}

void dezalocareTabela(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* p = ht->tabela[i];
        while (p) {
            Nod* aux = p;
            p = p->next;
            free(aux->cladire.strada);
            free(aux);
        }
    }
    free(ht->tabela);
    ht->tabela = NULL;
    ht->dim = 0;
}


void citesteCladiriDinFisier(const char* numeFisier, HashTable* ht) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        return;
    }

    char linie[256];
    while (fgets(linie, sizeof(linie), f)) {
        Cladire c;
        char stradaTemp[100];

        int nr = sscanf(linie, "%d %d %[^\n] %d", &c.id, &c.anConstructie, stradaTemp, &c.nrEtaje);
        if (nr == 4) {
            c.strada = (char*)malloc(strlen(stradaTemp) + 1);
            strcpy(c.strada, stradaTemp);
            inserareCladire(ht, c);
        }
    }

    fclose(f);
}


int main() {
    HashTable ht = initTabela(10);
    citesteCladiriDinFisier("apartamente.txt", &ht);

    printf("\n--- Tabela initiala ---\n");
    afisareTabela(ht);

    printf("\n--- Cladiri din anul 1990 ---\n");
    afisareCladiriDinAn(ht, 1990);

    printf("\n--- Stergere cladire id=2, an=1990 ---\n");
    stergeCladireID_An(&ht, 2, 1990);
    afisareTabela(ht);

    printf("\n--- Stergere doar dupa id=5 ---\n");
    stergeCladireID(&ht, 5);
    afisareTabela(ht);

    printf("\n--- Cladiri din 1977 (copie in vector) ---\n");
    int dim = 0;
    Cladire* vector = cladiriDinAn(ht, 1977, &dim);
    for (int i = 0; i < dim; i++) {
        afisareCladire(vector[i]);
        free(vector[i].strada);
    }
    free(vector);

    printf("\n--- Modific an constructie id=3 -> din 1977 in 2000 ---\n");
    modificaAn(&ht, 3, 1977, 2000);
    afisareTabela(ht);

    dezalocareTabela(&ht);
    return 0;
}
