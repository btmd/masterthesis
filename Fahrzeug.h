/*
 * Fahrzeug.h
 *
 *  Created on: 24.05.2013
 *      Author: tmd
 */

#ifndef FAHRZEUG_H_
#define FAHRZEUG_H_

#include "stdio.h"
#include "iostream"
/**
 * Performancemessung; DEBUG-Option! Kann im FINAL raus
 */
#include <time.h>

#include "../include/Feature.h"
#include "../include/Form.h"
#include "../include/Farbe.h"
#include "../include/Anpr.h"

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

class Fahrzeug{
public:
	Fahrzeug();
	Fahrzeug(String bilderPfad); // Berechnung aller Komponenten für das jeweilige Bild
	~Fahrzeug();

/**********
 * Setter *
 **********/
	void setFormParameter(Mat Eingangsbild);

	void setFarbParameter(Mat Eingangsbild);

	void setFeatureParameter(Mat Eingangsbild);

	void setAnprParamter(String);

	void setMatParameter( String Eingangsbild);

/**********
 * Getter *
 **********/
	float getGroessenverhaeltnis();
	Mat getGrabCutErgebnisFreigestelltesFahrzeug();
	Mat getMaskgrabCut();

	MatND getHistogramm();

	vector<KeyPoint> getGFTTKeypoints();
	vector<KeyPoint> getFASTKeypoints();
	Mat getSIFTDescriptoren();
	Mat getBRIEFDescriptoren();

	String getNummernschild();

	Mat getUrsprungsBild();
	String getPfadUrsprungsBild();

	void showHistogramm();
	void showKeypoints();
	void showMatches();
	void showNummernschild();

private:
	Mat ursprungsBild;
	Mat maskeGrabCut;
	String pfadUrsprungsBild; // Wichtig für ANPR

	float groessenverhaeltnis;
	Mat grabCutErgebnisFreigestelltesFahrzeug;

	MatND histogramm;

	vector <KeyPoint> gfttkeypoints, fastkeypoints;
	Mat siftdescriptoren, briefdescriptoren;

	String nummernschild;

};


#endif /* FAHRZEUG_H_ */
