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
typedef struct Nod Nod;

struct Nod {
    Masina info;
    Nod* next;
};

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
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

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

void afisareListaMasini(Nod* cap) {
    while (cap) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;

    if (*cap == NULL)
    {
        *cap = nou;
    }
    else
    {
        Nod* temp = *cap;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = nou;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = *cap;
    *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* cap = NULL;

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        adaugaMasinaInLista(&cap, m);
    }
    fclose(f);
    return (void*)cap;
}

void dezalocareListaMasini(Nod** cap) {
    while (*cap) {
        Nod* p = (*cap);
        (*cap) = (*cap)->next;
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }
}

float calculeazaPretMediu(Nod* cap) {
    float suma = 0;
    int k = 0;
    while (cap) {
        suma += cap->info.pret;
        k++;
        cap = cap->next;
    }
    return (k > 0) ? suma / k : 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
    while (*cap && (*cap)->info.serie == serieCautata) {
        Nod* temp = *cap;
        *cap = (*cap)->next;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }
    Nod* curent = *cap;
    while (curent && curent->next) {
        if (curent->next->info.serie == serieCautata)
        {
            Nod* temp = curent->next;
            curent->next = curent->next->next;
            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
        }
        else
        {
            curent = curent->next;
        }
    }
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
    float suma = 0;
    while (cap) {
        if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
            suma += cap->info.pret;
        }
        cap = cap->next;
    }
    return suma;
}

int main() {
    Nod* masini = (Nod*)citireListaMasiniDinFisier("masini.txt");
    afisareListaMasini(masini);
    printf("---------------------------------------\n");
    printf("Pret mediu: %.2f\n", calculeazaPretMediu(masini));
    stergeMasiniDinSeria(&masini, 'A');
    stergeMasiniDinSeria(&masini, 'B');
    printf("----------DUPA STERGERE-----------\n");
    afisareListaMasini(masini);
    printf("---------------------------------------\n");
    printf("Pret total masini sofer Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(masini, "Ionescu"));
    dezalocareListaMasini(&masini);
    return 0;
}