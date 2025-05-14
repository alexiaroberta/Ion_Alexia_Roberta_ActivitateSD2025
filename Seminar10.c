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
	struct Nod* st;
	struct Nod* dr;
	int gradEchilibru;
};
typedef struct Nod Nod;

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
		if (inaltimeSt > inaltimeDr)
			return 1 + inaltimeSt;
		else
			return 1 + inaltimeDr;
	}
	return 0;
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore) {
		int noduriSt = determinaNumarNoduri(arbore->st);
		int noduriDr = determinaNumarNoduri(arbore->dr);
		return 1 + noduriSt + noduriDr;
	}
	return 0;
}

void calculGradEchilibru(Nod* arbore) {
	if (arbore) {
		arbore->gradEchilibru = calculeazaInaltimeArbore(arbore->dr) - calculeazaInaltimeArbore(arbore->st);
	}
}

//rotire simpla dreapta
Nod* rotireSimplaDreapta(Nod* pivot, Nod* fiuSt) {
	pivot->st = fiuSt->dr;
	calculGradEchilibru(pivot);
	fiuSt->dr = pivot;
	calculGradEchilibru(fiuSt);

	return fiuSt;
}

//rotire simpla stanga
Nod* rotireSimplaStanga(Nod* pivot, Nod* fiuDr) {
	pivot->dr = fiuDr->st;
	calculGradEchilibru(pivot);
	fiuDr->st = pivot;
	calculGradEchilibru(fiuDr);

	return fiuDr;
}

//rotire dubla stanga dreapta
Nod* rotireDublaStangaDreapta(Nod* pivot, Nod* fiuSt) {
	//aducerea dezechilibrului pe partea stanga
	pivot->st = rotireSimplaStanga(fiuSt, fiuSt->dr);
	calculGradEchilibru(pivot);
	fiuSt = pivot->st;
	//rotire propriu-zisa in pivot
	fiuSt = rotireSimplaDreapta(pivot, fiuSt);
	calculGradEchilibru(fiuSt);

	return fiuSt;
}

//rotire dubla dreapta stanga
Nod* rotireDublaDreaptaStanga(Nod* pivot, Nod* fiuDr) {
	//aducerea dezechilibrului pe partea dreapta
	pivot->dr = rotireSimplaDreapta(fiuDr, fiuDr->st);
	calculGradEchilibru(pivot);
	fiuDr = pivot->dr;
	//rotire propriu-zisa in pivot
	fiuDr = rotireSimplaStanga(pivot, fiuDr);
	calculGradEchilibru(fiuDr);

	return fiuDr;
}

void inserareInArbore(Nod** radacina, Masina m) {
	if (*radacina) {
		if ((*radacina)->info.id > m.id) {
			inserareInArbore(&(*radacina)->st, m);
		}
		else if ((*radacina)->info.id < m.id) {
				inserareInArbore(&(*radacina)->dr, m);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = m;
		nou->st = NULL;
		nou->dr = NULL;
		*radacina = nou;
	}
	calculGradEchilibru(*radacina);
	if ((*radacina)->gradEchilibru == 2) {
		if ((*radacina)->dr->gradEchilibru == -1) {
			//rotire dubla dreapta stanga
			*radacina = rotireDublaDreaptaStanga(*radacina, (*radacina)->dr);
		}
		else {
			//dezechilibru dreapta
			*radacina = rotireSimplaStanga(*radacina, (*radacina)->dr);
		}
	}
	else if ((*radacina)->gradEchilibru == -2) {
		if ((*radacina)->st->gradEchilibru == 1) {
			//rotire dubla stanga dreapta
			*radacina = rotireDublaStangaDreapta(*radacina, (*radacina)->st);
		}
		else {
			//dezechilibru stanga
			*radacina = rotireSimplaDreapta(*radacina, (*radacina)->st);
		}
	}
}

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

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)){
		Masina m = citireMasinaDinFisier(f);
		inserareInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//RSD
void afisarePreOrdine(Nod* arbore) {
	if (arbore){
		afisareMasina(arbore->info); 
		afisarePreOrdine(arbore->st); 
		afisarePreOrdine(arbore->dr); 
	}
}

//SRD
void afisareInOrdine(Nod* arbore) {
	if (arbore) {
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

//SDR
void afisarePostOrdine(Nod* arbore) {
	if (arbore) {
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
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
			return getMasinaByID(arbore->dr, id);
		}
		else if (arbore->info.id > id) {
			return getMasinaByID(arbore->st, id);
		}
		else {
			return arbore->info;
		}
	}
	return m;
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore) {
		float totalSt = calculeazaPretTotal(arbore->st);
		float totalDr = calculeazaPretTotal(arbore->dr);
		return arbore->info.pret + totalSt + totalDr;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore) {
		float totalSt = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
		float totalDr = calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
		float sumaCrt = 0;
		if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
			sumaCrt = arbore->info.pret;
		}
		return sumaCrt + totalSt + totalDr;
	}
	return 0;
}

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("Afisare in ordine: \n");
	afisareInOrdine(arbore);
	printf("\nAfisare preordine: \n");
	afisarePreOrdine(arbore);
	printf("\nAfisare postordine: \n");
	afisarePostOrdine(arbore);

	int id = 8;
	printf("\nMasina cu id-ul: %d\n", id);
	Masina m = getMasinaByID(arbore, id);
	afisareMasina(m);

	float pret = calculeazaPretTotal(arbore);
	printf("\nPretul total al masinilor: %.2f\n", pret);

	float suma = calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");
	printf("\nSuma masinilor preturilor soferului Ionescu: %.2f\n", suma);

	int nrNoduri = determinaNumarNoduri(arbore);
	printf("\nNumarul de noduri: %d\n", nrNoduri);

	int inaltimeArbore = calculeazaInaltimeArbore(arbore);
	printf("\nInaltime arborelui: %d\n", inaltimeArbore);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}