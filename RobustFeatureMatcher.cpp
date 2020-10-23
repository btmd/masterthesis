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
 * RobustFeatureMatcher.cpp
 *
 *  Created on: 01.02.2013
 *		Author: tmd
 */

#include "../include/RobustFeatureMatcher.h"
#include "stdio.h"
#include <iostream>


RobustFeatureMatcher::RobustFeatureMatcher()
{
	//DEFAULT-Werte
	this->ratio = 0.70;
	this->refineF = true;
	this->confidence = 0.90;
	this->distance = 1.50;

	detector= new FastFeatureDetector;
	extractor= new BriefDescriptorExtractor;
}

RobustFeatureMatcher::~RobustFeatureMatcher(){}

// setze Vertrauensinterval
void RobustFeatureMatcher::setConfidenceLevel(double setconfidence)
{
	this->confidence = setconfidence;
}

// kleinste Distanz zum Epipolar(?)
void RobustFeatureMatcher::setMinDistanceToEpipolar(double mindist)
{
	this->distance = mindist;
}

// Ratio setzen
void RobustFeatureMatcher::setRatio(float ratio)
{
	this->ratio = ratio;
}

// set refineF
void RobustFeatureMatcher::setRefineF(bool refineF)
{
	this->refineF = refineF;
}

// Set the feature detector
void RobustFeatureMatcher::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect)
{
	detector= detect;
}

// Set the descriptor extractor
void RobustFeatureMatcher::setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc)
{
	extractor= desc;
}

// lies Vertrauensinterval
double RobustFeatureMatcher::getConfidenceLevel()
{
	return this->confidence;
}

// kleinste Distanz zum Epipolar(?)
double RobustFeatureMatcher::getMinDistanceToEpipolar()
{
	return this->distance;
}

// Ratio lesen
float RobustFeatureMatcher::getRatio()
{
	return this->ratio;
}

// lies refineF
bool RobustFeatureMatcher::getRefineF()
{
	return this->refineF;
}

// Match feature points using symmetry test and RANSAC
vector<DMatch> RobustFeatureMatcher::match(vector<KeyPoint> keypoints0, vector<KeyPoint> keypoints1, Mat descriptors0, Mat descriptors1)
{
	vector<DMatch> matches;

	// 2. Descriptoren vergleichen
	cv::BruteForceMatcher<cv::L2<float> > matcher;

	// Vektor matches0 enthält die nächsten Nachbarn im Vergleich der Descriptoren von Bild0 zu Bild1 (bis zu k=2 pro Eintrag)
	vector<vector<DMatch> > matches0;
	matcher.knnMatch(descriptors0, descriptors1, matches0, 2);

	// Vektor matches1 enthält die nächsten Nachbarn im Vergleich der Descriptoren von Bild0 zu Bild1 (bis zu k=2 pro Eintrag)
	vector<vector<DMatch> > matches1;
	matcher.knnMatch(descriptors1, descriptors0, matches1, 2);

	// 3. Validate matches using RANSAC
	//Mat fundemental = ransacTest(symmetryTest(matches0, matches1), keypoints0, keypoints1, matches);
	ransacTest(symmetryTest(matches0, matches1), keypoints0, keypoints1, matches);

	// return der besten Matches, die den crossCheck überstanden haben
	//return fundemental;

	return matches;
}


// symmetrische Matches werden in den symMatches Vektor geschrieben
vector<DMatch> RobustFeatureMatcher::symmetryTest(const std::vector<std::vector<cv::DMatch> >& symTestmatches1,const std::vector<std::vector<cv::DMatch> >& symTestmatches2)
{
	vector<cv::DMatch> symMatches;

	// Fuer alle Matches von Fahrzeug 1 zu Fahrzeug 2
	for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator1= symTestmatches1.begin();	matchIterator1!= symTestmatches1.end(); ++matchIterator1)
	{
		//std::cout << "in symmetrytest " << std::endl;
		// ignore deleted matches
		if (matchIterator1->size() < 2)
			continue;
			// Fuer alle Matches von Fahrzeug 2 zu Fahrzeug 1
		for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator2= symTestmatches2.begin(); matchIterator2!= symTestmatches2.end(); ++matchIterator2)
		{
			// ignore deleted matches
			if (matchIterator2->size() < 2)
				continue;

			// Match symmetry test
			if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx && (*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx)
			{

				// add symmetrical match
				symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,(*matchIterator1)[0].trainIdx,(*matchIterator1)[0].distance));
				break; // next match in image 1 -> image 2
			}
		}
	}

	return symMatches;
}

// Identify good matches using RANSAC
// Return fundemental matrix
Mat RobustFeatureMatcher::ransacTest( const vector<DMatch>& matches, const vector<KeyPoint>& keypoints1, const vector<cv::KeyPoint>& keypoints2, vector<DMatch>& outMatches)
{

	/*
	 * DEBUGAUSGABEN
	std::cout << "matches: " << matches.size() << std::endl;
	std::cout << "keypoints1: " << keypoints1.size() << std::endl;
	std::cout << "keypoints2: " << keypoints2.size() << std::endl;
*/
	// Convert keypoints into Point2f
	std::vector<cv::Point2f> points1, points2;
	for (std::vector<cv::DMatch>::const_iterator it= matches.begin();it!= matches.end(); ++it)
	{

		// Get the position of left keypoints
		float x= keypoints1[it->queryIdx].pt.x;
		float y= keypoints1[it->queryIdx].pt.y;
		points1.push_back(cv::Point2f(x,y));

		// Get the position of right keypoints
		x= keypoints2[it->trainIdx].pt.x;
		y= keypoints2[it->trainIdx].pt.y;
		points2.push_back(cv::Point2f(x,y));
	}

	// Compute F matrix using RANSAC
	std::vector<uchar> inliers(points1.size(),0);

	// points1, points2 = matching points; inliers = match status (inlier or outlier); CV_FM_RANSAC = RANSAC method;
	// distance = distance to epipolar line; confidence = confidence propability
	/*DEBUG
	std::cout << "findFundamentalMat() " << std::endl;
	std::cout << "inliers.size() " << inliers.size() << std::endl;
	std::cout << "outmatches.size() " << outMatches.size() << std::endl;
	std::cout << "points1.size() " << points1.size() << std::endl;
	std::cout << "points2.size() " << points2.size() << std::endl;
	std::cout << "distance " << distance << std::endl;
	std::cout << "confidence " << confidence << std::endl;
*/
	Mat fundamental;

	if(inliers.size() != 0)
	{
		fundamental= cv::findFundamentalMat( Mat(points1), Mat(points2), inliers, CV_FM_RANSAC, distance, confidence);
	}

	//DEBUG std::cout << "wie viele surviving inliers? " << std::endl;
	// extract the surviving (inliers) matches
	std::vector<uchar>::const_iterator itIn= inliers.begin();
	std::vector<cv::DMatch>::const_iterator	itM= matches.begin();

	//DEBUG std::cout << "inliers.size() " << inliers.size() << std::endl;
	// für alle outMatches
	for ( ;itIn!= inliers.end(); ++itIn, ++itM)
	{
		// Falls es ein gültiger Wert ist
		if (*itIn) { outMatches.push_back(*itM);}
	}

	//wenn outMatches.size() = 0, dann crasht die cv::fundamentalMat()
/*DEBUG
	std::cout << "refineF " << refineF << std::endl;
	std::cout << "outmatches.size() " << outMatches.size() << std::endl;
	std::cout << "" << std::endl;
*/
	if (refineF && outMatches.size()!=0)
	{
		//DEBUG std::cout << "refineF && outmatches.size() != 0" << std::endl;
		// The F matrix will be recomputed with all accepted matches
		// Convert keypoints into Point2f for final F computation
		points1.clear();
		points2.clear();
/*DEBUG
		std::cout << "points-vektoren sind clear" << std::endl;
		std::cout << "points1.size "<< points1.size() << std::endl;
		std::cout << "points2.size "<< points2.size() << std::endl;
*/
		//DEBUG std::cout << "outMatches.size bevor die keypoints konvertiert werden: " << outMatches.size() << std::endl;
		for (std::vector<cv::DMatch>::const_iterator it= outMatches.begin(); it!= outMatches.end(); ++it)
		{
			// Get the position of left keypoints
			//DEBUG std::cout << "// Get the position of left keypoints" << std::endl;
			float x= keypoints1[it->queryIdx].pt.x;
			float y= keypoints1[it->queryIdx].pt.y;
			points1.push_back(cv::Point2f(x,y));

			//DEBUG: std::cout << "// Get the position of right keypoints" << std::endl;
			// Get the position of right keypoints
			x= keypoints2[it->trainIdx].pt.x;
			y= keypoints2[it->trainIdx].pt.y;
			points2.push_back(cv::Point2f(x,y));
		}
/*
 * DEBUG
		std::cout << "points-vektoren sind voll" << std::endl;
		std::cout << "points1.size "<< points1.size() << std::endl;
		std::cout << "points2.size "<< points2.size() << std::endl;

		std::cout << "alle keypoints konvertiert..." << std::endl;
		std::cout << "berechne fundamentalMat..." << std::endl;
		std::cout << "mat(points1).cols:" << Mat(points1).cols << std::endl;
		std::cout << "mat(points1).rows:" << Mat(points1).rows << std::endl;
		std::cout << "mat(points2).cols:" << Mat(points2).cols << std::endl;
		std::cout << "mat(points2).rows:" << Mat(points2).rows << std::endl;
*/
		// Compute 8-point F from all accepted matches
		fundamental= cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2), CV_FM_8POINT); // 8-point method
	}

	return fundamental;
}
