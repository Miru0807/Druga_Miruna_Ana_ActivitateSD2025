#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    float volum;
    char* culoare;
} Cana;

Cana citireCanaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);

    char* aux;
    Cana c;

    aux = strtok(buffer, sep);
    c.id = atoi(aux);

    c.volum = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    c.culoare = malloc(strlen(aux) + 1);
    strcpy(c.culoare, aux);

    return c;
}

void afisareCana(Cana c) {
    printf("ID: %d, Volum: %.2fL, Culoare: %s\n", c.id, c.volum, c.culoare);
}

// Implementati modul de lucru cu stiva (LIFO) pe un vector
typedef struct {
    Cana elemente[MAX];
    int top;
} StivaVector;

void pushVector(StivaVector* s, Cana c) {
    if (s->top == MAX - 1) return;
    s->elemente[++(s->top)] = c;
}

Cana popVector(StivaVector* s) {
    if (s->top == -1) return (Cana) { -1, 0, NULL };
    return s->elemente[(s->top)--];
}

// Implementati modul de lucru cu stiva (LIFO) pe o stiva simplu inlantuita
typedef struct NodLSI {
    Cana info;
    struct NodLSI* next;
} NodLSI;

void pushLSI(NodLSI** varf, Cana c) {
    NodLSI* nou = malloc(sizeof(NodLSI));
    nou->info = c;
    nou->next = *varf;
    *varf = nou;
}

Cana popLSI(NodLSI** varf) {
    if (*varf == NULL) return (Cana) { -1, 0, NULL };
    NodLSI* temp = *varf;
    Cana c = temp->info;
    *varf = temp->next;
    free(temp);
    return c;
}

// Implementati modul de lucru cu stiva (LIFO) pe o lista dublu inlantuita
typedef struct NodLDI {
    Cana info;
    struct NodLDI* next;
    struct NodLDI* prev;
} NodLDI;

void pushLDI(NodLDI** varf, Cana c) {
    NodLDI* nou = malloc(sizeof(NodLDI));
    nou->info = c;
    nou->prev = NULL;
    nou->next = *varf;
    if (*varf) (*varf)->prev = nou;
    *varf = nou;
}

Cana popLDI(NodLDI** varf) {
    if (*varf == NULL) return (Cana) { -1, 0, NULL };
    NodLDI* temp = *varf;
    Cana c = temp->info;
    *varf = temp->next;
    if (*varf) (*varf)->prev = NULL;
    free(temp);
    return c;
}

// Implementati modul de lucru cu coada (FIFO) pe un vector
typedef struct {
    Cana elemente[MAX];
    int front, rear;
} CoadaVector;

void enqueueVector(CoadaVector* c, Cana e) {
    if (c->rear == MAX - 1) return;
    c->elemente[++(c->rear)] = e;
}

Cana dequeueVector(CoadaVector* c) {
    if (c->front > c->rear) return (Cana) { -1, 0, NULL };
    return c->elemente[(c->front)++];
}

// Implementati modul de lucru cu coada (FIFO) pe o lista simplu inlantuita
typedef struct NodCS {
    Cana info;
    struct NodCS* next;
} NodCS;

typedef struct {
    NodCS* first;
    NodCS* last;
} CoadaLSI;

void enqueueLSI(CoadaLSI* c, Cana e) {
    NodCS* nou = malloc(sizeof(NodCS));
    nou->info = e;
    nou->next = NULL;
    if (c->last) c->last->next = nou;
    else c->first = nou;
    c->last = nou;
}

Cana dequeueLSI(CoadaLSI* c) {
    if (c->first == NULL) return (Cana) { -1, 0, NULL };
    NodCS* temp = c->first;
    Cana e = temp->info;
    c->first = temp->next;
    if (c->first == NULL) c->last = NULL;
    free(temp);
    return e;
}

// Implementati modul de lucru cu coada (FIFO) pe o lista dublu inlantuita
typedef struct NodCD {
    Cana info;
    struct NodCD* next;
    struct NodCD* prev;
} NodCD;

typedef struct {
    NodCD* first;
    NodCD* last;
} CoadaLDI;

void enqueueLDI(CoadaLDI* c, Cana e) {
    NodCD* nou = malloc(sizeof(NodCD));
    nou->info = e;
    nou->next = NULL;
    nou->prev = c->last;
    if (c->last) c->last->next = nou;
    else c->first = nou;
    c->last = nou;
}

Cana dequeueLDI(CoadaLDI* c) {
    if (c->first == NULL) return (Cana) { -1, 0, NULL };
    NodCD* temp = c->first;
    Cana e = temp->info;
    c->first = temp->next;
    if (c->first) c->first->prev = NULL;
    else c->last = NULL;
    free(temp);
    return e;
}

int main() {
    FILE* f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis\n");
        return -1;
    }
    Cana c;
    StivaVector stivaV = { .top = -1 };
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        pushVector(&stivaV, c);
    }
    fclose(f);
    printf("\nStiva Vector (LIFO):\n");
    while (stivaV.top != -1) {
        Cana extras = popVector(&stivaV);
        afisareCana(extras);
        free(extras.culoare);
    }
    f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis pt LSI\n");
        return -1;
    }
    NodLSI* stivaLSI = NULL;
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        pushLSI(&stivaLSI, c);
    }
    fclose(f);
    printf("\nStiva LSI (LIFO):\n");
    while (stivaLSI != NULL) {
        Cana extras = popLSI(&stivaLSI);
        afisareCana(extras);
        free(extras.culoare);
    }
    f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis pt LDI\n");
        return -1;
    }
    NodLDI* stivaLDI = NULL;
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        pushLDI(&stivaLDI, c);
    }
    fclose(f);
    printf("\nStiva LDI (LIFO):\n");
    while (stivaLDI != NULL) {
        Cana extras = popLDI(&stivaLDI);
        afisareCana(extras);
        free(extras.culoare);
    }
    f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis pt Coada Vector\n");
        return -1;
    }
    CoadaVector coadaV = { .front = 0, .rear = -1 };
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        enqueueVector(&coadaV, c);
    }
    fclose(f);
    printf("\nCoada Vector (FIFO):\n");
    while (coadaV.front <= coadaV.rear) {
        Cana extras = dequeueVector(&coadaV);
        afisareCana(extras);
        free(extras.culoare);
    }
    f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis pt Coada LSI\n");
        return -1;
    }
    CoadaLSI coadaLSI = { .first = NULL, .last = NULL };
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        enqueueLSI(&coadaLSI, c);
    }
    fclose(f);
    printf("\nCoada LSI (FIFO):\n");
    while (coadaLSI.first != NULL) {
        Cana extras = dequeueLSI(&coadaLSI);
        afisareCana(extras);
        free(extras.culoare);
    }
    f = fopen("cani.txt", "r");
    if (!f) {
        printf("Fisier cani.txt nu s-a deschis pt Coada LDI\n");
        return -1;
    }
    CoadaLDI coadaLDI = { .first = NULL, .last = NULL };
    while (!feof(f)) {
        c = citireCanaDinFisier(f);
        enqueueLDI(&coadaLDI, c);
    }
    fclose(f);
    printf("\nCoada LDI (FIFO):\n");
    while (coadaLDI.first != NULL) {
        Cana extras = dequeueLDI(&coadaLDI);
        afisareCana(extras);
        free(extras.culoare);
    }
    return 0;
}


