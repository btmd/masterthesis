/*
 * Form.h
 *
 *  Created on: 10.04.2013
 *      Author: tmd
 */

#ifndef FORM_H_
#define FORM_H_

#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class Form{

	public:

		Form();
		Form(Mat);
		virtual ~Form();

		void setSize(Mat);
		void setMarker(Mat);
		void setMaske(Mat);

		float getSize();
		Mat getErgebnisAsMat();
		Mat getMaske();


	private:

		Rect marker;
		Mat fahrzeug;
		Mat fahrzeugMaske;

		float hoehe, breite;
		float size;

		Mat result;
		Mat bgModel,fgModel;
		Mat imggray;



};


#endif /* FORM_H_ */
