/*
 * Farbe.h
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 */

#ifndef FARBE_H_
#define FARBE_H_

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class Farbe{
	public:

		Farbe(); // DefaultKonstruktor;
		Farbe(Mat img, Mat maske); // führt HSV-Transformation durch
		virtual ~Farbe(); // DefaultDestruktor

		void setFarbeHistogram(Mat img, Mat maske);
		MatND getFarbeHistogram();

	private:
		// Variablen für die Berechnung der Histogramme
		//int channels[2] = { 0, 1 };
		int channels[2];

		// Anzahl der bins für Hue und Saturation
		int h_bins;
		int s_bins;
		int histSize[2];

		// Werte, die die Farben annehmen können
		float h_ranges[2];
		float s_ranges[2];
		const float* ranges[2];

		MatND histogram;

};

#endif /* FARBE_H_ */
