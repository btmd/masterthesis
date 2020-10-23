/*
 * Form.cpp
 *
 *  Created on: 10.04.2013
 *      Author: tmd
 */

#include "../include/Form.h"

// Implementierung der Konstruktoren
Form::Form()
{
	this->hoehe = 0.0;
	this->breite = 0.0;
	this->size = 0.0;
}

Form::~Form(){}

/*
 * Konstruktor, in dem ich nur die Matrix übergebe und damit in höheren Klassen Befehle wegfallen
 */
Form::Form(Mat img)
{
	this->hoehe = 0.0;
	this->breite = 0.0;
	this->size = 0.0;
	setSize(img);
}

// Implementierung der Methoden
void Form::setMarker(Mat img)
{
	/*
	 * Marker-Größe ist abhängig von Lot und Lane!
	 * Marker und Lot muss größer, Marker und Lane muss etwas enger
	 */
	/*
	Point p1(img.size.p[1]*0.05,img.size.p[0]*0.05);
	Point p2(img.size.p[1]*0.95,img.size.p[0]*0.95);
	 */

	Point p1(img.size.p[1]*0.15,img.size.p[0]*0.15);
	Point p2(img.size.p[1]*0.80,img.size.p[0]*0.80);

	this->marker = Rect(p1,p2);

}

void Form::setSize(Mat img)
{

	float size = 0.0;
	Form::setMarker(img);

/*
	Mat grabmarker;
	img.copyTo(grabmarker);
	rectangle(grabmarker, this->marker, Scalar(0,255,0), 1, 8, 0 );
	imshow("Marker für GrabCut", grabmarker);
*/
	//DEBUG cout << "vor Grabcut" << endl;

	/*
	if(img.empty()){cout << "img.empty = true" << endl;}
	if(result.empty()){cout << "result.empty = true" << endl;}
	if(marker.height){cout << "marker.height = true" << endl;}
	if(img.empty()){cout << "img.empty = true" << endl;}
	if(bgModel.empty()){cout << "this->bgModel.empty = true" << endl;}
	if(fgModel.empty()){cout << "this->fgModel.empty = true" << endl;}
*/


	grabCut(img, result, this->marker, this->bgModel, this->fgModel, 4, GC_INIT_WITH_RECT);
	//DEBUG cout << "nach grabcut" << endl;

	//DEBUG cout << "vor compare" << endl;
	compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);

	//DEBUG cout << "nach compare" << endl;
	//definiert den Vordergrund
	result= result&1; // Vordergrundpixel werden 1
	//DEBUG cout << "result-matrix gesetzt" << endl;

	this->fahrzeugMaske = result.clone();
	//DEBUG cout << "vordergrund in eigene matrix kopiert" << endl;
	/*
	 * result ist meine Maske, die ich später benötige
	 */

	Mat markermat(img.size(),CV_8UC3, cv::Scalar(0,0,0));
	//DEBUG cout << "matrix markermat gesetzt" << endl;
	img.copyTo(markermat, result); // ohne BG
	//DEBUG cout << "img.copyto von markermat und result ausgeführt" << endl;
	// Mat fahrzeug als Rückgabewert in das Objekt Fahrzeug
	// agiert als Maske für spätere Berechnungen

	//DEBUG cout << "this->fahrzeug = markermat.clone();" << endl;
	this->fahrzeug = markermat.clone();
	//DEBUG cout << "this->fahrzeug = markermat.clone(); geschafft" << endl;


	/*
	 * DEBUG
	 */
//	imshow("ergebnis von Grabcut", fahrzeug);

	//cout << "" << endl;
	//DEBUG cout << "cvtColor(markermat, imggray, CV_RGB2GRAY);" << endl;

	// Grauwertumwandlung zur Konturenfindung
	cvtColor(markermat, imggray, CV_RGB2GRAY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// gefundene Konturen speichern
	//DEBUG cout << "findcontours?" << endl;
	findContours(imggray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	/*DEBUG
	cout << "konturen gefunden..." << endl;
	cout << "contours beinhaktet die konturen" << endl;
	cout << "contours.size(): " << contours.size() << endl;
	 */
	if (contours.size() != 0)
	{
		// BoundingRect der größten Kontur ist das Fahrzeug
		int boundRectContourIndex = 0; // Index der breitesten Kontur
		int maxHeight = 0;
			for(unsigned int i=0;i<contours.size();i++){

				if(maxHeight < boundingRect(Mat(contours[i])).height)
				{
					maxHeight = boundingRect(Mat(contours[i])).height;
					// boundRectContourIndex der Index der breitesten Kontur
					boundRectContourIndex = i;
				}
			}

			this->hoehe = boundingRect(Mat(contours[boundRectContourIndex])).height;
			this->breite = boundingRect(Mat(contours[boundRectContourIndex])).width;
			//DEBUG cout << "höhe und breite: " << this->hoehe << " und " << this->breite<< endl;
	} else {
		this->hoehe = 0.0;
		this->breite = 0.0;
	}



	//DEBUG cout << "höhe und breite: " << this->hoehe << " und " << this->breite<< endl;

	// Cast von int nach float
	float fhoehe = this->hoehe;
	size = fhoehe / this->breite;
	//DEBUG cout << "alles von int nach float gecastet" << endl;

	this->size = size;
}

float Form::getSize()
{
	return this->size;
}

Mat Form::getErgebnisAsMat()
{
	return fahrzeug;
}

void Form::setMaske(Mat maske)
{
	this->fahrzeugMaske = maske;
}

Mat Form::getMaske()
{
	return this->fahrzeugMaske;
}
