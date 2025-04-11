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

struct Nod {
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = (float)atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\nNr. usi: %d\nPret: %.2f\nModel: %s\nNume sofer: %s\nSerie: %c\n\n",
        masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareListaMasini(Nod* cap) {
    while (cap != NULL) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;
    if (*cap) {
        Nod* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
    else {
        *cap = nou;
    }
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    Nod* cap = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
        }
        fclose(f);
    }
    return cap;
}

void dezalocareListaMasini(Nod** cap) {
    while (*cap) {
        Nod* p = *cap;
        (*cap) = p->next;
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }
}

void stergeNodPozitie(Nod** cap, int pozitie) {
    if (pozitie < 0 || *cap == NULL) return;
    Nod* temp = *cap;
    if (pozitie == 0) {
        *cap = temp->next;
    }
    else {
        Nod* prev = NULL;
        int index = 0;
        while (temp != NULL && index < pozitie) {
            prev = temp;
            temp = temp->next;
            index++;
        }
        if (temp == NULL) return;
        prev->next = temp->next;
    }
    free(temp->info.model);
    free(temp->info.numeSofer);
    free(temp);
}

void inserareSortataDupaPret(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;

    if (*cap == NULL || (*cap)->info.pret > masinaNoua.pret) {
        nou->next = *cap;
        *cap = nou;
        return;
    }

    Nod* curent = *cap;
    while (curent->next && curent->next->info.pret <= masinaNoua.pret) {
        curent = curent->next;
    }
    nou->next = curent->next;
    curent->next = nou;
}

Masina* extrageMasiniScumpe(Nod* cap, int* dimVector, float prag) {
    *dimVector = 0;
    Nod* temp = cap;
    while (temp) {
        if (temp->info.pret > prag)
            (*dimVector)++;
        temp = temp->next;
    }

    if (*dimVector == 0) return NULL;

    Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dimVector));
    temp = cap;
    int index = 0;
    while (temp) {
        if (temp->info.pret > prag) {
            vector[index].id = temp->info.id;
            vector[index].nrUsi = temp->info.nrUsi;
            vector[index].pret = temp->info.pret;
            vector[index].serie = temp->info.serie;

            vector[index].model = (char*)malloc(strlen(temp->info.model) + 1);
            strcpy(vector[index].model, temp->info.model);

            vector[index].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
            strcpy(vector[index].numeSofer, temp->info.numeSofer);

            index++;
        }
        temp = temp->next;
    }
    return vector;
}

void interschimbaElementePozitii(Nod* cap, int p1, int p2) {
    if (p1 == p2) return;
    Nod* n1 = NULL;
    Nod* n2 = NULL;
    Nod* temp = cap;
    int index = 0;

    while (temp) {
        if (index == p1) n1 = temp;
        if (index == p2) n2 = temp;
        temp = temp->next;
        index++;
    }

    if (n1 && n2) {
        Masina aux = n1->info;
        n1->info = n2->info;
        n2->info = aux;
    }
}

int main() {
    Nod* cap = citireListaMasiniDinFisier("masini.txt");
    printf("Lista initiala:\n");
    afisareListaMasini(cap);

    printf("\n--- Stergere pozitie 2 ---\n");
    stergeNodPozitie(&cap, 2);
    afisareListaMasini(cap);

    printf("\n--- Inserare sortata masina cu pret 6000 ---\n");
    Masina mNou;
    mNou.id = 999;
    mNou.nrUsi = 4;
    mNou.pret = 6000;
    mNou.model = (char*)malloc(strlen("Noua") + 1);
    strcpy(mNou.model, "Noua");
    mNou.numeSofer = (char*)malloc(strlen("Test") + 1);
    strcpy(mNou.numeSofer, "Test");
    mNou.serie = 'Z';
    inserareSortataDupaPret(&cap, mNou);
    afisareListaMasini(cap);

    printf("\n--- Extractie masini > 8000 ---\n");
    int dim;
    Masina* vector = extrageMasiniScumpe(cap, &dim, 8000);
    for (int i = 0; i < dim; i++) {
        afisareMasina(vector[i]);
        free(vector[i].model);
        free(vector[i].numeSofer);
    }
    free(vector);

    printf("\n--- Interschimbare pozitii 1 si 3 ---\n");
    interschimbaElementePozitii(cap, 1, 3);
    afisareListaMasini(cap);

    dezalocareListaMasini(&cap);
    return 0;
}
