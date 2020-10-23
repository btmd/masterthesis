/*
 * Main.cpp
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 */

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <vector>
#include <dirent.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/legacy/legacy.hpp"

#include "../include/Metrik.h"
#include "../include/Fahrzeugpool.h"

#include "gxsdldr.cpp"

using namespace cv;
using namespace std;

/**
 * Structs um die besten X Vergleichsergebnisse zu ermitteln
 */
struct VergleichsErgebnisse
{
    Fahrzeug fz;
    float Ubereinstimmungswahrscheinlichkeit;
};

struct Vergleichs_Ergebnis_zweier_Fahrzeuge
{
	Fahrzeug ursprungs_fz; // kostet die Erstellung eines Structs mit zwei Objekten viel Rechenzeit?
	Fahrzeug vergleichs_fz;

	float	Uebereinstimmung_gesamt;
	float	Uebereinstimmung_Komp_1;
	float	Uebereinstimmung_Komp_2;
	float	Uebereinstimmung_Komp_3;
	float	Uebereinstimmung_Komp_4;
	float	Uebereinstimmung_Komp_5;

};

struct Tabellenwerte
{
    float mingesamt, minK1, minK2, minK3, minK4, minK5;
    float maxgesamt, maxK1, maxK2, maxK3, maxK4, maxK5;
    float mediangesamt, medianK1, medianK2, medianK3, medianK4, medianK5;
    float sdgesamt, sdK1, sdK2, sdK3, sdK4, sdK5;
    float mittelwertgesamt, mwK1, mwK2, mwK3, mwK4, mwK5;
    float varianzgesamt, varK1, varK2, varK3, varK4, varK5;

    float sensitivitaet10, spezifitaet10;
    float sensitivitaet75, spezifitaet75;
    float sensitivitaet50, spezifitaet50;

    int truePositive10, trueNegative10, falsePositive10, falseNegative10;
    int truePositive75, trueNegative75, falsePositive75, falseNegative75;
    int truePositive50, trueNegative50, falsePositive50, falseNegative50;
};


/*
 * Methoden
 */
void ausgabeTPTN(vector<Tabellenwerte> ausgabeVektor, int quartil, int median) {
	for(unsigned int i = 0;i<ausgabeVektor.size();i++) {
		printf("Schwelle || TP | FP | TN | FN | Sensitivität | Spezifität\n");
		printf(" %d || %d | %d | %d | %d | %f | %f\n",
				10,
				ausgabeVektor.at(i).truePositive10,
				ausgabeVektor.at(i).falsePositive10,
				ausgabeVektor.at(i).trueNegative10,
				ausgabeVektor.at(i).falseNegative10,
				ausgabeVektor.at(i).sensitivitaet10,
				ausgabeVektor.at(i).spezifitaet10);
		printf(" %d || %d | %d | %d | %d | %f | %f\n",
				quartil,
				ausgabeVektor.at(i).truePositive75,
				ausgabeVektor.at(i).falsePositive75,
				ausgabeVektor.at(i).trueNegative75,
				ausgabeVektor.at(i).falseNegative75,
				ausgabeVektor.at(i).sensitivitaet75,
				ausgabeVektor.at(i).spezifitaet75);
		printf(" %d || %d | %d | %d | %d | %f | %f\n",
				median,
				ausgabeVektor.at(i).truePositive50,
				ausgabeVektor.at(i).falsePositive50,
				ausgabeVektor.at(i).trueNegative50,
				ausgabeVektor.at(i).falseNegative50,
				ausgabeVektor.at(i).sensitivitaet50,
				ausgabeVektor.at(i).spezifitaet50);
	}
}

//void ausgabeWerte(vector<Tabellenwerte> ausgabeVektor, int quartil, int median){
void ausgabeWerte(vector<Tabellenwerte> ausgabeVektor){
	for(unsigned int i = 0;i<ausgabeVektor.size();i++) {
		printf("   || Gesamt || Form || Farbe || FAST || GFTT || NP\n");
		printf("SD || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f\n",
				ausgabeVektor.at(i).sdgesamt,
				ausgabeVektor.at(i).sdK1,
				ausgabeVektor.at(i).sdK2,
				ausgabeVektor.at(i).sdK3,
				ausgabeVektor.at(i).sdK4,
				ausgabeVektor.at(i).sdK5);

		printf("MED || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f\n",
				ausgabeVektor.at(i).mediangesamt,
				ausgabeVektor.at(i).medianK1,
				ausgabeVektor.at(i).medianK2,
				ausgabeVektor.at(i).medianK3,
				ausgabeVektor.at(i).medianK4,
				ausgabeVektor.at(i).medianK5);

		printf("MIT || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f\n",
				ausgabeVektor.at(i).mittelwertgesamt,
				ausgabeVektor.at(i).mwK1,
				ausgabeVektor.at(i).mwK2,
				ausgabeVektor.at(i).mwK3,
				ausgabeVektor.at(i).mwK4,
				ausgabeVektor.at(i).mwK5);

		printf("MIN || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f\n",
				ausgabeVektor.at(i).mingesamt,
				ausgabeVektor.at(i).minK1,
				ausgabeVektor.at(i).minK2,
				ausgabeVektor.at(i).minK3,
				ausgabeVektor.at(i).minK4,
				ausgabeVektor.at(i).minK5);

		printf("MAX || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f\n",
				ausgabeVektor.at(i).maxgesamt,
				ausgabeVektor.at(i).maxK1,
				ausgabeVektor.at(i).maxK2,
				ausgabeVektor.at(i).maxK3,
				ausgabeVektor.at(i).maxK4,
				ausgabeVektor.at(i).maxK5);
	}
}

/*
 * Übergabe des Structs,
 * 			des Fahrzeugvergleich-Vektors
 *
 * Rückgabe des Structs um es in den Auswertungsvektor der Tabellenwerte zu pushen
 */
Tabellenwerte berechnungAuswertung(Tabellenwerte tabWerte, vector <Vergleichs_Ergebnis_zweier_Fahrzeuge> fahrzeugvergleichsvektor ) {
	for (unsigned int i=0; i<fahrzeugvergleichsvektor.size();i++) {
			// Min-Werte berechnen Zielfahrzeuge
			if(tabWerte.mingesamt > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt) {
				tabWerte.mingesamt = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt;
			}
			if(tabWerte.minK1 > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1) {
				tabWerte.minK1 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1;
			}
			if(tabWerte.minK2 > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2) {
				tabWerte.minK2 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2;
			}
			if(tabWerte.minK3 > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3) {
				tabWerte.minK3 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3;
			}
			if(tabWerte.minK4 > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4) {
				tabWerte.minK4 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4;
			}
			if(tabWerte.minK5 > fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5) {
				tabWerte.minK5 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5;
			}
			//cout << "Max-Werte berechnen Zielfahrzeuge" << endl;
			// Max-Werte berechnen Zielfahrzeuge
			if(tabWerte.maxgesamt < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt) {
				tabWerte.maxgesamt = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt;
			}
			if(tabWerte.maxK1 < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1) {
				tabWerte.maxK1 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1;
			}
			if(tabWerte.maxK2 < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2) {
				tabWerte.maxK2 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2;
			}
			if(tabWerte.maxK3 < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3) {
				tabWerte.maxK3 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3;
			}
			if(tabWerte.maxK4 < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4) {
				tabWerte.maxK4 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4;
			}
			if(tabWerte.maxK5 < fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5) {
				tabWerte.maxK5 = fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5;
			}
		}

		/*
		 * Medianberechnung Zielfahreuge
		 * Median: Zahl der Stichprobe /2, Wert an dieser Stelle nehmen.
		 */
		int zielmedianberechnung = fahrzeugvergleichsvektor.size();
		if (zielmedianberechnung % 2 != 0) {zielmedianberechnung=zielmedianberechnung+1;}
		zielmedianberechnung = zielmedianberechnung/2;
		/*
		 * zielmedianberechnung == 1 bedeutet: 1 Element. also muss zielmedianberechnung auf das 0-te Vektorelement zugreifen.
		 */
		if(zielmedianberechnung == 1){zielmedianberechnung=0;}
		//cout << "zielmedianberechnung " << zielmedianberechnung << endl;

		//zielfahrzeug
		//tabWerte.mediangesamt = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_gesamt;
		tabWerte.mediangesamt = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_gesamt;
		tabWerte.medianK1 = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_Komp_1;
		tabWerte.medianK2 = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_Komp_2;
		tabWerte.medianK3 = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_Komp_3;
		tabWerte.medianK4 = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_Komp_4;
		tabWerte.medianK5 = fahrzeugvergleichsvektor.at(zielmedianberechnung).Uebereinstimmung_Komp_5;

		/*
		 * Mittelwerte für einzelne Komponenten berechnen
		 */
		//cout << "Mittelwerte berechnen Zielfahrzeuge" << endl;
		for (unsigned int i=0;i<fahrzeugvergleichsvektor.size();i++) {
			tabWerte.mittelwertgesamt = tabWerte.mittelwertgesamt + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt;
			tabWerte.mwK1 = tabWerte.mwK1 + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1;
			tabWerte.mwK2 = tabWerte.mwK2 + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2;
			tabWerte.mwK3 = tabWerte.mwK3 + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3;
			tabWerte.mwK4 = tabWerte.mwK4 + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4;
			tabWerte.mwK5 = tabWerte.mwK5 + fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5;
		}

		tabWerte.mittelwertgesamt = tabWerte.mittelwertgesamt / fahrzeugvergleichsvektor.size();
		tabWerte.mwK1 = tabWerte.mwK1 / fahrzeugvergleichsvektor.size();
		tabWerte.mwK2 = tabWerte.mwK2 / fahrzeugvergleichsvektor.size();
		tabWerte.mwK3 = tabWerte.mwK3 / fahrzeugvergleichsvektor.size();
		tabWerte.mwK4 = tabWerte.mwK4 / fahrzeugvergleichsvektor.size();
		tabWerte.mwK5 = tabWerte.mwK5 / fahrzeugvergleichsvektor.size();


		/*
		 * Berechnung der Varianz
		 */
		//cout << "Varianzen berechnen" << endl;
		for (unsigned int i=0;i<fahrzeugvergleichsvektor.size();i++) {
			tabWerte.varianzgesamt = tabWerte.varianzgesamt + pow((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_gesamt - tabWerte.mittelwertgesamt) , 2);
			tabWerte.varK1 = (tabWerte.varK1 + ((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1 - tabWerte.mwK1) * (fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_1 - tabWerte.mwK1)));
			tabWerte.varK2 = (tabWerte.varK2 + ((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2 - tabWerte.mwK2) * (fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_2 - tabWerte.mwK2)));
			tabWerte.varK3 = (tabWerte.varK3 + ((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3 - tabWerte.mwK3) * (fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_3 - tabWerte.mwK3)));
			tabWerte.varK4 = (tabWerte.varK4 + ((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4 - tabWerte.mwK4) * (fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_4 - tabWerte.mwK4)));
			tabWerte.varK5 = (tabWerte.varK5 + ((fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5 - tabWerte.mwK5) * (fahrzeugvergleichsvektor.at(i).Uebereinstimmung_Komp_5 - tabWerte.mwK5)));
		}
		tabWerte.varianzgesamt = tabWerte.varianzgesamt / fahrzeugvergleichsvektor.size();
		tabWerte.varK1 = tabWerte.varK1 / fahrzeugvergleichsvektor.size();
		tabWerte.varK2 = tabWerte.varK2 / fahrzeugvergleichsvektor.size();
		tabWerte.varK3 = tabWerte.varK3 / fahrzeugvergleichsvektor.size();
		tabWerte.varK4 = tabWerte.varK4 / fahrzeugvergleichsvektor.size();
		tabWerte.varK5 = tabWerte.varK5 / fahrzeugvergleichsvektor.size();

		/*
		 * Berechnung der SD
		 */
		//cout << "SD-Werte berechnen" << endl;
		tabWerte.sdgesamt = sqrt(tabWerte.varianzgesamt);
		tabWerte.sdK1 = sqrt(tabWerte.varK1);
		tabWerte.sdK2 = sqrt(tabWerte.varK2);
		tabWerte.sdK3 = sqrt(tabWerte.varK3);
		tabWerte.sdK4 = sqrt(tabWerte.varK4);
		tabWerte.sdK5 = sqrt(tabWerte.varK5);

		//DEBUGcout << "tabwerte nach Berechnung: " << tabWerte.mingesamt << endl;

		return tabWerte;
}

Tabellenwerte berechnungDurchschnitte(Tabellenwerte durchschnitt, vector<Tabellenwerte> tabellenvektor, int anzahlFahrzeuge) {
	for(int i=0;i<anzahlFahrzeuge ;i++) {

				durchschnitt.sdgesamt = durchschnitt.sdgesamt + tabellenvektor.at(i).sdgesamt;
				durchschnitt.sdK1 = durchschnitt.sdK1 + tabellenvektor.at(i).sdK1;
				durchschnitt.sdK2 = durchschnitt.sdK2 + tabellenvektor.at(i).sdK2;
				durchschnitt.sdK3 = durchschnitt.sdK3 + tabellenvektor.at(i).sdK3;
				durchschnitt.sdK4 = durchschnitt.sdK4 + tabellenvektor.at(i).sdK4;
				durchschnitt.sdK5 = durchschnitt.sdK5 + tabellenvektor.at(i).sdK5;

				durchschnitt.mediangesamt = durchschnitt.mediangesamt + tabellenvektor.at(i).mediangesamt;
				durchschnitt.medianK1 = durchschnitt.medianK1 + tabellenvektor.at(i).medianK1;
				durchschnitt.medianK2 = durchschnitt.medianK2 + tabellenvektor.at(i).medianK2;
				durchschnitt.medianK3 = durchschnitt.medianK3 + tabellenvektor.at(i).medianK3;
				durchschnitt.medianK4 = durchschnitt.medianK4 + tabellenvektor.at(i).medianK4;
				durchschnitt.medianK5 = durchschnitt.medianK5 + tabellenvektor.at(i).medianK5;

				durchschnitt.mittelwertgesamt = durchschnitt.mittelwertgesamt + tabellenvektor.at(i).mittelwertgesamt;
				durchschnitt.mwK1 = durchschnitt.mwK1 + tabellenvektor.at(i).mwK1;
				durchschnitt.mwK2 = durchschnitt.mwK2 + tabellenvektor.at(i).mwK2;
				durchschnitt.mwK3 = durchschnitt.mwK3 + tabellenvektor.at(i).mwK3;
				durchschnitt.mwK4 = durchschnitt.mwK4 + tabellenvektor.at(i).mwK4;
				durchschnitt.mwK5 = durchschnitt.mwK5 + tabellenvektor.at(i).mwK5;

				durchschnitt.mingesamt = durchschnitt.mingesamt + tabellenvektor.at(i).mingesamt;
				durchschnitt.minK1 = durchschnitt.minK1 + tabellenvektor.at(i).minK1;
				durchschnitt.minK2 = durchschnitt.minK2 + tabellenvektor.at(i).minK2;
				durchschnitt.minK3 = durchschnitt.minK3 + tabellenvektor.at(i).minK3;
				durchschnitt.minK4 = durchschnitt.minK4 + tabellenvektor.at(i).minK4;
				durchschnitt.minK5 = durchschnitt.minK5 + tabellenvektor.at(i).minK5;

				durchschnitt.maxgesamt = durchschnitt.maxgesamt + tabellenvektor.at(i).maxgesamt;
				durchschnitt.maxK1 = durchschnitt.maxK1 + tabellenvektor.at(i).maxK1;
				durchschnitt.maxK2 = durchschnitt.maxK2 + tabellenvektor.at(i).maxK2;
				durchschnitt.maxK3 = durchschnitt.maxK3 + tabellenvektor.at(i).maxK3;
				durchschnitt.maxK4 = durchschnitt.maxK4 + tabellenvektor.at(i).maxK4;
				durchschnitt.maxK5 = durchschnitt.maxK5 + tabellenvektor.at(i).maxK5;

				durchschnitt.truePositive10 = durchschnitt.truePositive10 + tabellenvektor.at(i).truePositive10;
				durchschnitt.truePositive75 = durchschnitt.truePositive75 + tabellenvektor.at(i).truePositive75;
				durchschnitt.truePositive50 = durchschnitt.truePositive50 + tabellenvektor.at(i).truePositive50;

				durchschnitt.falsePositive10 = durchschnitt.falsePositive10 + tabellenvektor.at(i).falsePositive10;
				durchschnitt.falsePositive75 = durchschnitt.falsePositive75 + tabellenvektor.at(i).falsePositive75;
				durchschnitt.falsePositive50 = durchschnitt.falsePositive50 + tabellenvektor.at(i).falsePositive50;

				durchschnitt.trueNegative10 = durchschnitt.trueNegative10 + tabellenvektor.at(i).trueNegative10;
				durchschnitt.trueNegative75 = durchschnitt.trueNegative75 + tabellenvektor.at(i).trueNegative75;
				durchschnitt.trueNegative50 = durchschnitt.trueNegative50 + tabellenvektor.at(i).trueNegative50;

				durchschnitt.falseNegative10 = durchschnitt.falseNegative10 + tabellenvektor.at(i).falseNegative10;
				durchschnitt.falseNegative75 = durchschnitt.falseNegative75 + tabellenvektor.at(i).falseNegative75;
				durchschnitt.falseNegative50 = durchschnitt.falseNegative50 + tabellenvektor.at(i).falseNegative50;

				durchschnitt.sensitivitaet10 = durchschnitt.sensitivitaet10 + tabellenvektor.at(i).sensitivitaet10;
				durchschnitt.sensitivitaet75 = durchschnitt.sensitivitaet75 + tabellenvektor.at(i).sensitivitaet75;
				durchschnitt.sensitivitaet50 = durchschnitt.sensitivitaet50 + tabellenvektor.at(i).sensitivitaet50;

				durchschnitt.spezifitaet10 = durchschnitt.spezifitaet10 + tabellenvektor.at(i).spezifitaet10;
				durchschnitt.spezifitaet75 = durchschnitt.spezifitaet75 + tabellenvektor.at(i).spezifitaet75;
				durchschnitt.spezifitaet50 = durchschnitt.spezifitaet50 + tabellenvektor.at(i).spezifitaet50;
			}

			durchschnitt.sdgesamt = durchschnitt.sdgesamt / anzahlFahrzeuge;
			durchschnitt.sdK1 = durchschnitt.sdK1 / anzahlFahrzeuge;
			durchschnitt.sdK2 = durchschnitt.sdK2 / anzahlFahrzeuge;
			durchschnitt.sdK3 = durchschnitt.sdK3 / anzahlFahrzeuge;
			durchschnitt.sdK4 = durchschnitt.sdK4 / anzahlFahrzeuge;
			durchschnitt.sdK5 = durchschnitt.sdK5 / anzahlFahrzeuge;

			durchschnitt.mediangesamt = durchschnitt.mediangesamt / anzahlFahrzeuge;
			durchschnitt.medianK1 = durchschnitt.medianK1 / anzahlFahrzeuge;
			durchschnitt.medianK2 = durchschnitt.medianK2 / anzahlFahrzeuge;
			durchschnitt.medianK3 = durchschnitt.medianK3 / anzahlFahrzeuge;
			durchschnitt.medianK4 = durchschnitt.medianK4 / anzahlFahrzeuge;
			durchschnitt.medianK5 = durchschnitt.medianK5 / anzahlFahrzeuge;

			durchschnitt.mittelwertgesamt = durchschnitt.mittelwertgesamt / anzahlFahrzeuge;
			durchschnitt.mwK1 = durchschnitt.mwK1 / anzahlFahrzeuge;
			durchschnitt.mwK2 = durchschnitt.mwK2 / anzahlFahrzeuge;
			durchschnitt.mwK3 = durchschnitt.mwK3 / anzahlFahrzeuge;
			durchschnitt.mwK4 = durchschnitt.mwK4 / anzahlFahrzeuge;
			durchschnitt.mwK5 = durchschnitt.mwK5 / anzahlFahrzeuge;

			durchschnitt.mingesamt = durchschnitt.mingesamt / anzahlFahrzeuge;
			durchschnitt.minK1 = durchschnitt.minK1 / anzahlFahrzeuge;
			durchschnitt.minK2 = durchschnitt.minK2 / anzahlFahrzeuge;
			durchschnitt.minK3 = durchschnitt.minK3 / anzahlFahrzeuge;
			durchschnitt.minK4 = durchschnitt.minK4 / anzahlFahrzeuge;
			durchschnitt.minK5 = durchschnitt.minK5 / anzahlFahrzeuge;

			durchschnitt.maxgesamt = durchschnitt.maxgesamt / anzahlFahrzeuge;
			durchschnitt.maxK1 = durchschnitt.maxK1 / anzahlFahrzeuge;
			durchschnitt.maxK2 = durchschnitt.maxK2 / anzahlFahrzeuge;
			durchschnitt.maxK3 = durchschnitt.maxK3 / anzahlFahrzeuge;
			durchschnitt.maxK4 = durchschnitt.maxK4 / anzahlFahrzeuge;
			durchschnitt.maxK5 = durchschnitt.maxK5 / anzahlFahrzeuge;

			durchschnitt.truePositive10 = durchschnitt.truePositive10 / anzahlFahrzeuge;
			durchschnitt.truePositive75 = durchschnitt.truePositive75 / anzahlFahrzeuge;
			durchschnitt.truePositive50 = durchschnitt.truePositive50 / anzahlFahrzeuge;

			durchschnitt.trueNegative10 = durchschnitt.trueNegative10 / anzahlFahrzeuge;
			durchschnitt.trueNegative75 = durchschnitt.trueNegative75 / anzahlFahrzeuge;
			durchschnitt.trueNegative50 = durchschnitt.trueNegative50 / anzahlFahrzeuge;

			durchschnitt.falsePositive10 = durchschnitt.falsePositive10 / anzahlFahrzeuge;
			durchschnitt.falsePositive75 = durchschnitt.falsePositive75 / anzahlFahrzeuge;
			durchschnitt.falsePositive50 = durchschnitt.falsePositive50 / anzahlFahrzeuge;

			durchschnitt.falseNegative10 = durchschnitt.falseNegative10 / anzahlFahrzeuge;
			durchschnitt.falseNegative75 = durchschnitt.falseNegative75 / anzahlFahrzeuge;
			durchschnitt.falseNegative50 = durchschnitt.falseNegative50 / anzahlFahrzeuge;

			durchschnitt.sensitivitaet10 = durchschnitt.sensitivitaet10 / anzahlFahrzeuge;
			durchschnitt.sensitivitaet75 = durchschnitt.sensitivitaet75 / anzahlFahrzeuge;
			durchschnitt.sensitivitaet50 = durchschnitt.sensitivitaet50 / anzahlFahrzeuge;

			durchschnitt.spezifitaet10 = durchschnitt.spezifitaet10 / anzahlFahrzeuge;
			durchschnitt.spezifitaet75 = durchschnitt.spezifitaet75 / anzahlFahrzeuge;
			durchschnitt.spezifitaet50 = durchschnitt.spezifitaet50 / anzahlFahrzeuge;

			return durchschnitt;
}

void ausgabeDurchschnitt(Tabellenwerte durchschnitt) {
	printf("Durchschnitt || Gesamt || Form || Farbe || FAST || GFTT || NP\n");
	printf("  SD || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f \n",
			durchschnitt.sdgesamt, durchschnitt.sdK1, durchschnitt.sdK2, durchschnitt.sdK3, durchschnitt.sdK4, durchschnitt.sdK5);
	printf(" MED || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f \n",
			durchschnitt.mediangesamt, durchschnitt.medianK1, durchschnitt.medianK2, durchschnitt.medianK3, durchschnitt.medianK4, durchschnitt.medianK5);
	printf(" MIT || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f \n",
			durchschnitt.mittelwertgesamt, durchschnitt.mwK1, durchschnitt.mwK2, durchschnitt.mwK3, durchschnitt.mwK4, durchschnitt.mwK5);
	printf(" MIN || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f \n",
			durchschnitt.mingesamt, durchschnitt.minK1, durchschnitt.minK2, durchschnitt.minK3, durchschnitt.minK4, durchschnitt.minK5);
	printf(" MAX || %.2f || %.2f || %.2f || %.2f || %.2f || %.2f \n",
			durchschnitt.maxgesamt, durchschnitt.maxK1, durchschnitt.maxK2, durchschnitt.maxK3, durchschnitt.maxK4, durchschnitt.maxK5);
}


//BERECHNUNG DER TRUE/FALSE/POSITIVES/NEGATIVES
//FÜR ALLE FAHRZEUGE
Tabellenwerte tPtNBerechnung(Tabellenwerte tabWerte,
		vector<Vergleichs_Ergebnis_zweier_Fahrzeuge> vector_allefahrzeuge,
		vector<Vergleichs_Ergebnis_zweier_Fahrzeuge> vector_zielfahrzeuge,
		int truepos1, int truepos2, int truepos3, int quartil, int median) {
//cout << "TP/FP/TN/FN berechnen Alle Fahrzeuge" << endl;
//TRUE POSITIVES
	tabWerte.truePositive10 = truepos1; // FEHLER BEI WENIGER ALS 10 FAHRZEUGEN!
	tabWerte.truePositive75 = truepos2;
	tabWerte.truePositive50 = truepos3;
//FALSE POSITIVES:
	tabWerte.falsePositive10 = 10 - tabWerte.truePositive10;
	tabWerte.falsePositive75 = quartil - tabWerte.truePositive75;
	tabWerte.falsePositive50 = median - tabWerte.truePositive50;
//FALSE NEGATIVES
	tabWerte.falseNegative10 = vector_zielfahrzeuge.size() - tabWerte.truePositive10;
	tabWerte.falseNegative75 = vector_zielfahrzeuge.size() - tabWerte.truePositive75;
	tabWerte.falseNegative50 = vector_zielfahrzeuge.size() - tabWerte.truePositive50;
// TRUE NEGATIVES
	tabWerte.trueNegative10 = vector_allefahrzeuge.size() - 10 - tabWerte.falseNegative10;
	tabWerte.trueNegative75 = vector_allefahrzeuge.size() - quartil - tabWerte.falseNegative75;
	tabWerte.trueNegative50 = vector_allefahrzeuge.size() - median - tabWerte.falseNegative50;

//SENSITIVITÄT
	tabWerte.sensitivitaet10 = (float)(tabWerte.truePositive10 / ((float)(tabWerte.truePositive10) + (tabWerte.falseNegative10)));
	tabWerte.sensitivitaet75 = (float)(tabWerte.truePositive75 / ((float)(tabWerte.truePositive75) + (tabWerte.falseNegative75)));
	tabWerte.sensitivitaet50 = (float)(tabWerte.truePositive50 / ((float)(tabWerte.truePositive50) + (tabWerte.falseNegative50)));
//SPEZIFITÄT
	tabWerte.spezifitaet10 = (float)(tabWerte.truePositive10 / ((float)(tabWerte.truePositive10) + (tabWerte.falsePositive10)));
	tabWerte.spezifitaet75 = (float)(tabWerte.truePositive75 / ((float)(tabWerte.truePositive75) + (tabWerte.falsePositive75)));
	tabWerte.spezifitaet50 = (float)(tabWerte.truePositive50 / ((float)(tabWerte.truePositive50) + (tabWerte.falsePositive50)));

	return tabWerte;
}
/*
 Schwelle || TP | FP | TN | FN | Sensitivität | Spezifität
 10 || 2 | 8 | -6 | 0 | 1.000000 | 0.200000
 1 || 2 | -1 | 3 | 0 | 1.000000 | 2.000000
 2 || 2 | 0 | 2 | 0 | 1.000000 | 1.000000
 *
 */
void ausgabeDurchschnittTPTN(Tabellenwerte durchschnitt, int quartil, int median) {
	printf("  TH || TP || FP || TN || FN || Sensitivität || Spezifität \n");
	printf("  10 || %d || %d || %d || %d || %.2f || %.2f \n",
				durchschnitt.truePositive10, durchschnitt.falsePositive10, durchschnitt.trueNegative10, durchschnitt.falseNegative10, durchschnitt.sensitivitaet10, durchschnitt.spezifitaet10);
	printf(" %d || %d || %d || %d || %d || %.2f || %.2f \n",
			quartil, durchschnitt.truePositive75, durchschnitt.falsePositive75, durchschnitt.trueNegative75, durchschnitt.falseNegative75, durchschnitt.sensitivitaet75, durchschnitt.spezifitaet75);
	printf(" %d || %d || %d || %d || %d || %.2f || %.2f \n",
			median, durchschnitt.truePositive50, durchschnitt.falsePositive50, durchschnitt.trueNegative50, durchschnitt.falseNegative50, durchschnitt.sensitivitaet50, durchschnitt.spezifitaet50);
}

/*
 * Methoden Ende
 */

//Notwendig, damit std::sort nach dem Vergleichsergebnis des Struct sortiert
bool best_propability (const Vergleichs_Ergebnis_zweier_Fahrzeuge& struct1, const Vergleichs_Ergebnis_zweier_Fahrzeuge& struct2)
{
	return (struct1.Uebereinstimmung_gesamt > struct2.Uebereinstimmung_gesamt);
}

bool wayToSort(int i, int j) { return i > j; }

bool einlesen_bilder = true;
bool einlesen = true;

//Hilfsvariablen für die Auswertungsberechnungen
string vglfz = "";
int name = 0;

vector <Tabellenwerte> vector_struct_zielfahrzeuge_tabellenwerte;
vector <Tabellenwerte> vector_struct_allefahrzeuge_tabellenwerte;

unsigned int quartil75 = 0;
unsigned int quartil25 = 0;
unsigned int medianberechnung = 0;

int TP1 = 0;
int TP2 = 0;
int TP3 = 0;


int main( int argc, char** argv )
{
	Fahrzeugpool gesamter_Fahrzeugpool;
	Fahrzeugpool vergleichs_Fahrzeugpool;

	Tabellenwerte durchschnittziel;
	durchschnittziel.mingesamt = 0.0; durchschnittziel.minK1 = 0.0; durchschnittziel.minK2 = 0.0; durchschnittziel.minK3 = 0.0; durchschnittziel.minK4 = 0.0; durchschnittziel.minK5 = 0.0;
	durchschnittziel.maxgesamt = 0.0; durchschnittziel.maxK1 = 0.0; durchschnittziel.maxK2 = 0.0; durchschnittziel.maxK3 = 0.0; durchschnittziel.maxK4 = 0.0; durchschnittziel.maxK5 = 0.0;
	durchschnittziel.mediangesamt = 0.0; durchschnittziel.medianK1 = 0.0; durchschnittziel.medianK2 = 0.0; durchschnittziel.medianK3 = 0.0; durchschnittziel.medianK4 = 0.0; durchschnittziel.medianK5 = 0.0;
	durchschnittziel.sdgesamt = 0.0; durchschnittziel.sdK1 = 0.0; durchschnittziel.sdK2 = 0.0; durchschnittziel.sdK3 = 0.0; durchschnittziel.sdK4 = 0.0; durchschnittziel.sdK5 = 0.0;
	durchschnittziel.mittelwertgesamt = 0.0; durchschnittziel.mwK1 = 0.0; durchschnittziel.mwK2 = 0.0; durchschnittziel.mwK3 = 0.0; durchschnittziel.mwK4 = 0.0; durchschnittziel.mwK5 = 0.0;
	durchschnittziel.varianzgesamt = 0.0; durchschnittziel.varK1 = 0.0; durchschnittziel.varK2 = 0.0; durchschnittziel.varK3 = 0.0; durchschnittziel.varK4 = 0.0; durchschnittziel.varK5 = 0.0;

	durchschnittziel.sensitivitaet10 = 0.0; durchschnittziel.sensitivitaet75 = 0.0; durchschnittziel.sensitivitaet50 = 0.0;
	durchschnittziel.spezifitaet10 = 0.0; durchschnittziel.spezifitaet75 = 0.0; durchschnittziel.spezifitaet50 = 0.0;

	durchschnittziel.truePositive10 = 0; durchschnittziel.truePositive75 = 0; durchschnittziel.truePositive50 = 0;
	durchschnittziel.trueNegative10 = 0; durchschnittziel.trueNegative75 = 0; durchschnittziel.trueNegative50 = 0;
	durchschnittziel.falsePositive10 = 0; durchschnittziel.falsePositive75 = 0; durchschnittziel.falsePositive50 = 0;
	durchschnittziel.falseNegative10 = 0; durchschnittziel.falseNegative75 = 0; durchschnittziel.falseNegative50 = 0;

	Tabellenwerte durchschnittalle;
	durchschnittalle.mingesamt = 0.0; durchschnittalle.minK1 = 0.0; durchschnittalle.minK2 = 0.0; durchschnittalle.minK3 = 0.0; durchschnittalle.minK4 = 0.0; durchschnittalle.minK5 = 0.0;
	durchschnittalle.maxgesamt = 0.0; durchschnittalle.maxK1 = 0.0; durchschnittalle.maxK2 = 0.0; durchschnittalle.maxK3 = 0.0; durchschnittalle.maxK4 = 0.0; durchschnittalle.maxK5 = 0.0;
	durchschnittalle.mediangesamt = 0.0; durchschnittalle.medianK1 = 0.0; durchschnittalle.medianK2 = 0.0; durchschnittalle.medianK3 = 0.0; durchschnittalle.medianK4 = 0.0; durchschnittalle.medianK5 = 0.0;
	durchschnittalle.sdgesamt = 0.0; durchschnittalle.sdK1 = 0.0; durchschnittalle.sdK2 = 0.0; durchschnittalle.sdK3 = 0.0; durchschnittalle.sdK4 = 0.0; durchschnittalle.sdK5 = 0.0;
	durchschnittalle.mittelwertgesamt = 0.0; durchschnittalle.mwK1 = 0.0; durchschnittalle.mwK2 = 0.0; durchschnittalle.mwK3 = 0.0; durchschnittalle.mwK4 = 0.0; durchschnittalle.mwK5 = 0.0;
	durchschnittalle.varianzgesamt = 0.0; durchschnittalle.varK1 = 0.0; durchschnittalle.varK2 = 0.0; durchschnittalle.varK3 = 0.0; durchschnittalle.varK4 = 0.0; durchschnittalle.varK5 = 0.0;

	durchschnittalle.sensitivitaet10 = 0.0; durchschnittalle.sensitivitaet75 = 0.0; durchschnittalle.sensitivitaet50 = 0.0;
	durchschnittalle.spezifitaet10 = 0.0; durchschnittalle.spezifitaet75 = 0.0; durchschnittalle.spezifitaet50 = 0.0;

	durchschnittalle.truePositive10 = 0; durchschnittalle.truePositive75 = 0; durchschnittalle.truePositive50 = 0;
	durchschnittalle.trueNegative10 = 0; durchschnittalle.trueNegative75 = 0; durchschnittalle.trueNegative50 = 0;
	durchschnittalle.falsePositive10 = 0; durchschnittalle.falsePositive75 = 0; durchschnittalle.falsePositive50 = 0;
	durchschnittalle.falseNegative10 = 0; durchschnittalle.falseNegative75 = 0; durchschnittalle.falseNegative50 = 0;

	/**
	 * Im ersten Funktionstest arbeite ich nur mit wenigen Bildern.
	 * Je nach Bildanzahl im Ordner kann das aber auch beliebig gesteigert werden.
	 * Das erfordert jedoch große Mengen Rechenzeit.
	 * TODO: Fahrzeugvektoren als Datei speichern und dann nur noch einlesen.
	 * In Vorbereitung auf die Verteidigung!
	 */

/*
 * Hier lese ich Ordnerinhalte ein für die beiden Fahrzeugpoolvektoren
 */

	vector<string> dateipfade_gesamtPool;
	vector<string> dateipfade_vglPool;

	string gesamtPool = argv[1];
	string vglPool = argv[2];
	const char * c_gesamtPool = gesamtPool.c_str();
	const char * c_vglPool = vglPool.c_str();

	string _temp = "";
	string ordnernameargv1 = argv[1];
	string ordnernameargv2 = argv[2];

	DIR *dir;
	struct dirent *ent;

	if (einlesen){
	/*
	 * einlesen der Dateipfade des argv[1] in einen Vektor
	 */
	if ((dir = opendir(c_gesamtPool)) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			_temp = ent->d_name;
			if(_temp[0]!='.')
			{
				_temp = ordnernameargv1 + _temp;
				dateipfade_gesamtPool.push_back(_temp);
			}
		}
		closedir (dir);
	} else {
		/* Verzeichnis konnte nicht geöffnet werden */
		perror ("");
		return EXIT_FAILURE;
	}

	/*
	 * einlesen der Dateipfade des argv[2] in einen Vektor
	 */
	if ((dir = opendir(c_vglPool)) != NULL)
		{
			while ((ent = readdir(dir)) != NULL)
			{
				_temp = ent->d_name;
				if(_temp[0]!='.')
				{
					_temp = ordnernameargv2 + _temp;
					dateipfade_vglPool.push_back(_temp);
				}
			}
			closedir (dir);
		} else {
			/* Verzeichnis konnte nicht geöffnet werden */
			perror ("");
			return EXIT_FAILURE;
		}
	einlesen = false;
	}

	int anzahl_gesamtBilder = dateipfade_gesamtPool.size();
	int anzahl_vglBilder = dateipfade_vglPool.size();
	cout << "Fahrzeuge im Gesamtpool: " << anzahl_gesamtBilder << endl;


	if (einlesen_bilder){
		for(int i=0;i<anzahl_gesamtBilder;i++)
		{
			gesamter_Fahrzeugpool.addToFahrzeugpoolVektor(dateipfade_gesamtPool[i]);
			//cout << "Fahrzeug " << i << " hinzugefügt!" << " Dateipfad: " << dateipfade_gesamtPool[i]<< " np:"<< gesamter_Fahrzeugpool.getFzByIdFromFahrzeugpoolVektor(i).getNummernschild() << endl;
		}

		/*
		 * Fahrzeuge zum Pool hinzufügen; Fahrzeugobjekte erstellen
		 */
		for(int i=0;i<anzahl_vglBilder;i++)
		{
			//cout << "Vergleichsfahrzeug berechnen" << endl;
			vergleichs_Fahrzeugpool.addToFahrzeugpoolVektor(dateipfade_vglPool[i]);
		}
	einlesen_bilder = false;
	}

	vector<Fahrzeug> beste_Vergleiche_der_Fahrzeuge;
	Fahrzeug aktuellBestesFahrzeug;

	float _tempVergleich = 0.0;
	float _bestVergleich = 0.0;

	Mat src, vgl;

	/**
	 * Vector zwischenergebnisse vom Typ Struct VergleichsErgebnisse.
	 * Hier werden alle mit dem Originalobjekt verglichenen Fahrzeuge zwischengespeichert.
	 * Es enthält ihr Übereinstimmungsergebnis, sowie ihr Fahrzeugobjekt.
	 * Dadurch kann sofort ermittelt werden, welches Fz in einem Vergleich welchen Wert erreichte.
	 */
	vector <VergleichsErgebnisse> zwischenergebnisse;
	vector <Vergleichs_Ergebnis_zweier_Fahrzeuge> vector_struct_zwischenergebnisse;

	/**
	 * Struct um die Ergebnisse der Fahrzeuge zwischenzuspeichern.
	 * Werden nachfolgend ermittelt, in den Vector push_back() und überschrieben.
	 * Es ist also lediglich ein Objekt notwendig.
	 */
	VergleichsErgebnisse test1;
	Vergleichs_Ergebnis_zweier_Fahrzeuge test2;

	int auswahl = atoi(argv[3]);

	if (auswahl == 1) // nur Formvergleich
	{
		for(unsigned int i=0;i<vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size();i++)
		{
			for(unsigned int j=0;j<gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size();j++)
			{
				Metrik metrik(vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i).getGroessenverhaeltnis(), gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j));
				_tempVergleich = metrik.getGesamtWahrscheinlichkeit();

				test1.fz = gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j);
				test1.Ubereinstimmungswahrscheinlichkeit = _tempVergleich;
				zwischenergebnisse.push_back(test1);

				if(_bestVergleich <= _tempVergleich){
					_bestVergleich = _tempVergleich;
					aktuellBestesFahrzeug = gesamter_Fahrzeugpool.getFzByIdFromFahrzeugpoolVektor(j);
				}
			}
			beste_Vergleiche_der_Fahrzeuge.push_back(aktuellBestesFahrzeug);
			_bestVergleich = 0;
		}
	}
	else if (auswahl == 2) // nur Farbvergleich
	{
		for(unsigned int i=0;i<vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size();i++)
		{
			for(unsigned int j=0;j<gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size();j++)
			{
				Metrik metrik(gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getHistogramm(), vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i));
				_tempVergleich = metrik.getGesamtWahrscheinlichkeit();

				test1.fz = gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j);
				test1.Ubereinstimmungswahrscheinlichkeit = _tempVergleich;
				zwischenergebnisse.push_back(test1);

				if(_bestVergleich <= _tempVergleich){
					_bestVergleich = _tempVergleich;
					aktuellBestesFahrzeug = gesamter_Fahrzeugpool.getFzByIdFromFahrzeugpoolVektor(j);
				}
			}
			beste_Vergleiche_der_Fahrzeuge.push_back(aktuellBestesFahrzeug);
			_bestVergleich = 0;
		}
	}
	else if (auswahl == 3) // nur Featurevergleich
	{
		for(unsigned int i=0;i<vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size();i++)
		{
			for(unsigned int j=0;j<gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size();j++)
			{
				Metrik metrik(gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getFASTKeypoints(),
								gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getGFTTKeypoints(),
								gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getBRIEFDescriptoren(),
								gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getSIFTDescriptoren(),
								vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i));

				_tempVergleich = metrik.getGesamtWahrscheinlichkeit();

				test1.fz = gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j);
				test1.Ubereinstimmungswahrscheinlichkeit = _tempVergleich;
				zwischenergebnisse.push_back(test1);


				if(_bestVergleich <= _tempVergleich){
					_bestVergleich = _tempVergleich;
					aktuellBestesFahrzeug = gesamter_Fahrzeugpool.getFzByIdFromFahrzeugpoolVektor(j);
				}
			}
			beste_Vergleiche_der_Fahrzeuge.push_back(aktuellBestesFahrzeug);
			_bestVergleich = 0;
		}
	}
	else if (auswahl == 4) // nur Nummernschildvergleich
	{
		for(unsigned int i=0;i<vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size();i++)
		{
			for(unsigned int j=0;j<gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size();j++)
			{
				Metrik metrik(gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j).getNummernschild(),
								vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i));

				_tempVergleich = metrik.getGesamtWahrscheinlichkeit();

				test1.fz = gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j);
				test1.Ubereinstimmungswahrscheinlichkeit = _tempVergleich;
				zwischenergebnisse.push_back(test1);


				if(_bestVergleich <= _tempVergleich){
					_bestVergleich = _tempVergleich;
					aktuellBestesFahrzeug = gesamter_Fahrzeugpool.getFzByIdFromFahrzeugpoolVektor(j);
				}
			}
			beste_Vergleiche_der_Fahrzeuge.push_back(aktuellBestesFahrzeug);
			_bestVergleich = 0;
		}
	}

	else if (auswahl > 4) // Vergleich über alle Komponenten
	{
		cout << " Zeiten: Form, Farbe, FAST, GFTT, ANPR, GESAMT" << endl;
		for(unsigned int i=0;i<vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size();i++)
		{
			vector_struct_zwischenergebnisse.clear();
			for(unsigned int j=0;j<gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size();j++)
			{
				// cout << "gesamterFZpool" << gesamter_Fahrzeugpool.getFahrzeugpoolVektor().size() << endl;
				//cout << "vergleichsFZpool" << vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().size() << endl;
				//cout << " Zeiten: Form, Farbe, FAST, GFTT, ANPR, GESAMT" << endl;
				Metrik metrik(gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j), vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i));

				test2.ursprungs_fz = gesamter_Fahrzeugpool.getFahrzeugpoolVektor().at(j);
				test2.vergleichs_fz = vergleichs_Fahrzeugpool.getFahrzeugpoolVektor().at(i);

				test2.Uebereinstimmung_gesamt = metrik.getGesamtWahrscheinlichkeit();
				test2.Uebereinstimmung_Komp_1 = metrik.getFarbVergleichProzent();
				test2.Uebereinstimmung_Komp_2 = metrik.getFormVergleichProzent();
				if(test2.Uebereinstimmung_Komp_2 != test2.Uebereinstimmung_Komp_2) {
					test2.Uebereinstimmung_Komp_2 = 0.0;
				}
				test2.Uebereinstimmung_Komp_3 = metrik.getFastFeatureVergleichProzent();
				test2.Uebereinstimmung_Komp_4 = metrik.getGfttFeatureVergleichProzent();
				test2.Uebereinstimmung_Komp_5 = metrik.getNummernschildVergleichProzent();

				vector_struct_zwischenergebnisse.push_back(test2);
				//cout << "DEBUG003" << endl;
			}

				sort(vector_struct_zwischenergebnisse.begin(), vector_struct_zwischenergebnisse.end(), best_propability);

			/*
			 * Jetzt brauche ich einen zweiten Vector, der nur __ZIELFAHRZEUGE__ enthält.
			 */
//******BEREWCHNUNG DER TP***************/
			string ursprungsfz = "";
			string vglfz = "";
			int name = 0;

			string bmw = "dunkelgrau-bmw";
			string mer = "silber-mercedes";
			string sma = "blau-smart";
			string sko = "rot-Skoda";
			string maz = "silber-mazda";

			vector <Vergleichs_Ergebnis_zweier_Fahrzeuge> vector_struct_zielfahrzeuge;

			//Bestimmung der 3 Thresholds: oberen 10 Fahrzeuge, Oberes Quartil, Median

			//unsigned int quartil75 = vector_struct_zwischenergebnisse.size();
			quartil75 = vector_struct_zwischenergebnisse.size();
			quartil75 = quartil75*0.25;

			quartil25 = vector_struct_zwischenergebnisse.size();
			quartil25 = quartil25*0.75;

			//unsigned int medianberechnung = vector_struct_zwischenergebnisse.size();
			medianberechnung = vector_struct_zwischenergebnisse.size();
			if (medianberechnung % 2 != 0) {medianberechnung=medianberechnung+1;}
			medianberechnung = medianberechnung/2;
			//cout << "DEBUG4" << endl;
			ursprungsfz = vector_struct_zwischenergebnisse.at(0).vergleichs_fz.getPfadUrsprungsBild();
			if (ursprungsfz.find(bmw) != string::npos) {
				name = 0;
			} else if (ursprungsfz.find(mer) != string::npos) {
				name = 1;
			} else if (ursprungsfz.find(sma) != string::npos) {
				name = 2;
			} else if (ursprungsfz.find(sko) != string::npos) {
				name = 3;
			} else if (ursprungsfz.find(maz) != string::npos) {
				name = 4;
			}

//////////	 Vektor mit Zielfahrzeugen wird befüllt

			// Werte für die True Positives jeweils für die drei Thresholds (10, 3.Quartil, Median)
			TP1 = 0;
			TP2 = 0;
			TP3 = 0;

			switch( name ) {
				case 0: { // dunkelgrau-bmw
					for(unsigned int i = 0; i<vector_struct_zwischenergebnisse.size();i++) {
						if(vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().find(bmw) != string::npos) {
							// Wenn Zielfahrzeug im Ergebnisvektor gefunden wurde, kommt es zum einen in den Zielvektor
							vector_struct_zielfahrzeuge.push_back(vector_struct_zwischenergebnisse.at(i));
							// zum anderen wird es als TP
							// dies geschieht für die 3 Thresholds
							// die FALSE POSITIVES/NEGATIVES erkenne ich nach diesen Schleifen, wenn ich die größe des Zielvektors kenne
							if (i<=10) {
								//TP für alle hochsetzen
								TP1++; TP2++; TP3++;
							} else if (i > 10 && i <= quartil75) {
								//TP für quartil und median hochsetzen
								TP2++; TP3++;
							} else if (i > quartil75 && i <= medianberechnung) {
								//TP für median hochsetzen
								TP3++;
							}
						}
					}
					break;
				}
				case 1: { // silber-mercedes
					for(unsigned int i = 0; i<vector_struct_zwischenergebnisse.size();i++) {
						if(vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().find(mer) != string::npos) {
							vector_struct_zielfahrzeuge.push_back(vector_struct_zwischenergebnisse.at(i));
							if (i<=10) {
								//TP für alle hochsetzen
								TP1++; TP2++; TP3++;
							} else if (i > 10 && i <= quartil75) {
								//TP für quartil und median hochsetzen
								TP2++; TP3++;
							} else if (i > quartil75 && i <= medianberechnung) {
								//TP für median hochsetzen
								TP3++;
							}
						}
					}
					break;
				}
				case 2: { // blau-smart
					for(unsigned int i = 0; i<vector_struct_zwischenergebnisse.size();i++) {
						if(vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().find(sma) != string::npos) {
							vector_struct_zielfahrzeuge.push_back(vector_struct_zwischenergebnisse.at(i));
							if (i<=10) {
								//TP für alle hochsetzen
								TP1++; TP2++; TP3++;
							} else if ((i > 10) && (i <= quartil75)) {
								//TP für quartil und median hochsetzen
								TP2++; TP3++;
							} else if ((i > quartil75) && (i <= medianberechnung)) {
								//TP für median hochsetzen
								TP3++;
							}
						}

					}
					break;
				}
				case 3: { // rot-Skoda
					for(unsigned int i = 0; i<vector_struct_zwischenergebnisse.size();i++) {
						if(vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().find(sko) != string::npos) {
							vector_struct_zielfahrzeuge.push_back(vector_struct_zwischenergebnisse.at(i));
							if (i<=10) {
								//TP für alle hochsetzen
								TP1++; TP2++; TP3++;
							} else if (i > 10 && i <= quartil75) {
								//TP für quartil und median hochsetzen
								TP2++; TP3++;
							} else if (i > quartil75 && i <= medianberechnung) {
								//TP für median hochsetzen
								TP3++;
							}
						}
					}
					break;
				}
				case 4: { // silber-mazda
					for(unsigned int i = 0; i<vector_struct_zwischenergebnisse.size();i++) {
						if(vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().find(maz) != string::npos) {
							vector_struct_zielfahrzeuge.push_back(vector_struct_zwischenergebnisse.at(i));
							if (i<=10) {
								//TP für alle hochsetzen
								TP1++; TP2++; TP3++;
							} else if (i > 10 && i <= quartil75) {
								//TP für quartil und median hochsetzen
								TP2++; TP3++;
							} else if (i > quartil75 && i <= medianberechnung) {
								//TP für median hochsetzen
								TP3++;
							}
						}
					}
					break;
				}
			} // end of Switch-Case zur Fahrzeugbestimmung
	/*
	 * jetzt gibt es in Abhängigkeit vom Vergleichsfahrzeug einen Vektor, der die richtigen Ergebnisse beinhaltet.
	 * Nun kann ich ein Struct anlegen, dass die
	 * MIN, MAX, MEDIAN und SD pro Komponenten erfasst
	 * sowie die TP/FP/TN/FN
	 */

	 //Die Tabellenwerte werden hier erfasst und initialisiert, damit sie zu Beginn immer 0 sind.

			Tabellenwerte zielfahrzeug;
			zielfahrzeug.mingesamt = 100.0; zielfahrzeug.minK1 = 100.0; zielfahrzeug.minK2 = 100.0; zielfahrzeug.minK3 = 100.0; zielfahrzeug.minK4 = 100.0; zielfahrzeug.minK5 = 100.0;
			zielfahrzeug.maxgesamt = 0.0; zielfahrzeug.maxK1 = 0.0; zielfahrzeug.maxK2 = 0.0; zielfahrzeug.maxK3 = 0.0; zielfahrzeug.maxK4 = 0.0; zielfahrzeug.maxK5 = 0.0;
			zielfahrzeug.mediangesamt = 0.0; zielfahrzeug.medianK1 = 0.0; zielfahrzeug.medianK2 = 0.0; zielfahrzeug.medianK3 = 0.0; zielfahrzeug.medianK4 = 0.0; zielfahrzeug.medianK5 = 0.0;
			zielfahrzeug.sdgesamt = 0.0; zielfahrzeug.sdK1 = 0.0; zielfahrzeug.sdK2 = 0.0; zielfahrzeug.sdK3 = 0.0; zielfahrzeug.sdK4 = 0.0; zielfahrzeug.sdK5 = 0.0;
			zielfahrzeug.mittelwertgesamt = 0.0; zielfahrzeug.mwK1 = 0.0; zielfahrzeug.mwK2 = 0.0; zielfahrzeug.mwK3 = 0.0; zielfahrzeug.mwK4 = 0.0; zielfahrzeug.mwK5 = 0.0;
			zielfahrzeug.varianzgesamt = 0.0; zielfahrzeug.varK1 = 0.0; zielfahrzeug.varK2 = 0.0; zielfahrzeug.varK3 = 0.0; zielfahrzeug.varK4 = 0.0; zielfahrzeug.varK5 = 0.0;

			zielfahrzeug.sensitivitaet10 = 0.0; zielfahrzeug.sensitivitaet75 = 0.0; zielfahrzeug.sensitivitaet50 = 0.0;
			zielfahrzeug.spezifitaet10 = 0.0; zielfahrzeug.spezifitaet75 = 0.0; zielfahrzeug.spezifitaet50 = 0.0;

			zielfahrzeug.truePositive10 = 0; zielfahrzeug.truePositive75 = 0; zielfahrzeug.truePositive50 = 0;
			zielfahrzeug.trueNegative10 = 0; zielfahrzeug.trueNegative75 = 0; zielfahrzeug.trueNegative50 = 0;
			zielfahrzeug.falsePositive10 = 0; zielfahrzeug.falsePositive75 = 0; zielfahrzeug.falsePositive50 = 0;
			zielfahrzeug.falseNegative10 = 0; zielfahrzeug.falseNegative75 = 0; zielfahrzeug.falseNegative50 = 0;

			Tabellenwerte allefahrzeuge;
			allefahrzeuge.mingesamt = 100.0; allefahrzeuge.minK1 = 100.0; allefahrzeuge.minK2 = 100.0; allefahrzeuge.minK3 = 100.0; allefahrzeuge.minK4 = 100.0; allefahrzeuge.minK5 = 100.0;
			allefahrzeuge.maxgesamt = 0.0; allefahrzeuge.maxK1 = 0.0; allefahrzeuge.maxK2 = 0.0; allefahrzeuge.maxK3 = 0.0; allefahrzeuge.maxK4 = 0.0; allefahrzeuge.maxK5 = 0.0;
			allefahrzeuge.mediangesamt = 0.0; allefahrzeuge.medianK1 = 0.0; allefahrzeuge.medianK2 = 0.0; allefahrzeuge.medianK3 = 0.0; allefahrzeuge.medianK4 = 0.0; allefahrzeuge.medianK5 = 0.0;
			allefahrzeuge.sdgesamt = 0.0; allefahrzeuge.sdK1 = 0.0; allefahrzeuge.sdK2 = 0.0; allefahrzeuge.sdK3 = 0.0; allefahrzeuge.sdK4 = 0.0; allefahrzeuge.sdK5 = 0.0;
			allefahrzeuge.mittelwertgesamt = 0.0; allefahrzeuge.mwK1 = 0.0; allefahrzeuge.mwK2 = 0.0; allefahrzeuge.mwK3 = 0.0; allefahrzeuge.mwK4 = 0.0; allefahrzeuge.mwK5 = 0.0;
			allefahrzeuge.varianzgesamt = 0.0; allefahrzeuge.varK1 = 0.0; allefahrzeuge.varK2 = 0.0; allefahrzeuge.varK3 = 0.0; allefahrzeuge.varK4 = 0.0; allefahrzeuge.varK5 = 0.0;

			allefahrzeuge.sensitivitaet10 = 0.0; allefahrzeuge.sensitivitaet75 = 0.0; allefahrzeuge.sensitivitaet50 = 0.0;
			allefahrzeuge.spezifitaet10 = 0.0; allefahrzeuge.spezifitaet75 = 0.0; allefahrzeuge.spezifitaet50 = 0.0;

			allefahrzeuge.truePositive10 = 0; allefahrzeuge.truePositive75 = 0; allefahrzeuge.truePositive50 = 0;
			allefahrzeuge.trueNegative10 = 0; allefahrzeuge.trueNegative75 = 0; allefahrzeuge.trueNegative50 = 0;
			allefahrzeuge.falsePositive10 = 0; allefahrzeuge.falsePositive75 = 0; allefahrzeuge.falsePositive50 = 0;
			allefahrzeuge.falseNegative10 = 0; allefahrzeuge.falseNegative75 = 0; allefahrzeuge.falseNegative50 = 0;

			/*
			 * 0-Initialisierung fertig, das ist wichtig für eine unverfälschte Berechnung
			 */


			/*
			 * BERECHNUNG DER TABELLEN ERFOLGT HIER!
			 * ICH HABE DEN SORTIERTEN VEKTOR DER ZWISCHENERGEBNISSE UND DER ZIELFAHRZEUGE
			 *
			 * ZWISCHENERGEBNISSE: 	VECTOR_STRUCT_ZWISCHENERGEBNISSE
			 * ZIELFAHRZEUGE: 		VECTOR_STRUCT_ZIELFAHRZEUGE
			 *
			 * PRO ELEMENT DES JEWEILIGEN VEKTORS MUSS ICH EINE TABELLE BERECHNEN
			 * JEDER VERGLEICH (VGL-FZ mit ALLEN FP-FZ) ERZEUGT EINE TABELLE!!!
			 * JEDE TABELLE ENTHÄLT MEDIAN/MITTELWERT/SD/MIN/MAX-WERTE
			 * ALSO MUSS FÜR JEDEN VERGLEICH EINE TABELLE ERSTELLT WERDEN.
			 * DAMIT IST ES RICHTIG; DASS FÜR 1 VGL-FZ nur EINE TABELLE ERSTELLT WIRD!!!!!!!!!
			 * UND ALLE FZ = DURCHSCHNITT IST
			 * MAX-WERTE FÜR ALLE-FZ = MAX-WERTE FÜR ZIELFZ!
			 *
			 * MIN-WERTE STIMMEN, WENN MEHR ALS 1 VGL-FZ EXISTIERT
			 */
		//Berechnung für alle Zielfahrzeuge
		zielfahrzeug = berechnungAuswertung(zielfahrzeug, vector_struct_zielfahrzeuge);
		// Alle Auswertungsberechnungen aller verglichenen Fahrzeuge
		allefahrzeuge = berechnungAuswertung(allefahrzeuge, vector_struct_zwischenergebnisse);
		allefahrzeuge = tPtNBerechnung(allefahrzeuge, vector_struct_zwischenergebnisse, vector_struct_zielfahrzeuge, TP1, TP2, TP3, quartil75, medianberechnung);
		vector_struct_zielfahrzeuge_tabellenwerte.push_back(zielfahrzeug);
		vector_struct_allefahrzeuge_tabellenwerte.push_back(allefahrzeuge);

		printf("Platzierung, Gesamt, Form, Farbe, FAST, GFTT, NP, Bild\n");
		for(unsigned int i=0; i < vector_struct_zwischenergebnisse.size(); i++)
		{
			printf(" %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %s\n",
					i,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_gesamt,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_Komp_1,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_Komp_2,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_Komp_3,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_Komp_4,
					vector_struct_zwischenergebnisse.at(i).Uebereinstimmung_Komp_5,
					vector_struct_zwischenergebnisse.at(i).ursprungs_fz.getPfadUrsprungsBild().c_str());


		}
		cout << "zielfahrzeuge" << endl;
		for(unsigned int i=0; i < vector_struct_zielfahrzeuge.size(); i++)
		{
			printf(" %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %s\n",
					i,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_gesamt,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_Komp_1,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_Komp_2,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_Komp_3,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_Komp_4,
					vector_struct_zielfahrzeuge.at(i).Uebereinstimmung_Komp_5,
					vector_struct_zielfahrzeuge.at(i).ursprungs_fz.getPfadUrsprungsBild().c_str());
		}
	}

		durchschnittziel = berechnungDurchschnitte(durchschnittziel,vector_struct_zielfahrzeuge_tabellenwerte,vector_struct_zielfahrzeuge_tabellenwerte.size());
		ausgabeDurchschnitt(durchschnittziel);
		durchschnittalle = berechnungDurchschnitte(durchschnittalle,vector_struct_allefahrzeuge_tabellenwerte,vector_struct_allefahrzeuge_tabellenwerte.size());
		ausgabeTPTN(vector_struct_allefahrzeuge_tabellenwerte,quartil75,medianberechnung);

	}

	waitKey();
	return 0;
}
