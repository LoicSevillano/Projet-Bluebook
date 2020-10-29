#include "Fonctionglobal.h"

double calcul_impots(double b)
{
	int tx_impot = 2;

	double resultat = (b * tx_impot) / 100;
	return resultat;
}