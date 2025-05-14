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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = *stiva;
	*stiva = nou;
}

Masina popStack(Nod** stiva) {
	if (*stiva) {
		Nod* temp = (*stiva);
		Masina m = temp->info;
		(*stiva) = temp->next;
		free(temp);
		return m;
	}
	else {
		return (Masina) { -1, 0, 0, NULL, NULL, '-' };
	}
}

unsigned char isStackEmpty(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* stiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while (*stiva) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* stiva) {//returneaza numarul de elemente din stiva
	int contor = 0;
	while (stiva) {
		contor++;
		stiva = stiva->next;
	}
	return contor;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
struct NodDublu {
	Masina info;
	struct NodDublu* prev;
	struct NodDublu* next;
};
typedef struct NodDublu NodDublu;

struct ListaDubla {
	NodDublu* first;
	NodDublu* last;
};
typedef struct ListaDubla ListaDubla;

void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->last;
	if (coada->last) {
		coada->last->next = nou;
	}
	else {
		coada->first = nou;
	}
	coada->last = nou;
}

Masina dequeue(ListaDubla* coada) {
	if (coada->first) {
		Masina m = coada->first->info;
		NodDublu* temp = coada->first;
		coada->first = coada->first->next;
		free(temp);
		if (coada->first == NULL) {
			coada->last = NULL;
		}
		else {
			coada->first->prev = NULL;
		}
		return m;
	}
	else {
		Masina m;
		m.id = -1;
		return m;
	}
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla coada;
	coada.first = NULL;
	coada.last = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(ListaDubla* coada) {
	while (coada->first) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

float calculeazaPretTotalStiva(Nod** stiva) {
	Nod* temp = NULL;
	float suma = 0;
	while (!isStackEmpty(*stiva)) {
		Masina m = popStack(stiva);
		suma += m.pret;
		pushStack(&temp, m);
	}
	while (!isStackEmpty(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return suma;
}

float calculeazaPretTotalCoada(ListaDubla* coada) {
	ListaDubla temp;
	temp.first = NULL;
	temp.last = NULL;
	float suma = 0;
	while (coada->first) {
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&temp, m);
	}
	while (temp.first) {
		enqueue(coada, dequeue(&temp));
	}
	return suma;
}

Masina getMasinaByIDStiva(Nod** stiva, int id) {
	Nod* temp = NULL;
	Masina rezultat = (Masina){ -1, 0, 0, NULL, NULL, '-' };
	while (!isStackEmpty(*stiva)) {
		Masina masina = popStack(stiva);
		if (masina.id == id && rezultat.id == -1) {
			rezultat = masina;
		}
		pushStack(&temp, masina);
	}
	while (!isStackEmpty(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return rezultat;
}

Masina getMasinaByIDCoada(ListaDubla* coada, int id) {
	ListaDubla temp;
	temp.first = NULL;
	temp.last = NULL;
	Masina rezultat = (Masina){ -1, 0, 0, NULL, NULL, '-' };
	while (coada->first) {
		Masina m = dequeue(coada);
		if (m.id == id && rezultat.id == -1) {
			rezultat = m;
		}
		enqueue(&temp, m);
	}
	while (temp.first) {
		enqueue(coada, dequeue(&temp));
	}
	return rezultat;
}

int main() {
	printf("---Stiva---\n");
	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	Masina m1 = popStack(&stiva);
	afisareMasina(m1);

	int nrElementeStiva = size(stiva);
	printf("Nr de elemente din stiva: %d\n\n", nrElementeStiva);
	printf("Pret total: %.2f\n\n", calculeazaPretTotalStiva(&stiva));
	printf("Afisare masina cautata dupa id: \n");
	afisareMasina(getMasinaByIDStiva(&stiva, 2));

	dezalocareStivaDeMasini(&stiva);

	printf("---Coada---\n");
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	Masina m2 = dequeue(&coada);
	afisareMasina(m2);

	printf("Pret total: %.2f\n\n", calculeazaPretTotalCoada(&coada));
	printf("Afisare masina cautata dupa id: \n");
	afisareMasina(getMasinaByIDCoada(&coada, 3));

	dezalocareCoadaDeMasini(&coada);
	return 0;
}