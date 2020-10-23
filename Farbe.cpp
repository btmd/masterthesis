/*
 * Farbe.cpp
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 */

#include "../include/Farbe.h"

using namespace cv;

Farbe::Farbe()
{

	this->channels[0] = 0;
	this->channels[1] = 1;

	// 300 Klassen für die Farbe (hue) und 320 Klassen für die Sättigung (saturation)
	this->h_bins = 1;
	this->s_bins = 1;
	this->histSize[0] = h_bins;
	this->histSize[1] = s_bins;

	// Hue- und Saturation-Werte
	h_ranges[0] = 0;
	h_ranges[1] = 256; //ursprünglich 256
	s_ranges[0] = 0;
	s_ranges[1] = 256; //ursprünglich 256
	ranges[0] = h_ranges;
	ranges[1] = s_ranges;
}

Farbe::Farbe(Mat img, Mat maske)
{

	this->channels[0] = 0;
	this->channels[1] = 1;

	// 32 Klassen für die Farbe (hue) und 32 Klassen für die Sättigung (saturation)
	this->h_bins = 32;
	this->s_bins = 32;
	this->histSize[0] = h_bins;
	this->histSize[1] = s_bins;

	// Hue- und Saturation-Werte
	h_ranges[0] = 0;
	h_ranges[1] = 256; //ursprünglich 256
	s_ranges[0] = 0;
	s_ranges[1] = 256; //ursprünglich 256
	ranges[0] = h_ranges;
	ranges[1] = s_ranges;

	setFarbeHistogram(img, maske);
}

Farbe::~Farbe(){}


// Implementierung der Methoden
void Farbe::setFarbeHistogram(Mat bild, Mat maske)
{
	Mat hsvMat;

	cvtColor(bild, hsvMat, CV_BGR2HSV_FULL);

	//calcHist( &hsvMat, 1, this->channels, Mat(), this->histogram, 2, this->histSize, this->ranges, true, false );
	calcHist(&hsvMat, 1, this->channels, maske, this->histogram, 2, this->histSize, this->ranges, true, false);

	normalize(this->histogram, this->histogram, 0, 1, NORM_MINMAX, -1, Mat());
}

MatND Farbe::getFarbeHistogram()
{
	return this->histogram;
}
