/*
 * Metrik.cpp
 *
 *  Created on: 10.04.2013
 *      Author: tmd
 */

#include "../include/Metrik.h"


// Implementierung der Konstruktoren
Metrik::Metrik()
{
    this->FormvergleichProzent = 0;
    this->FarbvergleichProzent = 0;
    this->FastFeaturevergleichProzent = 0;
    this->GfttFeaturevergleichProzent = 0;
    this->NummernschildvergleichProzent = 0;
    this->gesamtWahrscheinlichkeit = 0.0;
    end = 0;
    start = 0;

}

Metrik::Metrik(Fahrzeug fz1, Fahrzeug fz2)
{
	/*
	 * Konstruktor um 2 Fahrzeuge vollständig hinsichtlich aller!! Eigenschaften zu vergleichen
	 */

    this->FormvergleichProzent = 0;
    this->FarbvergleichProzent = 0;
    this->FastFeaturevergleichProzent = 0;
    this->GfttFeaturevergleichProzent = 0;
    this->NummernschildvergleichProzent = 0;
    this->gesamtWahrscheinlichkeit = 0.0;

    /*
     * Hier erzeugt das Metrik-Objekt alle Vergleiche.
     * Die Eigenschaften der Fahrzeug-Objekte wurden bereits bei der Erstellung des Fahrzeugpolvektors erzeugt
     */

    //cout << "setze Formvergleich" << endl;
	gettimeofday(&tv1,NULL);
    start = tv1.tv_usec;

	this->setFormVergleich(fz1.getGroessenverhaeltnis(), fz2.getGroessenverhaeltnis());

	// Vergleich der Fahrzeug-Farbhistogramme
	//cout << "setze Farbvergleich" << endl;

	gettimeofday(&tv2,NULL);
	end = tv2.tv_usec;
	//cout << "Start: " << start << " end: " << end << endl;
	//cout << "Dauer Formvergleich: " << difftime(end, start) << endl;
	zeitvektor.push_back(difftime(end, start));
	start = end;
	this->setFarbVergleich(fz1.getHistogramm(), fz2.getHistogramm());

	gettimeofday(&tv3,NULL);
	end = tv3.tv_usec;
	//cout << "Start: " << start << " end: " << end << endl;
	//cout << "Dauer Farbvergleich: " << difftime(end, start) << endl;
	zeitvektor.push_back(difftime(end, start));
	start = end;
	// Vergleich der beiden berechneten Features/Keypoints
	//cout << "setze FASTvergleich" << endl;

	/*
	 * T3
	 */

	this->setFastFeatureVergleich(this->setFeatureVergleich(fz1.getFASTKeypoints(), fz2.getFASTKeypoints(), fz1.getBRIEFDescriptoren(), fz2.getBRIEFDescriptoren()));
	//cout << "setze GFTTvergleich" << endl;

	gettimeofday(&tv4,NULL);
	end = tv4.tv_usec;
//	cout << "Start: " << start << " end: " << end << endl;
//	cout << "Dauer FEATURE-FAST-Vergleich: " << difftime(end, start) << endl;
	zeitvektor.push_back(difftime(end, start));
	start = end;
	/*
	 * T4
	 */
	this->setGfttFeatureVergleich(this->setFeatureVergleich(fz1.getGFTTKeypoints(), fz2.getGFTTKeypoints(), fz1.getSIFTDescriptoren(), fz2.getSIFTDescriptoren()));

	gettimeofday(&tv5,NULL);
	end = tv5.tv_usec;
//	cout << "Start: " << start << " end: " << end << endl;
//	cout << "Dauer FEATURE-GFTT-Vergleich: " << difftime(end, start) << endl;
	zeitvektor.push_back(difftime(end, start));
	start = end;

	this->setNummernschildVergleich(fz1.getNummernschild(), fz2.getNummernschild() );

	/*
	 * Hier erfolgt die Berechnung der Gesamtwahrscheinlichkeit
	 */
	gettimeofday(&tv6,NULL);
	end = tv6.tv_usec;
	zeitvektor.push_back(difftime(end, start));
	start = end;


	float a, b, c, d, e = 0.0;
	a = this->FormvergleichProzent;
	b = this->FarbvergleichProzent;
	c = this->FastFeaturevergleichProzent;
	d = this->GfttFeaturevergleichProzent;
	e = this->NummernschildvergleichProzent;

	float p_ges = 0.0;
	p_ges = (a+b+c+d+e)/(5);
	setGesamtWahrscheinlichkeit(p_ges);

	gettimeofday(&tv7,NULL);
	end = tv7.tv_usec;
	zeitvektor.push_back(difftime(end, start));

	/*
	 * Ausgabe der Berechnungszeiten
	 */
	for (unsigned int i = 0; i<zeitvektor.size();i++) {
		cout << zeitvektor.at(i) << ", ";
	}
	cout << endl;

}

Metrik::Metrik(float Groessenverhaeltnis, Fahrzeug fz2)
{
	/*
	 * Konstruktor um 2 Fahrzeuge hinsichtlich ihrer Größe zu vergleichen
	 */

	float a = this->FormvergleichProzent;
	setGesamtWahrscheinlichkeit(a);

	//statt this->FormvergleichProzent wird direkt der setter aufgerufen
	this->setFormVergleich(Groessenverhaeltnis, fz2.getGroessenverhaeltnis());
    this->FarbvergleichProzent = 0;
    this->FastFeaturevergleichProzent = 0;
    this->GfttFeaturevergleichProzent = 0;
    this->NummernschildvergleichProzent = 0;
    this->gesamtWahrscheinlichkeit = 0.0;

}

Metrik::Metrik(MatND histogram, Fahrzeug fz2)
{
	/*
	 * Konstruktor um 2 Fahrzeuge hinsichtlich ihrer Farbe zu vergleichen
	 */


    this->FormvergleichProzent = 0;
	this->setFarbVergleich(histogram, fz2.getHistogramm());
    this->FastFeaturevergleichProzent = 0;
    this->GfttFeaturevergleichProzent = 0;
    this->NummernschildvergleichProzent = 0;
    this->gesamtWahrscheinlichkeit = 0.0;

	float a = this->FarbvergleichProzent;

	setGesamtWahrscheinlichkeit(a);

}

// Featurevergleich
Metrik::Metrik(vector<KeyPoint> FASTkeypoints, vector<KeyPoint> GFTTkeypoints, Mat BRIEFDescriptors, Mat SIFTDescriptors, Fahrzeug fz2)
{
	//cout << "Konstruktor: Nur Featurevergleich" << endl;
	/*
	 * Konstruktor um 2 Fahrzeuge hinsichtlich ihrer Features zu vergleichen
	 */

    this->FormvergleichProzent = 0;
    this->FarbvergleichProzent = 0;

    this->GfttFeaturevergleichProzent = 0;
    this->FastFeaturevergleichProzent = 0;
	this->setFastFeatureVergleich(this->setFeatureVergleich(FASTkeypoints, fz2.getFASTKeypoints(), BRIEFDescriptors, fz2.getBRIEFDescriptoren()));
	this->setGfttFeatureVergleich(this->setFeatureVergleich(GFTTkeypoints, fz2.getGFTTKeypoints(), SIFTDescriptors, fz2.getSIFTDescriptoren()));
    this->NummernschildvergleichProzent = 0;
    this->gesamtWahrscheinlichkeit = 0.0;

    float c, d = 0.0;
    c = this->FastFeaturevergleichProzent;
    d = this->GfttFeaturevergleichProzent;

    float p_ges = 0.0;
    p_ges = (c*c+d*d)/(c+d);
    setGesamtWahrscheinlichkeit(p_ges);

}


Metrik::Metrik(String Nummernschild, Fahrzeug Fahrzeug_2)
{
    /*
	 * Konstruktor um 2 Fahrzeuge anhand ihrer Kennzeichen zu vergleichen
	 */


    this->FormvergleichProzent = 0;
    this->FarbvergleichProzent = 0;
    this->GfttFeaturevergleichProzent = 0;
    this->FastFeaturevergleichProzent = 0;
    this->NummernschildvergleichProzent = 0;
    this->setNummernschildVergleich(Nummernschild, Fahrzeug_2.getNummernschild());
	this->gesamtWahrscheinlichkeit = 0.0;

}

Metrik::~Metrik(){}


void Metrik::setFormVergleich(float GroesseFz1, float GroesseFz2)
{
	if(GroesseFz1 <= GroesseFz2)
	{
		this->FormvergleichProzent = GroesseFz1 / GroesseFz2 * 100;
	} else {
		this->FormvergleichProzent = GroesseFz2 / GroesseFz1 * 100;
	}

	if(this->FormvergleichProzent != this->FormvergleichProzent) {
		this->FormvergleichProzent = 0.0;
	}

}

void Metrik::setFarbVergleich(MatND HistogrammFz1, MatND HistogrammFz2)
{

/*
 * BHATTACHARYYA
 */

	Mat H1 = HistogrammFz1, H2 = HistogrammFz2;
	const Mat* arrays[] = {&H1, &H2, 0};
	Mat planes[2];
	NAryMatIterator it(arrays, planes);
	double result = 0;
	double s1 = 0, s2 = 0;
	int j, len = (int)it.size;

	CV_Assert( H1.type() == H2.type() && H1.type() == CV_32F );
	CV_Assert( it.planes[0].isContinuous() && it.planes[1].isContinuous() );

	for( size_t i = 0; i < it.nplanes; i++, ++it )
	{
		const float* h1 = (const float*)it.planes[0].data;
		const float* h2 = (const float*)it.planes[1].data;
		len = it.planes[0].rows*it.planes[0].cols;

        for( j = 0; j < len; j++ )
        {
            double a = h1[j];
            double b = h2[j];
            result += std::sqrt(a*b);
            s1 += a;
            s2 += b;
        }
	}

    s1 *= s2;
    s1 = fabs(s1) > FLT_EPSILON ? 1./std::sqrt(s1) : 1.;
    result = std::sqrt(std::max(1. - result*s1, 0.));
    if(result != result){
    	// Der Fall, wenn -nan auftritt
    	this->FarbvergleichProzent = 0.00;
    } else {
        this->FarbvergleichProzent = (100 - (result*100));
    }

}

float Metrik::setFeatureVergleich(vector<KeyPoint> keyPointsFz1, vector<KeyPoint> keyPointsFz2, Mat DescriptorFz1, Mat DescriptorFz2)
{
	RobustFeatureMatcher rfm;

	// Referenzvektoren für die spätere Prozentberechnung
	vector <DMatch> matches_Fz1_Self;
	vector <DMatch> matches_Fz2_Self;
	vector <DMatch> matches_foreign;

	float return_Matches = 0.0;

	matches_Fz1_Self = rfm.match(keyPointsFz1,keyPointsFz1,DescriptorFz1,DescriptorFz1);
	matches_Fz2_Self = rfm.match(keyPointsFz2,keyPointsFz2,DescriptorFz2,DescriptorFz2);

	if (matches_Fz1_Self.size() < matches_Fz2_Self.size())
	{
		matches_foreign = rfm.match(keyPointsFz2,keyPointsFz1,DescriptorFz2,DescriptorFz1);
		return_Matches = (matches_foreign.size() * 100.00f) / matches_Fz2_Self.size();

	} else
	{
		matches_foreign = rfm.match(keyPointsFz1,keyPointsFz2,DescriptorFz1,DescriptorFz2);
		return_Matches = (matches_foreign.size() * 100.00f) / matches_Fz1_Self.size();
	}

	return return_Matches;

}

void Metrik::setFastFeatureVergleich(float fastfeatureVergleich)
{
	this->FastFeaturevergleichProzent = fastfeatureVergleich;
	if(this->FastFeaturevergleichProzent != this->FastFeaturevergleichProzent){
		this->FastFeaturevergleichProzent = 0.0;
	}
}

void Metrik::setGfttFeatureVergleich(float gfttFeatureVergleich)
{
	this->GfttFeaturevergleichProzent = gfttFeatureVergleich;
	if(this->GfttFeaturevergleichProzent != this->GfttFeaturevergleichProzent){
		this->GfttFeaturevergleichProzent = 0.0;
	}
}

float Metrik::nummernschildTeilVergleich(String Nummernschild_Fz_1, String Nummernschild_Fz_2)
{

	float uebereinstimmung = 0.0;
	float levenshtein_differenz = 0;
	float laenge = 0;

	if (Nummernschild_Fz_1.length() >= Nummernschild_Fz_2.length())
	{
		laenge = Nummernschild_Fz_1.length();
	} else {
		laenge = Nummernschild_Fz_2.length();
	}

	levenshtein_differenz = Metrik::levenshtein_distance(Nummernschild_Fz_1, Nummernschild_Fz_2);

	float _temp = laenge - levenshtein_differenz;

	uebereinstimmung = (_temp / laenge) * 100;

	if(uebereinstimmung != uebereinstimmung){
		uebereinstimmung = 0.0;
	}

	return uebereinstimmung;
}


void Metrik::setNummernschildVergleich(String NummernschildFz1, String NummernschildFz2)
{
	if (NummernschildFz1 ==  ""){
		this->NummernschildvergleichProzent = 0.0;
	} else if (NummernschildFz2 == ""){
		this->NummernschildvergleichProzent = 0.0;
	} else {
		this->NummernschildvergleichProzent = nummernschildTeilVergleich(NummernschildFz1, NummernschildFz2);
	}
}


float Metrik::getFormVergleichProzent()
{
	return this->FormvergleichProzent;
}

float Metrik::getFarbVergleichProzent()
{
	return this->FarbvergleichProzent;
}

float Metrik::getFastFeatureVergleichProzent()
{
	return this->FastFeaturevergleichProzent;
}

float Metrik::getGfttFeatureVergleichProzent()
{
	return this->GfttFeaturevergleichProzent;
}

float Metrik::getNummernschildVergleichProzent()
{
	return this->NummernschildvergleichProzent;
}

void Metrik::setGesamtWahrscheinlichkeit(float fgesamtWahrscheinlichkeit)
{
	this->gesamtWahrscheinlichkeit = fgesamtWahrscheinlichkeit;
}

float Metrik::getGesamtWahrscheinlichkeit()
{
	return this->gesamtWahrscheinlichkeit;
}


template<class T>
unsigned int Metrik::levenshtein_distance(const T &s1, const T & s2) {
        const size_t len1 = s1.size(), len2 = s2.size();
        vector<unsigned int> col(len2+1), prevCol(len2+1);

        for (unsigned int i = 0; i < prevCol.size(); i++)
                prevCol[i] = i;
        for (unsigned int i = 0; i < len1; i++) {
                col[0] = i+1;
                for (unsigned int j = 0; j < len2; j++)
                        col[j+1] = min( min( 1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
                col.swap(prevCol);
        }

        return prevCol[len2];
}
