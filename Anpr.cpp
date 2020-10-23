/*
 * Anpr.cpp
 *
 *  Created on: 17.05.2013
 *      Author: tmd
 */

#include "../include/Anpr.h"

#include "gximage.h"
#include "cmanpr.h"

// Implementierung der Konstruktoren
Anpr::Anpr()
{
	nummernSchild = "";
}

Anpr::~Anpr(){}


// Implementierung der Methoden

void Anpr::setNummernschild(string img)
{
	const char * dateipfad= img.c_str();

	try {
		// ANPR-Objekt erzeugen
		cmAnpr anpr("default");

		// Bild-Objekt erzeugen
		gxImage image("default");

		// Bild laden
		image.Load(dateipfad);

		// sucht nach dem ersten zu findenen Kennzeichen im Bild
		if(anpr.FindFirst(image)) {
			nummernSchild = anpr.GetTextA();
		} else {
			nummernSchild = "";
		}

	} catch(gxError &e) {
		nummernSchild = "";
		// GX Exception abfangen
		wcerr << L"GX Error occurred: " << e.GetErrorString() << L"\n";

	}

	this->nummernSchild = nummernSchild;
}


string Anpr::getNummernschild()
{
	return this->nummernSchild;
}
