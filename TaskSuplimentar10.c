#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	struct Nod* st;
	struct Nod* dr;
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

void adaugaMasinaInArbore(Nod** arbore, Masina m){
	if (*arbore) {
		if ((*arbore)->info.id > m.id) {
			adaugaMasinaInArbore(&(*arbore)->st, m);
		}
		else if ((*arbore)->info.id < m.id) {
			adaugaMasinaInArbore(&(*arbore)->dr, m);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = m;
		nou->st = NULL;
		nou->dr = NULL;
		*arbore = nou;
	}
}

Masina citireMasinaDinFisier(FILE* f) {
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
	strcpy(m.model, token);
	token = strtok(NULL, delimitator);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(m.numeSofer, token);
	token = strtok(NULL, delimitator);
	m.serie = *token;
	return m;
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Nr usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n\n", m.serie);
}

//RSD
void afisareArborePreordine(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->info);
		afisareArborePreordine(arbore->st);
		afisareArborePreordine(arbore->dr);
	}
}

//SRD
void afisareArboreInordine(Nod* arbore) {
	if (arbore) {
		afisareArboreInordine(arbore->st);
		afisareMasina(arbore->info);
		afisareArboreInordine(arbore->dr);
	}
}

//SDR
void afisareArborePostordine(Nod* arbore) {
	if (arbore) {
		afisareArborePostordine(arbore->st);
		afisareArborePostordine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArbore(Nod** arbore) {
	if (*arbore) {
		dezalocareArbore(&(*arbore)->st);
		dezalocareArbore(&(*arbore)->dr);
		if ((*arbore)->info.model != NULL) {
			free((*arbore)->info.model);
		}
		if ((*arbore)->info.numeSofer != NULL) {
			free((*arbore)->info.numeSofer);
		}
		free(*arbore);
		*arbore = NULL;
	}
}

int nrNoduri(Nod* arbore) {
	if (arbore) {
		int nrNoduriSt = nrNoduri(arbore->st);
		int nrNoduriDr = nrNoduri(arbore->dr);
		return nrNoduriSt + nrNoduriDr + 1;
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
		if (inaltimeSt > inaltimeDr) {
			return 1 + inaltimeSt;
		}
		else {
			return 1 + inaltimeDr;
		}
	}
	return 0;
}

float calculeazaPretulTotal(Nod* arbore) {
	if (arbore) {
		float pretSt = calculeazaPretulTotal(arbore->st);
		float pretDr = calculeazaPretulTotal(arbore->dr);
		return arbore->info.pret + pretSt + pretDr;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore) {
		float pretSt = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
		float pretDr = calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
		float sumaCrt = 0;
		if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
			sumaCrt = arbore->info.pret;
		}
		return sumaCrt + pretSt + pretDr;
	}
	return 0;
}

//shallow copy
Masina getMasinaById(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id > id) {
			return getMasinaById(arbore->st, id);
		}
		else if (arbore->info.id < id) {
			return getMasinaById(arbore->dr, id);
		}
		else {
			return arbore->info;
		}
	}
	return m;
}

//deep copy
Masina getMasina(Nod* arbore, int id) {
	Masina m = initializareMasina(0, 0, 0, NULL, NULL, '-');
	if (arbore) {
		if (arbore->info.id > id) {
			return getMasina(arbore->st, id);
		}
		else if (arbore->info.id < id) {
			return getMasina(arbore->dr, id);
		}
		else {
			return initializareMasina(arbore->info.id, arbore->info.nrUsi, arbore->info.pret, arbore->info.model, arbore->info.numeSofer, arbore->info.serie);
		}
	}
	return m;
}

Masina getMasinaPretMaxim(Nod* arbore) {
	if (arbore) {
		Masina masinaSt = getMasinaPretMaxim(arbore->st);
		Masina masinaDr = getMasinaPretMaxim(arbore->dr);
		Masina masinaPrexMax = arbore->info;// = initializareMasina(....) era deep
		if (masinaSt.pret > masinaPrexMax.pret) {
			masinaPrexMax = masinaSt;
		}
		if(masinaDr.pret > masinaPrexMax.pret){
			masinaPrexMax = masinaDr;
		}
		return masinaPrexMax;
	}
	else {
		return initializareMasina(0, 0, 0, NULL, NULL, '-');
	}
}

Nod* subarboreCuInaltimeMaiMare(Nod* arbore) {
	if (arbore) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
		if (inaltimeSt > inaltimeDr) {
			return arbore->st;
		}
		else {
			return arbore->dr;
		}
	}
	return NULL;
}

int nrNoduriSubordine(Nod* arbore) {
	if (arbore) {
		return nrNoduri(arbore) - 1;
	}
	return 0;
}

Nod* subarboreCuMaiMulteNoduri(Nod* arbore) {
	if (arbore) {
		int nrSt = nrNoduri(arbore->st);
		int nrDr = nrNoduri(arbore->dr);
		if (nrSt > nrDr) {
			return arbore->st;
		}
		else {
			return arbore->dr;
		}
	}
	return NULL;
}

void stergeNodDupaId(Nod** arbore, int id) {
	if (*arbore) {
		if (id < (*arbore)->info.id) {
			stergeNodDupaId(&(*arbore)->st, id);
		}
		else if (id > (*arbore)->info.id) {
			stergeNodDupaId(&(*arbore)->dr, id);
		}
		else {
			Nod* aux = NULL;
			//fara copii
			if ((*arbore)->st == NULL && (*arbore)->dr == NULL) {
				free(*arbore);
				*arbore = NULL;
			}
			//un singur copil - dr
			else if ((*arbore)->st == NULL) {
				aux = *arbore;
				*arbore = (*arbore)->dr;
				free(aux);
			}
			//un singur copil - st
			else if ((*arbore)->dr == NULL) {
				aux = *arbore;
				*arbore = (*arbore)->st;
				free(aux);
			}
			//2 copii
			else {
				Nod* minimDr = (*arbore)->dr;
				while (minimDr->st != NULL) {
					minimDr = minimDr->st;
				}
				(*arbore)->info = minimDr->info;
				stergeNodDupaId(&(*arbore)->dr, minimDr->info.id);
			}
		}
	}
}

//salveaza nodurile intr-o lista simpla inlantuita
struct NodLS {
	Masina info;
	struct NodLS* next;
};
typedef struct NodLS NodLS;

void inserareInceputListaSimpla(NodLS** lista, Masina m) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = m;
	nou->next = *lista;
	*lista = nou;
}

void inserareFinalListaSimpla(NodLS** lista, Masina m){
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = m;
	nou->next = NULL;
	if (*lista) {
		NodLS* aux = *lista;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}


void afisareListaSimpla(NodLS* lista){
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void salvareInListaSimpla(Nod* arbore,NodLS** listaSimpla) {
	if (arbore) {
		salvareInListaSimpla(arbore->st, listaSimpla);
		inserareFinalListaSimpla(listaSimpla, arbore->info);
		salvareInListaSimpla(arbore->dr, listaSimpla);
	}
}

void dezalocareListaSimpla(NodLS** lista) {
	while (*lista) {
		NodLS* p = *lista;
		*lista = (*lista)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

//salveaza nodurile intr-o lista dubla - prag pret 5000
struct NodLD {
	Masina info;
	struct NodLD* prev;
	struct NodLD* next;
};
typedef struct NodLD NodLD;

struct ListaDubla {
	NodLD* inceput;
	NodLD* final;
};
typedef struct ListaDubla ListaDubla;

void adaugaMasinaInListaFinal(ListaDubla* lista, Masina m) {
	NodLD* nou = (NodLD*)malloc(sizeof(NodLD));
	nou->info = m;
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

void afisareListaDublaDeLaInceput(ListaDubla lista) {
	NodLD* aux = lista.inceput;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void salvareInListaDubla(Nod* arbore, float pragPret ,ListaDubla* listaDubla) {
	if (arbore) {
		salvareInListaDubla(arbore->st, pragPret, listaDubla);
		if (arbore->info.pret >= pragPret) {
			adaugaMasinaInListaFinal(listaDubla, arbore->info);
		}
		salvareInListaDubla(arbore->dr, pragPret, listaDubla);
	}
}

void dezalocareListaDubla(ListaDubla* lista) {
	while (lista->inceput) {
		if (lista->inceput->info.model) {
			free(lista->inceput->info.model);
		}
		if (lista->inceput->info.numeSofer) {
			free(lista->inceput->info.numeSofer);
		}
		NodLD* aux = lista->inceput;
		lista->inceput = aux->next;
		free(aux);
	}
	lista->final = NULL;
}

//salveaza nodurile intr-un vector 
int nrNoduriPragPret(Nod* arbore, float pragPret) {
	if (arbore) {
		int nrSt = nrNoduriPragPret(arbore->st, pragPret);
		int nrDr = nrNoduriPragPret(arbore->dr, pragPret);
		int curent;
		if (arbore->info.pret >= pragPret) {
			curent = 1;
		}
		else {
			curent = 0;
		}
		return nrSt + nrDr + curent;
	}
	return 0;
}

void copiere(Nod* arbore, float pragPret, Masina* vector, int* index) {
	if (arbore) {
		copiere(arbore->st, pragPret, vector, index);
		if (arbore->info.pret >= pragPret) {
			vector[*index] = initializareMasina(arbore->info.id, arbore->info.nrUsi, arbore->info.pret, arbore->info.model, arbore->info.numeSofer, arbore->info.serie);
			(*index)++;
		}

		copiere(arbore->dr, pragPret, vector, index);
	}
}

Masina* copiereInVector(Nod* arbore, float pragPret, int* dim) {
	if (arbore) {
		*dim = nrNoduriPragPret(arbore, pragPret);
		if (*dim == 0) {
			return NULL;
		}
		else {
			int contor = 0;
			Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dim));
			copiere(arbore, pragPret, vector, &contor);
			return vector;
		}
	}
	return NULL;
}

void dezalocareVector(Masina** vector, int* dim) {
	for (int i = 0; i < (*dim); i++) {
		free((*vector)[i].numeSofer);
		free((*vector)[i].model);
	}
	free(*vector);
	*vector = NULL;
	*dim = 0;
}

int main() {
	Nod* arbore = citireArboreDinFisier("masini_arbore.txt");
	printf("Afisare preordine(RSD): \n");
	afisareArborePreordine(arbore);
	printf("\nAfisare in ordine(SRD): \n");
	afisareArboreInordine(arbore);
	printf("\nAfisare postordine(SDR): \n");
	afisareArborePostordine(arbore);

	printf("\nNr de noduri: %d\n", nrNoduri(arbore));
	printf("\nInaltimea arborelui: %d\n", calculeazaInaltimeArbore(arbore));
	printf("\nPretul total al masinilor: %.2f\n", calculeazaPretulTotal(arbore));
	printf("\nPretul masinilor unui sofer: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	Masina m = getMasinaById(arbore, 1);
	afisareMasina(m);

	Masina mDeep = getMasina(arbore, 2);
	afisareMasina(mDeep);
	if (mDeep.model != NULL) {
		free(mDeep.model);
		mDeep.model = NULL;
	}
	if (mDeep.numeSofer != NULL) {
		free(mDeep.numeSofer);
		mDeep.numeSofer = NULL;
	}

	printf("\nMasina cu pretul maxim: \n");
	Masina mPretMaxim = getMasinaPretMaxim(arbore);
	afisareMasina(mPretMaxim);

	Nod* subarboreInaltimeMax = subarboreCuInaltimeMaiMare(arbore);
	printf("\nRadacina subarborelui cu inaltime cea mai mare: \n");
	afisareMasina(subarboreInaltimeMax->info);

	printf("\nNr noduri aflate in subordine radacinii: %d\n", nrNoduriSubordine(arbore));

	Nod* subarboreMulteNoduri = subarboreCuMaiMulteNoduri(arbore);
	printf("\nRadacina subarborelui cu cele mai multe noduri: \n");
	afisareMasina(subarboreMulteNoduri->info);

	printf("\nAfisare lista simpla cu noduri arbore: \n");
	NodLS* listaSimpla = NULL;
	salvareInListaSimpla(arbore, &listaSimpla);
	afisareListaSimpla(listaSimpla);
	//dezalocareListaSimpla(&listaSimpla); //-> doar daca fac deep copy
	while (listaSimpla) {
		NodLS *aux = listaSimpla;
		listaSimpla = listaSimpla->next;
		free(aux);
	}

	printf("\nAfisare lista dubla cu noduri arbore: \n");
	ListaDubla lista;
	lista.inceput = NULL;
	lista.final = NULL;
	salvareInListaDubla(arbore, 5000, &lista);
	afisareListaDublaDeLaInceput(lista);
	//dezalocareListaDubla(&lista); // -> doar pt deep copy
	while (lista.inceput) {
		NodLD* aux = lista.inceput;
		lista.inceput = aux->next;
		free(aux);
	}
	lista.final = NULL;

	int dim;
	Masina* vector = copiereInVector(arbore, 8000, &dim);
	if (vector == NULL) {
		printf("Nu exista masini cu pretul mai mare sau egal cu 8000.\n");
	}
	else {
		printf("Masina/Masinile cu pretul mai mare sau egal cu 8000: \n");
		for (int i = 0; i < dim; i++) {
			afisareMasina(vector[i]);
		}
		dezalocareVector(&vector, &dim);
	}

	printf("\nArbore dupa stergerea unui nod cu id dat: \n");
	stergeNodDupaId(&arbore, 7);
	afisareArboreInordine(arbore);

	dezalocareArbore(&arbore);

	return 0;
}