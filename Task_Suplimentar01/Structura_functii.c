#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Se consideră prima literă din numele vostru, și prima literă din prenumele vostru.  Druga Miruna

typedef struct {
    int cod;             
    int anulDescoperirii; 
    char* denumire;       
    char* culoare;        
    float carataj;        
    float valoareEstimata;
} Diamant;

// Realizati o functie cu ajutorul careia sa cititi de la tastatura un articol de tipul construit de 
//voi.Funcția returnează obiectul citit.
 
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

//Realizati o functie care calculeaza ceva pentru un obiect de tipul implementat de voi. (o 
//medie sau o suma, un maxim, un minim...sau orice altceva)

float calculeazaPretPeCarat(Diamant d) {
    if (d.carataj == 0) return 0;
    return d.valoareEstimata / d.carataj;
}

// Realizati o functie care modifica pentru un obiect primit ca parametru, un anumit camp. 
// noua valoare este primita ca parametru.

void modificaValoare(Diamant* d, float nouaValoare) {
    d->valoareEstimata = nouaValoare;
}

// Realizați o funcție care afișează un obiect de tipul creat. Afișarea se face la console, și sunt 
// afișate toate informațiile.

void afiseazaDiamant(Diamant d) {
    printf("Cod: %d\n", d.cod);
    printf("An descoperire: %d\n", d.anulDescoperirii);
    printf("Denumire: %s\n", d.denumire);
    printf("Culoare: %s\n", d.culoare);
    printf("Carataj: %.2f\n", d.carataj);
    printf("Valoare estimata: %.2f\n", d.valoareEstimata);
    printf("Pret estimat per carat: %.2f\n", calculeazaPretPeCarat(d));
}

int main() {
    Diamant diamant = citesteDiamant();
    afiseazaDiamant(diamant);
    float nouaValoare;
    printf("\nIntroduceti noua valoare estimata a diamantului: ");
    scanf("%f", &nouaValoare);
    modificaValoare(&diamant, nouaValoare);
    afiseazaDiamant(diamant);

    free(diamant.denumire);
    free(diamant.culoare);

    return 0;
}
