/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:
   Computer Vision Programming using the OpenCV Library.
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify,
   and distribute this source code, or portions thereof, for any purpose, without fee,
   subject to the restriction that the copyright notice may not be removed
   or altered from any source or altered source distribution.
   The software is released on an as-is basis and without any warranties of any kind.
   In particular, the software is not guaranteed to be fault-tolerant or free from failure.
   The author disclaims all warranties with regard to this software, any use,
   and any consequent failure, is purely the responsibility of the user.

   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

/*
 * RobustFeatureMatcher.h
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 */

#ifndef ROBUSTFEATUREMATCHER_H_
#define ROBUSTFEATUREMATCHER_H_

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;

class RobustFeatureMatcher {
	public:
		RobustFeatureMatcher();		// Konstruktor (mit Defaultwerten)
		~RobustFeatureMatcher();	// Destruktor

		void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);
		void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);

		void setConfidenceLevel(double confidence);
		void setMinDistanceToEpipolar(double mindist);
		void setRatio(float ratio);
		void setRefineF(bool refineF);

		float getRatio();
		double getMinDistanceToEpipolar();
		double getConfidenceLevel();
		bool getRefineF();

		vector<DMatch> match(vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, Mat descriptors0, Mat descriptors1);
		vector<DMatch> symmetryTest(const vector<vector<DMatch> >& matches1,const vector<vector<DMatch> >& matches2);
		Mat ransacTest(const vector<DMatch>& matches, const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2, std::vector<cv::DMatch>& outMatches);

	private:
		Ptr<FeatureDetector> detector;
		Ptr<DescriptorExtractor> extractor;

		float ratio; // maximale ratio zwischen dem ersten und zweiten NN
		bool refineF; // 'true' verfeinert die Fundamental-Matrix
		double distance; // kleinste Distanz zum Epipolar
		double confidence; // Wahrscheinlichtkeit / Konfidenzlevel

};

#endif /* ROBUSTFEATUREMATCHER_H_ */
