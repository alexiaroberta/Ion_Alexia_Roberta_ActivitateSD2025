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
	struct Nod* nodSt;
	struct Nod* nodDr;
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

void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore)) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->nodSt = NULL;
		nou->nodDr = NULL;
		(*arbore) = nou;
	}
	else {
		if ((*arbore)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->nodSt), masinaNoua);
		}
		else if ((*arbore)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->nodDr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdineSRD(Nod* arbore) {
	if (arbore) {
		afisareInOrdineSRD(arbore->nodSt);
		afisareMasina(arbore->info);
		afisareInOrdineSRD(arbore->nodDr);
	}
}

void afisarePreordineRSD(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->info);
		afisarePreordineRSD(arbore->nodSt);
		afisarePreordineRSD(arbore->nodDr);
	}
}

void afisarePostordineSDR(Nod* arbore) {
	if (arbore) {
		afisarePostordineSDR(arbore->nodSt);
		afisarePostordineSDR(arbore->nodDr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->nodSt);
		dezalocareArboreDeMasini(&(*arbore)->nodDr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		*arbore = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id < id) {
			return getMasinaByID(arbore->nodDr, id);
		}
		else if (arbore->info.id > id) {
			return getMasinaByID(arbore->nodSt, id);
		}
		else {
			return arbore->info;
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore) {
		int noduriSt = determinaNumarNoduri(arbore->nodSt);
		int noduriDr = determinaNumarNoduri(arbore->nodDr);
		return 1 + noduriSt + noduriDr;
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->nodSt);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->nodDr);
		if (inaltimeSt > inaltimeDr)
			return 1 + inaltimeSt;
		else
			return 1 + inaltimeDr;
	}
	return 0;
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore) {
		float totalSt = calculeazaPretTotal(arbore->nodSt);
		float totalDr = calculeazaPretTotal(arbore->nodDr);
		return arbore->info.pret + totalSt + totalDr;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore) {
		float totalSt = calculeazaPretulMasinilorUnuiSofer(arbore->nodSt, numeSofer);
		float totalDr = calculeazaPretulMasinilorUnuiSofer(arbore->nodDr, numeSofer);
		float sumaCrt = 0;
		if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
			sumaCrt = arbore->info.pret;
		}
		return sumaCrt + totalSt + totalDr;
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("Afisare in ordine: \n");
	afisareInOrdineSRD(radacina);
	printf("\nAfisare preordine: \n");
	afisarePreordineRSD(radacina);
	printf("\nAfisare postordine: \n");
	afisarePostordineSDR(radacina);

	int id = 8;
	printf("\nMasina cu id-ul: %d\n", id);
	Masina m = getMasinaByID(radacina, id);
	afisareMasina(m);

	float pret = calculeazaPretTotal(radacina);
	printf("\nPretul total al masinilor: %.2f\n", pret);

	float suma = calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu");
	printf("\nSuma masinilor preturilor soferului Ionescu: %.2f\n", suma);

	int nrNoduri = determinaNumarNoduri(radacina);
	printf("\nNumarul de noduri: %d\n", nrNoduri);

	int inaltimeArbore = calculeazaInaltimeArbore(radacina);
	printf("\nInaltime arborelui: %d\n", inaltimeArbore);

	dezalocareArboreDeMasini(&radacina);
	return 0;
}