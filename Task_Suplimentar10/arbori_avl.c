#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
    char titlu[100];
    char autor[100];
    char isbn[20]; 
    int anPublicare;
};

typedef struct Carte Carte;

struct NodAVL {
    Carte carte;           
    struct NodAVL* stg;    
    struct NodAVL* dr;     
    int inaltime;          
    int gradEchilibru;     
};

typedef struct NodAVL NodAVL;

int inaltime(NodAVL* nod) {
    if (nod == NULL)
        return 0;
    return nod->inaltime;
}


int gradEchilibru(NodAVL* nod) {
    if (nod == NULL)
        return 0;
    return inaltime(nod->stg) - inaltime(nod->dr);
}

void rotireDreapta(NodAVL** arbore) {
    NodAVL* aux = (*arbore)->stg;
    (*arbore)->stg = aux->dr;
    aux->dr = (*arbore);
    (*arbore) = aux;
    (*arbore)->inaltime = 1 + max(inaltime((*arbore)->stg), inaltime((*arbore)->dr));
    (*arbore)->dr->inaltime = 1 + max(inaltime((*arbore)->dr->stg), inaltime((*arbore)->dr->dr));
}

void rotireStanga(NodAVL** arbore) {
    NodAVL* aux = (*arbore)->dr;
    (*arbore)->dr = aux->stg;
    aux->stg = (*arbore);
    (*arbore) = aux;
    (*arbore)->inaltime = 1 + max(inaltime((*arbore)->stg), inaltime((*arbore)->dr));
    (*arbore)->stg->inaltime = 1 + max(inaltime((*arbore)->stg->stg), inaltime((*arbore)->stg->dr));
}

NodAVL* inserare(NodAVL* nod, Carte carte) {
    if (nod == NULL) {
        NodAVL* nou = (NodAVL*)malloc(sizeof(NodAVL));
        nou->carte = carte;
        nou->stg = nou->dr = NULL;
        nou->inaltime = 1;
        return nou;
    }


    if (strcmp(carte.isbn, nod->carte.isbn) < 0)
        nod->stg = inserare(nod->stg, carte);  
    else if (strcmp(carte.isbn, nod->carte.isbn) > 0)
        nod->dr = inserare(nod->dr, carte);  
    else
        return nod;  

    nod->inaltime = 1 + max(inaltime(nod->stg), inaltime(nod->dr));
    nod->gradEchilibru = gradEchilibru(nod);


    if (nod->gradEchilibru > 1 && strcmp(carte.isbn, nod->stg->carte.isbn) < 0) {
        rotireDreapta(&nod);  
    }

    if (nod->gradEchilibru < -1 && strcmp(carte.isbn, nod->dr->carte.isbn) > 0) {
        rotireStanga(&nod); 
    }

    if (nod->gradEchilibru > 1 && strcmp(carte.isbn, nod->stg->carte.isbn) > 0) {
        rotireStanga(&nod->stg);  
        rotireDreapta(&nod);  
    }

    if (nod->gradEchilibru < -1 && strcmp(carte.isbn, nod->dr->carte.isbn) < 0) {
        rotireDreapta(&nod->dr);  
        rotireStanga(&nod);  
    }
}

NodAVL* cautaCarte(NodAVL* radacina, const char* isbn) {
    if (radacina == NULL || strcmp(radacina->carte.isbn, isbn) == 0)
        return radacina;

    if (strcmp(isbn, radacina->carte.isbn) < 0)
        return cautaCarte(radacina->stg, isbn);  

    return cautaCarte(radacina->dr, isbn);  
}

void preordine(NodAVL* radacina, Carte* noduri, int* index) {
    if (radacina != NULL) {
        noduri[*index] = radacina->carte;
        (*index)++;
        preordine(radacina->stg, noduri, index);
        preordine(radacina->dr, noduri, index);
    }
}

void inordine(NodAVL* radacina, Carte* noduri, int* index) {
    if (radacina != NULL) {
        inordine(radacina->stg, noduri, index);
        noduri[*index] = radacina->carte;
        (*index)++;
        inordine(radacina->dr, noduri, index);
    }
}

void postordine(NodAVL* radacina, Carte* noduri, int* index) {
    if (radacina != NULL) {
        postordine(radacina->stg, noduri, index);
        postordine(radacina->dr, noduri, index);
        noduri[*index] = radacina->carte;
        (*index)++;
    }
}

NodAVL* citireArboreDeCartiDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Fisierul nu a putut fi deschis!\n");
        return NULL;
    }

    NodAVL* arbore = NULL;
    char buffer[256];
    while (fgets(buffer, 256, f)) {
        Carte carte;
        char* token = strtok(buffer, ",");
        if (token) strcpy(carte.titlu, token);
        token = strtok(NULL, ",");
        if (token) strcpy(carte.autor, token);
        token = strtok(NULL, ",");
        if (token) strcpy(carte.isbn, token);
        token = strtok(NULL, ",\n");
        if (token) carte.anPublicare = atoi(token);
        arbore = inserare(arbore, carte);
    }

    fclose(f);
    return arbore;
}

void afisareCarte(Carte c);

//carti publicate dupa un anumit an

void afiseazaCartiDupaAn(NodAVL* radacina, int an) {
    if (radacina) {
        afiseazaCartiDupaAn(radacina->stg, an);
        if (radacina->carte.anPublicare > an)
            afisareCarte(radacina->carte);
        afiseazaCartiDupaAn(radacina->dr, an);
    }
}


void afisareCarte(Carte c) {
    printf("Titlu: %s\n", c.titlu);
    printf("Autor: %s\n", c.autor);
    printf("ISBN: %s\n", c.isbn);
    printf("An publicare: %d\n", c.anPublicare);
    printf("-------------------------\n");
}

int main() {
    NodAVL* arbore = citireArboreDeCartiDinFisier("carti.txt");
    Carte noduri[100];
    int index;
    printf("Parcurgere Preordine:\n");
    index = 0;
    preordine(arbore, noduri, &index);
    for (int i = 0; i < index; i++) {
        afisareCarte(noduri[i]);
    }
    printf("\nParcurgere Inordine:\n");
    index = 0;
    inordine(arbore, noduri, &index);
    for (int i = 0; i < index; i++) {
        afisareCarte(noduri[i]);
    }
    printf("\nParcurgere Postordine:\n");
    index = 0;
    postordine(arbore, noduri, &index);
    for (int i = 0; i < index; i++) {
        afisareCarte(noduri[i]);
    }

    printf("\n-------------------------------\n");
    printf("\nCarti publicate dupa anul 2010:\n");
    afiseazaCartiDupaAn(arbore, 2010);
    return 0;
}

