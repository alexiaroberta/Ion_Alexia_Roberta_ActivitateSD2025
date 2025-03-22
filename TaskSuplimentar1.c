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

Cantina citire() {
	Cantina c;

	char buffer[100];
	printf("Introduceti denumirea cantinei: ");
	scanf(" %[^\n]", buffer);
	c.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(c.denumire, strlen(buffer) + 1, buffer);

	printf("Introduceti nr de angajati: ");
	scanf("%d", &c.nrAngajati);

	printf("Introduceti nr de preparate: ");
	scanf("%d", &c.nrPreparate);

	if (c.nrPreparate > 0) {
		c.preturi = (float*)malloc(sizeof(float) * c.nrPreparate);
		for (int i = 0; i < c.nrPreparate; i++) {   
			printf("Introduceti pretul preparatului(numar pozitiv) %d: ", i + 1);
			scanf("%f", &c.preturi[i]);
		}
		printf("\n");
	}
	else {
		c.preturi = NULL;
	}

	return c;
}

float mediePreturi(Cantina c) {
	if (c.nrPreparate > 0) {
		float suma = 0;
		for (int i = 0; i < c.nrPreparate; i++) {
			suma += c.preturi[i];
		}
		return suma / c.nrPreparate;
	}
	else {
		return 0;
	}
}

void modificaNrAngajati(Cantina* c, int nrAngajatiNou) {
	if (nrAngajatiNou > 0) {
		c->nrAngajati = nrAngajatiNou;
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

int main() {
	Cantina c = citire();
	afisare(c);
	printf("\n\nMedie preturi: %.2f\n", mediePreturi(c));
	int nrNouAngajati = 5;
	printf("\nModificare nr angajati: \n");
	modificaNrAngajati(&c, nrNouAngajati);
	afisare(c);
	free(c.denumire);
	free(c.preturi);
	return 0;
}
