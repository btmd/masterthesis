/*
 * Feature.h
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 *
 * Beschreibung der Klasse:
 * Ich erzeuge ein Objekt der Klasse FeatureMatching in der main()-Methode.
 * Dieses Objekt liest 2 Bilder ein.
 * Der FeatureMatcher verwendet den RobustFeatureMatcher, um einfache Features zu vergleichen.
 * Das Ergebnis ist dann der Rückgabewert.
 *
 * TODO:
 * Außerdem soll der FeatureMatcher D-Netze verwenden können.
 * Diese stellen entweder eine Alternative oder einen zusätzlichen Vergleichsmodus dar.
 * Eine Alternative sind sie, wenn die Berechnung beider Vergleiche nicht performant ist.
 * Ein zusätzlicher Vergleich damit ist sinnvoll, wenn dadurch das Ergebnis deutlich besser wird.
 * Dabei muss aber das Verhältnis von (Rechen-)Aufwand zu Ergebnisverbesserung beachtet werden.
 */

#ifndef FEATURE_H_
#define FEATURE_H_

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


class Feature{
	public:

		Feature();
		~Feature();

		void setGFTTFeatures(Mat ursprungsBildausschnitt);
		void setSIFTDescriptors(Mat ursprungsBildausschnitt);

		void setFASTFeatures(Mat ursprungsBildausschnitt);
		void setBRIEFDescriptors(Mat ursprungsBildausschnitt);

		vector<KeyPoint> getGFTTFeatures();
		Mat getSIFTDescriptors();
		vector<KeyPoint> getFASTFeatures();
		Mat getBRIEFDescriptors();

	private:
		Ptr<FeatureDetector> gfttdetector, fastdetector;
		Ptr<DescriptorExtractor> siftextractor, briefextractor;

		vector<KeyPoint> gfttkeypoints, fastkeypoints;
		Mat siftdescriptors, briefdescriptors;
};


#endif /* FEATURE_H_ */
