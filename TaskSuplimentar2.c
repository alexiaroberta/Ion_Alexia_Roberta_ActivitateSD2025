#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Alexia Ion -> A si I
struct Cantina {
	char* denumire;
	int nrAngajati;
	int nrPreparate;
	float* preturi;
};

typedef struct Cantina Cantina;

Cantina initializare(const char* denumire, int nrAngajati, int nrPreparate, float* preturi) {
	Cantina c;
	if (denumire != NULL) {
		c.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
		strcpy_s(c.denumire, strlen(denumire) + 1, denumire);
	}
	else {
		c.denumire = NULL;
	}
	c.nrAngajati = nrAngajati;
	c.nrPreparate = nrPreparate;
	if (nrPreparate > 0) {
		c.preturi = (float*)malloc(sizeof(float) * nrPreparate);
		for (int i = 0; i < c.nrPreparate; i++) {
			c.preturi[i] = preturi[i];
		}
	}
	else {
		c.preturi = NULL;
	}
	return c;
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

void copiereCantineNrMicAngajati(Cantina* vector, int nrElemente, int pragAngajati, Cantina** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].nrAngajati <= pragAngajati) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc(sizeof(Cantina) * (*dimensiune));
	int contor = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].nrAngajati <= pragAngajati) {
			(*vectorNou)[contor] = initializare(vector[i].denumire, vector[i].nrAngajati,
				vector[i].nrPreparate, vector[i].preturi);
			contor++;
		}
	}
}

void concatenare(Cantina* vector1, int nrElemente1, Cantina* vector2, int nrElemente2, Cantina** vectorNou, int* nrElementeNou) {
	*nrElementeNou = nrElemente1 + nrElemente2;
	*vectorNou = malloc(sizeof(Cantina) * (*nrElementeNou));

	for (int i = 0; i < nrElemente1; i++) {
		(*vectorNou)[i] = vector1[i];
		(*vectorNou)[i].denumire = malloc(sizeof(char) * (strlen(vector1[i].denumire) + 1));
		strcpy_s((*vectorNou)[i].denumire, strlen(vector1[i].denumire) + 1, vector1[i].denumire);
		(*vectorNou)[i].preturi = malloc(sizeof(float) * vector1[i].nrPreparate);
		for (int j = 0; j < vector1[i].nrPreparate; j++) {
			(*vectorNou)[i].preturi[j] = vector1[i].preturi[j];
		}
	}

	for (int i = 0; i < nrElemente2; i++) {
		(*vectorNou)[nrElemente1 + i] = vector2[i];
		(*vectorNou)[nrElemente1 + i].denumire = malloc(sizeof(char) * (strlen(vector2[i].denumire) + 1));
		strcpy_s((*vectorNou)[nrElemente1 + i].denumire, strlen(vector2[i].denumire) + 1, vector2[i].denumire);
		(*vectorNou)[nrElemente1 + i].preturi = malloc(sizeof(float) * vector2[i].nrPreparate);
		for (int j = 0; j < vector2[i].nrPreparate; j++) {
			(*vectorNou)[nrElemente1 + i].preturi[j] = vector2[i].preturi[j];
		}

	}
}

void dezalocare(Cantina** vector, int* nrElemente) {
	if ((*vector) != NULL && (*nrElemente) > 0) {
		for (int i = 0; i < (*nrElemente); i++) {
			free((*vector)[i].denumire);
			free((*vector)[i].preturi);
		}
		free(*vector);
		*vector = NULL;
		*nrElemente = 0;
	}
}

int main() {
	int nrElemente = 5;
	Cantina* vector = malloc(sizeof(Cantina) * nrElemente);
	vector[0] = initializare("Moxa", 15, 3, (float[]) { 12.5, 15, 14 });
	vector[1] = initializare("Leu", 20, 4, (float[]) { 20.5, 21, 14, 18, 29 });
	vector[2] = initializare("Agronomie", 13, 2, (float[]) { 24, 26 });
	vector[3] = initializare("Lalala", 16, 3, (float[]) { 23, 21, 25 });
	vector[4] = initializare("Cehov", 15, 5, (float[]) { 21, 24, 23, 19, 20 });
	afisareVector(vector, nrElemente);

	printf("\n\nCantine cu nr mic de angajati: \n");
	int dim = 0, prag = 15;
	Cantina* vectorNou = NULL;
	copiereCantineNrMicAngajati(vector, nrElemente, prag, &vectorNou, &dim);
	afisareVector(vectorNou, dim);
	dezalocare(&vectorNou, &dim);

	printf("\n\nConcatenare: \n");
	int nrElemente2 = 2;
	Cantina* vector2 = malloc(sizeof(Cantina) * nrElemente2);
	vector2[0] = initializare("ABC", 15, 2, (float[]) { 15, 14 });
	vector2[1] = initializare("DEF", 20, 3, (float[]) { 14, 18, 29 });
	concatenare(vector, nrElemente, vector2, nrElemente2, &vectorNou, &dim);
	afisareVector(vectorNou, dim);
	dezalocare(&vectorNou, &dim);

	dezalocare(&vector, &nrElemente);
	return 0;
}
