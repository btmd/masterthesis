/*
 * Fahrzeugpool.cpp
 *
 *  Created on: 01.11.2013
 *      Author: tmd
 */

#include "../include/Fahrzeugpool.h"

Fahrzeugpool::Fahrzeugpool()
{
	//cout << " Fahrzeugpool erstellt..." << endl;
}

Fahrzeugpool::~Fahrzeugpool(){};

void Fahrzeugpool::addToFahrzeugpoolVektor(String dateiPfad){
	//cout << "in addToFahrzeugpoolVektor" << endl;
	Fahrzeug fahrzeug(dateiPfad);
	this->fahrzeugpoolVektor.push_back(fahrzeug);
}

void Fahrzeugpool::deleteIDFromFahrzeugpoolVektor(int FahrzeugID){
	//TODO: this->fahrzeugpoolVektor.erase(this->fahrzeugpoolVektor[FahrzeugID]);
}

vector<Fahrzeug> Fahrzeugpool::getFahrzeugpoolVektor(){
	return this->fahrzeugpoolVektor;
}

Fahrzeug Fahrzeugpool::getFzByIdFromFahrzeugpoolVektor(int ID){

	return this->fahrzeugpoolVektor[ID];
}

vector<Fahrzeug> Fahrzeugpool::getSomeFahrzeugpoolVektor(int Anzahl){

	vector<Fahrzeug> anzahlErgebnisse;

	for (int i=0;i<Anzahl;i++){
		anzahlErgebnisse.at(i) = this->fahrzeugpoolVektor.at(i);
		//anzahlErgebnisse.[i] = this->fahrzeugpoolVektor.[i];
	}

	return anzahlErgebnisse;
}

