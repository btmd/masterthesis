/*
 * Fahrzeug.cpp
 *
 *  Created on: 24.05.2013
 *      Author: tmd
 */

#include "../include/Fahrzeug.h"

Fahrzeug::Fahrzeug()
{
	this->groessenverhaeltnis = 0.0;
}

Fahrzeug::~Fahrzeug(){};

Fahrzeug::Fahrzeug(String bilddatei)
{
	this->groessenverhaeltnis = 0.0;

	//cout << "setze MatParameter(bilddatei)" << endl;
	//cout << "bilddatei: " << bilddatei << endl;
	setMatParameter(bilddatei);

	//cout << "setMatParameter geschafft " << endl;
	//DEBUG imshow("testbild", this->ursprungsBild);

	setFormParameter(this->ursprungsBild);
	//cout << "Formparameter gesetzt" << endl;
	setFarbParameter(this->ursprungsBild);
	//cout << "Farbparameter gesetzt" << endl;
	//setFeatureParameter(this->getGrabCutErgebnisFreigestelltesFahrzeug());
	setFeatureParameter(this->ursprungsBild);
	//cout << "Featureparameter gesetzt" << endl;
	setAnprParamter(this->pfadUrsprungsBild);
	//cout << "Anpr-Parameter gesetzt" << endl;

}

/**********
 * Setter *
 **********/
	void Fahrzeug::setFormParameter(Mat mEingangsbild)
	{
		//DEBUG cout << "in setFormParameter; rufe Form-Konstruktor auf..." << endl;
		//if(mEingangsbild.empty()){cout << "mEingangsbild is empty" << endl;}
		Form form(mEingangsbild);
		//DEBUG cout << "Formkonstruktor erfolgreich aufgerufen" << endl;
		//form.setSize(mEingangsbild);

		this->groessenverhaeltnis = form.getSize();
		this->grabCutErgebnisFreigestelltesFahrzeug = form.getErgebnisAsMat();
		this->maskeGrabCut = form.getMaske();
	}

	void Fahrzeug::setFarbParameter(Mat mEingangsbild)
	{
		Farbe farbe(mEingangsbild, this->maskeGrabCut);
		//farbe.setFarbeHistogram(mEingangsbild);
		this->histogramm = farbe.getFarbeHistogram();
	}


	void Fahrzeug::setFeatureParameter(Mat mEingangsbild)
	{
		Feature feature;

		feature.setGFTTFeatures(mEingangsbild);
		this->gfttkeypoints = feature.getGFTTFeatures();

		feature.setFASTFeatures(mEingangsbild);
		this->fastkeypoints = feature.getFASTFeatures();

		feature.setSIFTDescriptors(mEingangsbild);
		this->siftdescriptoren = feature.getSIFTDescriptors();

		feature.setBRIEFDescriptors(mEingangsbild);
		 this->briefdescriptoren = feature.getBRIEFDescriptors();
	}

	void Fahrzeug::setAnprParamter(String)
	{
		Anpr anpr;
		anpr.setNummernschild(this->pfadUrsprungsBild);
		this->nummernschild = anpr.getNummernschild();
	}


	void Fahrzeug::setMatParameter(String sEingangsbild)
	{
		this->pfadUrsprungsBild = sEingangsbild;
		this->ursprungsBild = imread(sEingangsbild);
	}

/**********
 * Getter *
 **********/
	float Fahrzeug::getGroessenverhaeltnis()
	{
		return this->groessenverhaeltnis;
	}

	Mat Fahrzeug::getGrabCutErgebnisFreigestelltesFahrzeug()
	{
		return this->grabCutErgebnisFreigestelltesFahrzeug;
	}

	Mat Fahrzeug::getMaskgrabCut()
	{
		return this->maskeGrabCut;
	}

	MatND Fahrzeug::getHistogramm()
	{
		return this->histogramm;
	}

	vector<KeyPoint> Fahrzeug::getGFTTKeypoints()
	{
		return this->gfttkeypoints;
	}

	vector<KeyPoint> Fahrzeug::getFASTKeypoints()
	{
		return this->fastkeypoints;
	}

	Mat Fahrzeug::getSIFTDescriptoren()
	{
		return this->siftdescriptoren;
	}

	Mat Fahrzeug::getBRIEFDescriptoren()
	{
		return this->briefdescriptoren;
	}

	String Fahrzeug::getNummernschild()
	{
		return this->nummernschild;
	}

	Mat Fahrzeug::getUrsprungsBild()
	{
		return this->ursprungsBild;
	}

	String Fahrzeug::getPfadUrsprungsBild()
	{
		return this->pfadUrsprungsBild;
	}

void Fahrzeug::showHistogramm()
{
	imshow("histogramm", getHistogramm());
}

void Fahrzeug::showKeypoints()
/*
 * sinnvollerweise wird die matrix mit den gemalten Keypoints einfach returned, um sie parallel anzeigen zu lassen.
 */
{
	Mat paintedFeatures1, paintedFeatures2;
	Mat orignal_Fahrzeug = getGrabCutErgebnisFreigestelltesFahrzeug();
//	Scalar red = Scalar(0, 0, 255);
//	Scalar green = Scalar(0, 255, 0);
	getUrsprungsBild().copyTo(paintedFeatures1);
	getUrsprungsBild().copyTo(paintedFeatures2);

/*
	drawKeypoints(orignal_Fahrzeug, gfttkeypoints, paintedFeatures1, red);
	drawKeypoints(orignal_Fahrzeug, fastkeypoints, paintedFeatures2, green);

	imshow("fastKeypoints", paintedFeatures2);
	imshow("gfttKeypoints", paintedFeatures1);
*/
}

void Fahrzeug::showNummernschild()
{
	cout << "Nummernschild: " << this->nummernschild << endl;
}
