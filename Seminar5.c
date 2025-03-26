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
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* inceput;
	Nod* final;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.inceput;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaFinal(ListaDubla lista) {
	Nod* p = lista.final;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInListaLaFinal(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = lista->final;
	nou->next = NULL;
	if (lista->final != NULL) {
		lista->final->next = nou;
	}
	else {
		lista->inceput = nou;
	}
	lista->final = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = lista->inceput;
	if (lista->inceput != NULL) {
		lista->inceput->prev = nou;
	}
	else {
		lista->final = nou;
	}
	lista->inceput = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.inceput = NULL;
	lista.final = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		//adaugaMasinaInListaLaFinal(&lista, m);
		adaugaLaInceputInLista(&lista, m);
	}
	return lista;
	fclose(f);
}

void dezalocareLDMasini(ListaDubla* lista) {
	while (lista->inceput) {
		if (lista->inceput->info.model != NULL) {
			free(lista->inceput->info.model);
		}
		if (lista->inceput->info.numeSofer != NULL) {
			free(lista->inceput->info.numeSofer);
		}
		Nod* p = lista->inceput;
		lista->inceput = p->next;
		free(p);
	}
	lista->final = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	float suma = 0;
	int contor = 0;
	Nod* temp = lista.inceput;
	while (temp) {
		suma += temp->info.pret;
		contor++;
		temp = temp->next;
	}
	if (contor == 0) {
		return 0;
	}
	else {
		return suma / contor;
	}
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	Nod* p = lista->inceput;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				lista->final = p->prev;
			}
		}
		else {
			lista->inceput = p->next;
			if (p->next) {
				p->next->prev = NULL;
			}
			else {
				lista->final = NULL;
			}
		}
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	Nod* p = lista.inceput;
	Nod* maxPretMasina = p;
	if (lista.inceput) {
		while (p) {
			if (p->info.pret > maxPretMasina->info.pret) {
				maxPretMasina = p;
			}
			p = p->next;
		}
		char* nume = malloc(sizeof(char) * (strlen(maxPretMasina->info.numeSofer) + 1));
		strcpy_s(nume, strlen(maxPretMasina->info.numeSofer) + 1, maxPretMasina->info.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	afisareListaMasiniDeLaFinal(lista);
	float pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu este: %.2f\n", pretMediu);
	stergeMasinaDupaID(&lista, 2);
	afisareListaMasiniDeLaInceput(lista);
	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("\nNume sofer cea mai scumpa masina: %s\n", nume);
	free(nume);
	dezalocareLDMasini(&lista);
	return 0;
}