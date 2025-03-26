#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Cantina {
	char* denumire;
	int nrAngajati;
	int nrPreparate;
	float* preturi;
};

typedef struct Cantina Cantina;

void adaugaElementeInVector(Cantina** vector, int* nrElemente, Cantina c) {
	Cantina* aux = (Cantina*)malloc(sizeof(Cantina) * ((*nrElemente) + 1));
	for (int i = 0; i < (*nrElemente); i++) {
		aux[i] = (*vector)[i];
	}
	aux[(*nrElemente)] = c;
	free(*vector);
	*vector = aux;
	(*nrElemente)++;
}

void citireDinFisier(const char* numeFisier, Cantina** vector, int* nrElemente) {
	if (numeFisier != NULL && strlen(numeFisier) > 0) {
		FILE* f = fopen(numeFisier, "r");
		if (f != NULL) {
			char delimitator[] = " ,\n";
			char buffer[100];
			while (fgets(buffer, sizeof(buffer), f) != NULL) {
				char* token;
				Cantina c;
				token = strtok(buffer, delimitator);
				c.denumire = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy_s(c.denumire, strlen(token) + 1, token);
				token = strtok(NULL, delimitator);
				c.nrAngajati = atoi(token);
				token = strtok(NULL, delimitator);
				c.nrPreparate = atoi(token);
				if (c.nrPreparate > 0) {
					c.preturi = (float*)malloc(sizeof(float) * (c.nrPreparate));
					for (int i = 0; i < c.nrPreparate; i++) {
						token = strtok(NULL, delimitator);
						c.preturi[i] = atof(token);
					}
				}
				adaugaElementeInVector(vector, nrElemente, c);
			}
		}
		fclose(f);
	}
}

void afisare(Cantina c) {
	printf("\nDenumire: %s\n", c.denumire);
	printf("Nr angajati: %d\n", c.nrAngajati);
	printf("Nr preparate: %d\n", c.nrPreparate);
	if (c.nrPreparate > 0 && c.preturi != NULL) {
		printf("Preturi: \n");
		for (int i = 0; i < c.nrPreparate; i++) {
			printf("%.2f ", c.preturi[i]);
		}
	}
}

void afisareVector(Cantina* vector, int nrElemente) {
	if (nrElemente > 0 && vector != NULL) {
		for (int i = 0; i < nrElemente; i++) {
			afisare(vector[i]);
			printf("\n");
		}
	}
}

void salveazaObiectInFisier(const char* numeFisier, Cantina c) {
	FILE* f = fopen(numeFisier, "w");
	if (f != NULL) {
		fprintf(f, "%s, %d, %d, ", c.denumire, c.nrAngajati, c.nrPreparate);
		for (int i = 0; i < c.nrPreparate; i++) {
			fprintf(f, "%.2f ", c.preturi[i]);
		}
	}
	fclose(f);
}

void salvareVectorInFisier(const char* numeFisier, Cantina* vector, int nrElemente) {
	FILE* f = fopen(numeFisier, "w");
	if (f != NULL) {
		for (int i = 0; i < nrElemente; i++) {
			fprintf(f, "%s, %d, %d, ", vector[i].denumire, vector[i].nrAngajati, vector[i].nrPreparate);
			for (int j = 0; j < vector[i].nrPreparate; j++) {
				fprintf(f, "%.2f ", vector[i].preturi[j]);
			}
			fprintf(f, "\n");
		}
	}
	fclose(f);
}

void dezalocare(Cantina** vector, int* nrElemente) {
	if ((*vector) != NULL && (*nrElemente) > 0) {
		for (int i = 0; i < (*nrElemente); i++) {
			if ((*vector)[i].denumire != NULL) {
				free((*vector)[i].denumire);
			}
			if ((*vector)[i].preturi != NULL) {
				free((*vector)[i].preturi);
			}
		}
		free(*vector);
		*vector = NULL;
		*nrElemente = 0;
	}
}

int main() {
	Cantina* vector = NULL;
	int dim = 0;
	citireDinFisier("cantine.txt", &vector, &dim);
	afisareVector(vector, dim);
	salveazaObiectInFisier("cantina.txt", vector[0]);
	salvareVectorInFisier("cantineSalvare.txt", vector, dim);
	dezalocare(&vector, &dim);
	return 0;
}