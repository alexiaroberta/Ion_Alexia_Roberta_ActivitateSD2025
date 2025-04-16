#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <String.h>

struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct Masina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

Masina initializareMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	if (model != NULL) {
		m.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
		strcpy_s(m.model, strlen(model) + 1, model);
	}
	else {
		m.model = NULL;
	}
	if (numeSofer != NULL) {
		m.numeSofer = (char*)malloc(sizeof(char) * (strlen(numeSofer) + 1));
		strcpy_s(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
	}
	else {
		m.numeSofer = NULL;
	}
	m.serie = serie;
	return m;
}

void inserareSortataPretMasina(Nod** lista, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;
	if (*lista) {
		if ((*lista)->info.pret > nou->info.pret) {
			nou->next = *lista;
			*lista = nou;
		}
		else {
			Nod* aux = *lista;
			while (aux->next != NULL && aux->info.pret < nou->info.pret) {
				aux = aux->next;
			}
			if (aux->next != NULL) {
				nou->next = aux->next;
				aux->next = nou;
			}
			else {
				aux->next = nou;
			}
		}
	}
	else {
		*lista = nou;
	}
}

Masina citesteMasinaDinFisier(FILE* f) {
	char buffer[100];
	char delimitator[] = ",\n";
	fgets(buffer, sizeof(buffer), f);
	Masina m;
	char* token = strtok(buffer, delimitator);
	m.id = atoi(token);
	token = strtok(NULL, delimitator);
	m.nrUsi = atoi(token);
	token = strtok(NULL, delimitator);
	m.pret = atof(token);
	token = strtok(NULL, delimitator);
	m.model = (char*)malloc(sizeof(char) * (strlen(token) + 1));
	strcpy_s(m.model, strlen(token) + 1, token);
	token = strtok(NULL, delimitator);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(token) + 1));
	strcpy_s(m.numeSofer, strlen(token) + 1, token);
	token = strtok(NULL, delimitator);
	m.serie = *token;
	return m;
}

Nod* citireListaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f)) {
		Masina m = citesteMasinaDinFisier(f);
		inserareSortataPretMasina(&lista, m);
	}
	fclose(f);
	return lista;
}

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Nr usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n\n", m.serie);
}

void afisareLista(Nod* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

Nod* stergeNodDePePozitia(Nod* lista, int pozitia) {
	if (lista == NULL || pozitia < 0) {
		return lista;
	}
	if (pozitia == 0) {
		Nod* temp = lista;
		lista = lista->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
		return lista;
	}
	Nod* curent = lista;
	int i = 0;
	while (curent != NULL && i < pozitia - 1) {
		curent = curent->next;
		i++;
	}
	if (curent == NULL || curent->next == NULL) {
		return lista;
	}
	Nod* aux = curent->next;
	curent->next = aux->next;
	free(aux->info.model);
	free(aux->info.numeSofer);
	free(aux);

	return lista;
}

void interschimbaNoduri(Nod* lista, int poz1, int poz2) {
	if (lista != NULL && poz1 > 0 && poz2 > 0 && poz1 != poz2) {
		Nod* nod1 = NULL;
		Nod* nod2 = NULL;
		int index = 0;

		while (lista != NULL) {
			if (index == poz1) {
				nod1 = lista;
			}
			if (index == poz2) {
				nod2 = lista;
			}
			lista = lista->next;
			index++;
		}

		if (nod1 != NULL && nod2 != NULL) {
			Masina aux = nod1->info;
			nod1->info = nod2->info;
			nod2->info = aux;
		}
	}
}


Masina* salvareInVectorMasinaPragPret(Nod* lista, int* dim, float pragPret) {
	*dim = 0;
	Nod* aux = lista;
	while (aux) {
		if (aux->info.pret <= pragPret) {
			(*dim)++;
		}
		aux = aux->next;
	}
	if (*dim) {
		int contor = 0;
		Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dim));
		while (lista) {
			if (lista->info.pret <= pragPret) {
				vector[contor++] = initializareMasina(lista->info.id, lista->info.nrUsi, lista->info.pret, lista->info.model, lista->info.numeSofer, lista->info.serie);
			}
			lista = lista->next;
		}
		return vector;
	}
	else {
		return NULL;
	}
	return NULL;
}

void dezalocareVector(Masina** vector, int* dim) {
	for (int i = 0; i < (*dim); i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*dim = 0;
}

void dezalocareLista(Nod** lista) {
	while (*lista) {
		Nod* p = *lista;
		*lista = (*lista)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

int main() {
	Nod* lista = citireListaDinFisier("masini.txt");
	afisareLista(lista);

	int nr = 0;
	float prag = 8000;
	Masina* vector = salvareInVectorMasinaPragPret(lista, &nr, prag);
	if (vector == NULL) {
		printf("Nu exista masini cu pretul mai mic sau egal cu 8000.\n");
	}
	else {
		printf("Masina/Masinile cu pretul mai mic sau egal cu 8000: \n");
		for (int i = 0; i < nr; i++) {
			afisareMasina(vector[i]);
		}
		dezalocareVector(&vector, &nr);
	}

	int pozitie = 2;
	printf("\nStergere nod de pe pozitia: %d\n",pozitie);
	lista = stergeNodDePePozitia(lista, pozitie);
	afisareLista(lista);

	printf("Interschimbare noduri: \n");
	int poz1 = 1, poz2 = 2;
	interschimbaNoduri(lista, poz1, poz2);
	afisareLista(lista);

	dezalocareLista(&lista);
	return 0;
}