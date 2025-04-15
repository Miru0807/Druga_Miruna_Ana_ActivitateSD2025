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

//Citirea obiectelor din fisier si salvarea intr-un vector
Diamant* citesteDinFisier(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }

    Diamant* vector = malloc(20 * sizeof(Diamant));
    *dim = 0;
    char linie[256];

    while (fgets(linie, sizeof(linie), f)) {
        Diamant d;
        char denumireTemp[100] = "";
        char culoareTemp[50] = "";
        char* token = strtok(linie, " \n");

        if (token != NULL) {
            d.cod = atoi(token);

            token = strtok(NULL, " \n");
            if (token != NULL) {
                d.anulDescoperirii = atoi(token);
                token = strtok(NULL, " \n");
                if (token != NULL) {
                    strcat(denumireTemp, token);
                    token = strtok(NULL, " \n");
                    if (token != NULL) {
                        strcat(denumireTemp, " ");
                        strcat(denumireTemp, token);

                        d.denumire = malloc(strlen(denumireTemp) + 1);
                        strcpy(d.denumire, denumireTemp);
                        token = strtok(NULL, " \n");
                        if (token != NULL) {
                            strcpy(culoareTemp, token);
                            d.culoare = malloc(strlen(culoareTemp) + 1);
                            strcpy(d.culoare, culoareTemp);
                            token = strtok(NULL, " \n");
                            if (token != NULL) {
                                d.carataj = atof(token);
                                token = strtok(NULL, " \n");
                                if (token != NULL) {
                                    d.valoareEstimata = atof(token);

                                    vector[*dim] = d;
                                    (*dim)++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fclose(f);
    return vector;
}

//Copiere in matrice alocata dinamic + grupare dupa culoare
Diamant** creeazaMatrice(Diamant* vector, int dim, int* nrLinii, int** nrColoane) {
    *nrLinii = 4;
    Diamant** matrice = malloc((*nrLinii) * sizeof(Diamant*));
    *nrColoane = malloc(*nrLinii * sizeof(int));

    for (int i = 0; i < *nrLinii; i++) {
        matrice[i] = malloc(dim * sizeof(Diamant));
        (*nrColoane)[i] = 0;
    }

    for (int i = 0; i < dim; i++) {
        int linie = 3;
        if (strcmp(vector[i].culoare, "Alb") == 0) linie = 0;
        else if (strcmp(vector[i].culoare, "Verde") == 0) linie = 1;
        else if (strcmp(vector[i].culoare, "Transparent") == 0) linie = 2;

        matrice[linie][(*nrColoane)[linie]] = vector[i];
        (*nrColoane)[linie]++;
    }

    return matrice;
}

//Sortare linii dupa nr. de elemente
void sorteazaLiniile(Diamant*** matrice, int* nrColoane, int nrLinii) {
    for (int i = 0; i < nrLinii - 1; i++) {
        for (int j = i + 1; j < nrLinii; j++) {
            if (nrColoane[i] > nrColoane[j]) {
                int temp = nrColoane[i];
                nrColoane[i] = nrColoane[j];
                nrColoane[j] = temp;

                Diamant* tmp = (*matrice)[i];
                (*matrice)[i] = (*matrice)[j];
                (*matrice)[j] = tmp;
            }
        }
    }
}

//Afisare matrice
void afiseazaMatrice(Diamant** matrice, int linii, int* coloanePeLinie) {
    for (int i = 0; i < linii; i++) {
        printf("Linia %d (%d elemente):\n", i + 1, coloanePeLinie[i]);
        for (int j = 0; j < coloanePeLinie[i]; j++) {
            Diamant d = matrice[i][j];
            printf("  Cod: %d | An: %d | Denumire: %s | Culoare: %s | Carataj: %.2f | Valoare: %.2f\n",
                d.cod, d.anulDescoperirii, d.denumire, d.culoare, d.carataj, d.valoareEstimata);
        }
        printf("\n");
    }
}

int main() {
    int dim = 0;
    Diamant* vector = citesteDinFisier("diamante.txt", &dim);
    if (!vector) return -1;

    int nrLinii = 0;
    int* nrColoane = NULL;
    Diamant** matrice = creeazaMatrice(vector, dim, &nrLinii, &nrColoane);

    sorteazaLiniile(&matrice, nrColoane, nrLinii);
    printf("Matrice sortata dupa numarul de elemente pe linie:\n\n");
    afiseazaMatrice(matrice, nrLinii, nrColoane);

    for (int i = 0; i < dim; i++) {
        free(vector[i].denumire);
        free(vector[i].culoare);
    }
    free(vector);

    for (int i = 0; i < nrLinii; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(nrColoane);

    return 0;
}
