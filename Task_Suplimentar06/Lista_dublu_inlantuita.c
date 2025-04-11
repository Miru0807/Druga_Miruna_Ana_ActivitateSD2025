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
    Masina masina;
    struct Nod* next;
    struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
    Nod* first;
    Nod* last;
    int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

struct NodSimplu {
    Masina masina;
    struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

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

void afisareMasina(Masina m) {
    printf("Id: %d\nNr. usi: %d\nPret: %.2f\nModel: %s\nNume sofer: %s\nSerie: %c\n\n",
        m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
    Nod* p = lista.first;
    while (p) {
        afisareMasina(p->masina);
        p = p->next;
    }
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
    Nod* p = lista.last;
    while (p) {
        afisareMasina(p->masina);
        p = p->prev;
    }
}

void adaugaMasinaInLista(ListaDubla* lista, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = m;
    nou->next = NULL;
    nou->prev = lista->last;
    if (lista->last)
        lista->last->next = nou;
    else
        lista->first = nou;
    lista->last = nou;
    lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
    ListaDubla lista = { NULL, NULL, 0 };
    FILE* f = fopen(numeFisier, "r");
    while (!feof(f)) {
        adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
    }
    fclose(f);
    return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
    Nod* p = lista->first;
    while (p) {
        Nod* aux = p;
        p = p->next;
        free(aux->masina.model);
        free(aux->masina.numeSofer);
        free(aux);
    }
    lista->first = lista->last = NULL;
    lista->nrNoduri = 0;
}

void stergerePozitie(ListaDubla* lista, int pozitie) {
    if (pozitie < 0 || pozitie >= lista->nrNoduri) return;
    Nod* p = lista->first;
    int i = 0;
    while (p && i < pozitie) {
        p = p->next;
        i++;
    }
    if (!p) return;
    if (p->prev) p->prev->next = p->next;
    else lista->first = p->next;
    if (p->next) p->next->prev = p->prev;
    else lista->last = p->prev;
    free(p->masina.model);
    free(p->masina.numeSofer);
    free(p);
    lista->nrNoduri--;
}

void inserareSortataDupaPret(ListaDubla* lista, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = m;
    nou->next = NULL;
    nou->prev = NULL;
    if (!lista->first) {
        lista->first = lista->last = nou;
    }
    else if (m.pret < lista->first->masina.pret) {
        nou->next = lista->first;
        lista->first->prev = nou;
        lista->first = nou;
    }
    else {
        Nod* p = lista->first;
        while (p->next && p->next->masina.pret < m.pret) p = p->next;
        nou->next = p->next;
        if (p->next) p->next->prev = nou;
        else lista->last = nou;
        p->next = nou;
        nou->prev = p;
    }
    lista->nrNoduri++;
}

void adaugaInListaSimpla(NodSimplu** cap, Masina m) {
    NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
    nou->masina.id = m.id;
    nou->masina.nrUsi = m.nrUsi;
    nou->masina.pret = m.pret;
    nou->masina.serie = m.serie;
    nou->masina.model = malloc(strlen(m.model) + 1);
    strcpy(nou->masina.model, m.model);
    nou->masina.numeSofer = malloc(strlen(m.numeSofer) + 1);
    strcpy(nou->masina.numeSofer, m.numeSofer);
    nou->next = *cap;
    *cap = nou;
}

NodSimplu* extrageMasiniScumpe(ListaDubla lista, float prag) {
    NodSimplu* cap = NULL;
    Nod* p = lista.first;
    while (p) {
        if (p->masina.pret > prag) adaugaInListaSimpla(&cap, p->masina);
        p = p->next;
    }
    return cap;
}

void afisareListaSimpla(NodSimplu* cap) {
    while (cap) {
        afisareMasina(cap->masina);
        cap = cap->next;
    }
}

void interschimbaPozitii(ListaDubla lista, int p1, int p2) {
    if (p1 == p2 || p1 < 0 || p2 < 0 || p1 >= lista.nrNoduri || p2 >= lista.nrNoduri) return;
    Nod* n1 = lista.first;
    Nod* n2 = lista.first;
    int i = 0;
    while (n1 && i < p1) { n1 = n1->next; i++; }
    i = 0;
    while (n2 && i < p2) { n2 = n2->next; i++; }
    if (n1 && n2) {
        Masina aux = n1->masina;
        n1->masina = n2->masina;
        n2->masina = aux;
    }
}

int main() {
    ListaDubla lista = citireLDMasiniDinFisier("masini.txt");

    printf("\n--- Afisare crescator ---\n");
    afisareListaMasiniDeLaInceput(lista);

    printf("\n--- Afisare descrescator ---\n");
    afisareListaMasiniDeLaSfarsit(lista);

    printf("\n--- Stergere pozitia 3 ---\n");
    stergerePozitie(&lista, 3);
    afisareListaMasiniDeLaInceput(lista);

    printf("\n--- Inserare sortata masina noua ---\n");
    Masina mNou;
    mNou.id = 999;
    mNou.nrUsi = 5;
    mNou.pret = 7500;
    mNou.model = (char*)malloc(strlen("NouaMasina") + 1);
    strcpy(mNou.model, "NouaMasina");
    mNou.numeSofer = (char*)malloc(strlen("SoferX") + 1);
    strcpy(mNou.numeSofer, "SoferX");
    mNou.serie = 'X';
    inserareSortataDupaPret(&lista, mNou);
    afisareListaMasiniDeLaInceput(lista);

    printf("\n--- Extractie masini > 8000 in lista simpla ---\n");
    NodSimplu* cap = extrageMasiniScumpe(lista, 8000);
    afisareListaSimpla(cap);

    printf("\n--- Interschimbare pozitii 1 si 5 ---\n");
    interschimbaPozitii(lista, 1, 5);
    afisareListaMasiniDeLaInceput(lista);

    dezalocareLDMasini(&lista);
    return 0;
}
