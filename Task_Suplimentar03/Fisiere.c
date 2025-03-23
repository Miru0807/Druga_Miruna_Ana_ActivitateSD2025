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

// Functie pentru scrierea unui obiect intr-un fisier text
void salveazaDiamantInFisier(Diamant d, FILE* fisier) {
    fprintf(fisier, "%d %d %s %s %.2f %.2f\n",
        d.cod, d.anulDescoperirii, d.denumire,
        d.culoare, d.carataj, d.valoareEstimata);
}

// Functie pentru scrierea unui vector de obiecte intr-un fisier text
void salveazaVectorInFisier(Diamant* vector, int dim, const char* numeFisier) {
    FILE* fisier = fopen(numeFisier, "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului %s!\n", numeFisier);
        return;
    }
    for (int i = 0; i < dim; i++) {
        salveazaDiamantInFisier(vector[i], fisier);
    }
    fclose(fisier);
}

void adaugaDiamantInFisier(Diamant** diamante, int* dim, Diamant diamantNou) {
    Diamant* aux = (Diamant*)malloc(sizeof(Diamant) * ((*dim) + 1));
    for (int i = 0;i < *dim;i++) {
        aux[i] = (*diamante)[i];
    }
    aux[(*dim)] = diamantNou;
    free(*diamante);
    (*diamante) = aux;
    (*dim)++;
}

// Functie pentru citirea unui obiect diamant din fisier
Diamant citireDiamantFisier(FILE* file) {
    Diamant d;
    char buffer[100];
    char sep[3] = " \n";
    fgets(buffer, 100, file);
    d.cod = atoi(strtok(buffer, sep));
    d.anulDescoperirii = atoi(strtok(NULL, sep));
    char* aux;
    aux = strtok(NULL, sep);
    d.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(d.denumire, aux);
    aux = strtok(NULL, sep);
    d.culoare = (char*)malloc(strlen(aux) + 1);
    strcpy(d.culoare, aux);
    d.carataj = atof(strtok(NULL, sep));
    d.valoareEstimata = atof(strtok(NULL, sep));
    
    return d;

}



// Functie pentru citirea unui vector de obiecte intr-un fisier text
Diamant* citesteVectorDinFisier(const char* numeFisier, int* dim) {

    FILE* file = fopen(numeFisier, "r");
    Diamant* diamante = NULL;
    (*dim) = 0;
    while (!feof(file)) {
        adaugaDiamantInFisier(&diamante, dim, citireDiamantFisier(file));
    }
    fclose(file);
    return diamante;

}





// Scrieti o functie care va salva un vector de obiecte intr-un fisier text.
void afiseazaVectorDiamante(Diamant* vector, int dim) {
    printf("\nLista de diamante citite din fisier:\n");
    for (int i = 0; i < dim; i++) {
        printf("Cod: %d | An: %d | Denumire: %s | Culoare: %s | Carataj: %.2f | Valoare: %.2f\n",
            vector[i].cod, vector[i].anulDescoperirii, vector[i].denumire,
            vector[i].culoare, vector[i].carataj, vector[i].valoareEstimata);
    }
}

int main() {
    int dim = 10;
    Diamant diamante[10] = {
        {201, 2001, "Celestial Flame", "Rosu", 12.34, 4500000},
        {202, 1995, "Ocean Whisper", "Albastru", 88.88, 23000000},
        {203, 2010, "Sunbeam", "Galben", 15.75, 1500000},
        {204, 1985, "Frozen Tear", "Transparent", 67.45, 8000000},
        {205, 1978, "Nightfall", "Negru", 25.60, 1200000},
        {206, 2005, "Aurora's Heart", "Roz", 38.20, 9900000},
        {207, 1990, "Emerald Sky", "Verde", 45.00, 6700000},
        {208, 1988, "Luna Nova", "Alb", 33.33, 4400000},
        {209, 2020, "Desert Mirage", "Portocaliu", 20.50, 2500000},
        {210, 2015, "Mystic Shadow", "Violet", 19.99, 3700000}
    };



    const char* fisierDiamanteOut = "diamante-out.txt";
    const char* fisierDiamante = "diamante.txt";

    salveazaVectorInFisier(diamante, dim, fisierDiamanteOut);
    printf("\nObiectele au fost salvate in fisierul %s.\n", fisierDiamanteOut);


    int dimCitita;
    Diamant* diamanteCitite = citesteVectorDinFisier(fisierDiamante, &dimCitita);

    if (diamanteCitite != NULL) {
        afiseazaVectorDiamante(diamanteCitite, dimCitita);
        free(diamanteCitite); 
    }

    return 0;
}
