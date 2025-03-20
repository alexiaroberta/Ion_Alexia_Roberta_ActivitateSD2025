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
};

typedef struct Nod Nod;

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

void afisareListaMasini(Nod* nod) {
	while (nod) {
		afisareMasina(nod->info);
		nod = nod->next;
	}
}

void adaugaMasinaInListaFinal(Nod** lista, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if ((*lista) == NULL) {
		*lista = nodNou;
	}
	else {
		Nod* aux = *lista;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *lista;
	*lista = nodNou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInListaFinal(&lista, masina);
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	while (*lista) {
		Nod* p = *lista;
		*lista = (*lista)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

float calculeazaPretMediu(Nod* lista) {
	float suma = 0;
	int contor = 0;
	while (lista) {
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}
	if (contor == 0) {
		return 0;
	}
	else {
		return suma / contor;
	}
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	Nod* p = *lista;
	while (p != NULL && p->info.serie == serieCautata) {
		Nod* sters = p;
		p = p->next;

		free(sters->info.model);
		free(sters->info.numeSofer);
		free(sters);
	}

	*lista = p;
	if (p != NULL) {
		Nod* ant = p;
		Nod* curent = p->next;
		while (curent != NULL) {
			if (curent->info.serie == serieCautata) {
				Nod* sters = curent;
				ant->next = curent->next;
				curent = curent->next;

				free(sters->info.model);
				free(sters->info.numeSofer);
				free(sters);
			}
			else {
				ant = curent;
				curent = curent->next;
			}
		}

	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}

int getNrUsiMasinaScumpa(Nod* lista) {
	if (lista) {
		float pretMaxim = lista->info.pret;
		int nrUsi = lista->info.nrUsi;
		lista = lista->next;
		while (lista) {
			if (lista->info.pret > pretMaxim) {
				pretMaxim = lista->info.pret;
				nrUsi = lista->info.nrUsi;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
	return 0;
}

int main() {
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	float medie = calculeazaPretMediu(lista);
	printf("Pret mediu: %.2f\n", medie);
	float sumaSofer = calculeazaPretulMasinilorUnuiSofer(lista, "Ionescu");
	printf("Suma sofer: %.2f\n", sumaSofer);
	int nrUsi = getNrUsiMasinaScumpa(lista);
	printf("Nr usi masina scumpa: %d\n", nrUsi);
	stergeMasiniDinSeria(&lista, 'J');
	afisareListaMasini(lista);
	dezalocareListaMasini(&lista);
	return 0;
}