/*
 * Anpr.h
 *
 *  Created on: 17.05.2013
 *      Author: tmd
 */

#ifndef ANPR_H_
#define ANPR_H_

#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;


class Anpr{
	public:
		// Konstruktoren, Methoden, oeffentliche Variablen, getter und setter der privaten Variablen
		Anpr(); // DefaultKonstruktor; nutzt HSV-Transformation
		virtual ~Anpr(); // DefaultDestruktor

		void setNummernschild(string img);
		string getNummernschild();

	private:
		string nummernSchild;

};



#endif /* ANPR_H_ */
