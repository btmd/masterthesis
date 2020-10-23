/*
 * Metrik.h
 *
 *  Created on: 10.04.2013
 *      Author: tmd
 */

#ifndef METRIK_H_
#define METRIK_H_

#include "stdio.h"
#include "iostream"
#include <sys/time.h>

#include "../include/RobustFeatureMatcher.h"
#include "../include/Fahrzeug.h"

using namespace std;

class Metrik {

	public:

		Metrik();
		/*
		 * Die Schnittstellen sind also:
			- Bild, Fahrzeugpool
			- Float (Verhältnis Höhe zu Breite), Fahrzeugpool
			- MatND (HSV-Histogramm), Fahrzeugpool
			- Vector<KeyPoint> (GFTT / FAST Features; SIFT oder BRIEF Descriptoren), Fahrzeugpool
			- String, Fahrzeugpool
		 */
		// IM EINFACHSTEN FALL WIRD EIN FAHRZEUG-OBJEKT ÜBERGEBEN
		//Metrik(String BildDateiPfad1, String BildDateiPfad2);
		Metrik(Fahrzeug Fahrzeug_1, Fahrzeug Fahrzeug_2);

		// ausschließlich Größenvergleich
		Metrik(float Groessenverhaeltnis, Fahrzeug Fahrzeug_2);

		// ausschließlich Farbvergleich
		Metrik(MatND, Fahrzeug Fahrzeug_2);

		// ausschließlich Featurevergleich
		Metrik(vector<KeyPoint> FASTkeypoints, vector<KeyPoint> GFTTkeypoints, Mat BRIEFDescriptors, Mat SIFTDescriptors, Fahrzeug fz2);

		// ausschließlich Nummernschildvergleich
		Metrik(String Nummernschild, Fahrzeug Fahrzeug_2);

		virtual ~Metrik();

		void setFormVergleich(float, float);
		void setFarbVergleich(MatND, MatND);
		float setFeatureVergleich(vector<KeyPoint>, vector<KeyPoint>, Mat, Mat);
		void setFastFeatureVergleich(float);
		void setGfttFeatureVergleich(float);
		float nummernschildTeilVergleich(String Nummernschild_Fahrzeug_1, String Nummernschild_Fahrzeug_2);
		void setNummernschildVergleich(String Nummernschild_Fahrzeug_1, String Nummernschild_Fahrzeug_2);
		void setGesamtWahrscheinlichkeit(float);
		void draw_subdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color);
		template<class T> unsigned int levenshtein_distance(const T &s1, const T & s2);

		float getFormVergleichProzent();
		float getFarbVergleichProzent();
		float getFastFeatureVergleichProzent();
		float getGfttFeatureVergleichProzent();
		float getNummernschildVergleichProzent();
		float getGesamtWahrscheinlichkeit();

	private:
		// BEI DEN NÖTIGEN VEKTOREN BIN ICH MIR NOCH NICHT SICHER, WELCHE ICH TATSÄCHLICH UND IN WELCHER FORM BENÖTIGE
		// Vektor, der alle berechneten Fahrzeuge speichert -- TODO: WIRD EINE EIGENE KLASSE!

		vector<Fahrzeug> fahrzeugvektor; // alle erkannten Fahrzeuge

		// Vektor, der alle eingehenden Bilder speichert
		vector<Fahrzeug> inputVector; // ~ 10 Fahrzeuge, die als Quelle dienen

		// Vektor, der alle Ergebnisse, ihrer Reihenfolge entsprechend speichert
		vector<Fahrzeug> ergebnisVector; // die besten 10 Ergebnisse

		// Hier sind Variablen drin, die die Ergebnisse der Metrikklasse beinhalten!

		//Formvergleich:
		// Wie vergleiche ich 2 Verhältnisse? Size1 : Size 2 ergibt ????? Welche Dimension hat das Ergebnis?
		// im Endeffekt muss jedes Ergebnis eine Prozentzahl ausspucken
		float FormvergleichProzent;

		//Farbvergleich:
		float FarbvergleichProzent;

		//Featurevergleich
		vector<DMatch> fastmatches; // das muss hier rein, weil ich erst hier die matches erzeugen kann!
		vector<DMatch> gfttmatches; // da kann ich sie auch zwicchenspeichern
		//float FeaturevergleichProzent;
		float FastFeaturevergleichProzent;
		float GfttFeaturevergleichProzent;

		//Nummernschildvergleich
		float NummernschildvergleichProzent;

		float gesamtWahrscheinlichkeit;

		struct timeval tv1;
		struct timeval tv2;
		struct timeval tv3;
		struct timeval tv4;
		struct timeval tv5;
		struct timeval tv6;
		struct timeval tv7;

		time_t start;
		time_t end;

		vector<time_t> zeitvektor;
};

#endif /* METRIK_H_ */
