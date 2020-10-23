/*
 * Fahrzeugpool.h
 *
 *  Created on: 01.11.2013
 *      Author: tmd
 */

#ifndef FAHRZEUGPOOL_H_
#define FAHRZEUGPOOL_H_

//#include "../include/Fahrzeug.h"
#include "Fahrzeug.h"

using namespace std;
using namespace cv;

class Fahrzeugpool{
public:
	Fahrzeugpool(); // Erstellung eines Fahrzeugpools (Vektor) mit den Objekten vom Typ Fahrzeug
	~Fahrzeugpool();

	// Methoden

	void addToFahrzeugpoolVektor(String Dateipfad); // fügt ein Objekt zum FahrzeugpoolVektor hinzu
	void deleteIDFromFahrzeugpoolVektor(int FahrzeugID); // entfernt ein Fahrzeug aus dem Vektor (bei Verlassen des Parkhauses)

	vector<Fahrzeug> getFahrzeugpoolVektor();
	Fahrzeug getFzByIdFromFahrzeugpoolVektor(int ID);
	vector<Fahrzeug> getSomeFahrzeugpoolVektor(int Anzahl);

private:
	vector<Fahrzeug> fahrzeugpoolVektor;

};


#endif /* FAHRZEUGPOOL_H_ */
