#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int cod;
    int anulDescoperirii;
    char* denumire;
    char* culoare;
    float carataj;
    float valoareEstimata;
} Diamant;


Diamant citesteDiamant() {
    Diamant d;
    d.denumire = (char*)malloc(50 * sizeof(char)); 
    d.culoare = (char*)malloc(20 * sizeof(char)); 

    if (d.denumire == NULL || d.culoare == NULL) {
        printf("Eroare la alocarea memoriei!\n");
    }

    printf("Introduceti codul diamantului: ");
    scanf("%d", &d.cod);

    printf("Introduceti anul descoperirii: ");
    scanf("%d", &d.anulDescoperirii);

    printf("Introduceti denumirea diamantului: ");
    scanf("%s", d.denumire);

    printf("Introduceti culoarea diamantului: ");
    scanf("%s", d.culoare);

    printf("Introduceti caratajul diamantului: ");
    scanf("%f", &d.carataj);

    printf("Introduceti valoarea estimata a diamantului: ");
    scanf("%f", &d.valoareEstimata);

    return d;
}

//Realizati o functie care va creea un nou vector în care va copia dintr-un vector primit ca 
//parametru obiectele care indeplinesc o anumita conditie.Stabiliti voi conditia in functie de
//un atribut sau doua atribute.

void afiseazaVectorDiamante(Diamant* vector, int dim) {
    printf("\nLista de diamante:\n");
    for (int i = 0; i < dim; i++) {
        printf("Cod: %d, An: %d, Denumire: %s, Culoare: %s, Carataj: %.2f, Valoare: %.2f\n",
            vector[i].cod, vector[i].anulDescoperirii, vector[i].denumire,
            vector[i].culoare, vector[i].carataj, vector[i].valoareEstimata);
    }
}

Diamant* filtreazaDupaValoare(Diamant* vector, int dim, float prag, int* nouaDim) {
    *nouaDim = 0;
    for (int i = 0; i < dim; i++) {
        if (vector[i].valoareEstimata > prag) {
            (*nouaDim)++;
        }
    }

    Diamant* nouVector = (Diamant*)malloc((*nouaDim) * sizeof(Diamant));
    if (nouVector == NULL) {
        printf("Eroare la alocarea memoriei pentru vectorul filtrat!\n");
        return -1;
    }

    int index = 0;
    for (int i = 0; i < dim; i++) {
        if (vector[i].valoareEstimata > prag) {
            nouVector[index++] = vector[i];
        }
    }

    return nouVector;
}

// Realizati o functie care muta intr-un nou vector obiectele care indeplinesc o alta conditie 
// fata de cerinta precedenta.

Diamant* mutaDupaAn(Diamant* vector, int* dim, int an, int* nouaDim) {
    *nouaDim = 0;
    for (int i = 0; i < *dim; i++) {
        if (vector[i].anulDescoperirii < an) {
            (*nouaDim)++;
        }
    }

    Diamant* nouVector = (Diamant*)malloc((*nouaDim) * sizeof(Diamant));
    if (nouVector == NULL) {
        printf("Eroare la alocarea memoriei pentru vectorul mutat!\n");
        exit(1);
    }

    int index = 0, j = 0;
    for (int i = 0; i < *dim; i++) {
        if (vector[i].anulDescoperirii < an) {
            nouVector[index++] = vector[i];
        }
        else {
            vector[j++] = vector[i];
        }
    }

    *dim = j;
    return nouVector;
}

// Realizati o functie care concateneaza doi vectori. 
Diamant* concateneazaVectori(Diamant* v1, int dim1, Diamant* v2, int dim2, int* dimNou) {
    *dimNou = dim1 + dim2;
    Diamant* vectorNou = (Diamant*)malloc((*dimNou) * sizeof(Diamant));
    if (vectorNou == NULL) {
        printf("Eroare la alocarea memoriei pentru concatenare!\n");
        exit(1);
    }

    for (int i = 0; i < dim1; i++) {
        vectorNou[i] = v1[i];
    }

    for (int i = 0; i < dim2; i++) {
        vectorNou[dim1 + i] = v2[i];
    }

    return vectorNou;
}

int main() {
    int dim = 5;
    Diamant* diamante = (Diamant*)malloc(dim * sizeof(Diamant));
    if (diamante == NULL) {
        printf("Eroare la alocarea memoriei pentru vector!\n");
    }

    printf("\nIntroduceti datele pentru %d diamante:\n", dim);
    for (int i = 0; i < dim; i++) {
        diamante[i] = citesteDiamant();
    }
    
    // Realizati o functie care afiseaza un vector cu obiecte.

    afiseazaVectorDiamante(diamante, dim);

    // Filtrare
    int dimFiltrat;
    float prag = 40000.00;
    Diamant* diamanteScumpe = filtreazaDupaValoare(diamante, dim, prag, &dimFiltrat);
    printf("\nDiamante cu valoare estimata peste %.2f:\n", prag);
    afiseazaVectorDiamante(diamanteScumpe, dimFiltrat);

    // Mutare dupa anul descoperirii
    int dimMutat;
    int anPrag = 1900;
    Diamant* diamanteVechi = mutaDupaAn(diamante, &dim, anPrag, &dimMutat);
    printf("\nDiamante descoperite inainte de %d:\n", anPrag);
    afiseazaVectorDiamante(diamanteVechi, dimMutat);

    printf("\nDiamante ramase dupa mutare:\n");
    afiseazaVectorDiamante(diamante, dim);

    // Concatenare
    int dimConcat;
    Diamant* toateDiamantele = concateneazaVectori(diamante, dim, diamanteScumpe, dimFiltrat, &dimConcat);
    printf("\nConcatenare: toate diamantele initiale + cele scumpe:\n");
    afiseazaVectorDiamante(toateDiamantele, dimConcat);

    // Eliberare memorie
    free(diamante);
    free(diamanteScumpe);
    free(diamanteVechi);
    free(toateDiamantele);

    return 0;
}
