/*
 * Feature.cpp
 *
 *  Created on: 01.02.2013
 *      Author: tmd
 */

#include "../include/Feature.h"

using namespace std;
using namespace cv;

Feature::Feature()
{
	/*
	 * Stand: 5.11.13
	 */
	gfttdetector = new GFTTDetector();
	siftextractor = new SiftDescriptorExtractor();

	fastdetector = new FastFeatureDetector();
	briefextractor = new BriefDescriptorExtractor();

	/*
	 * Experiment 5.11.13
	 * GFTT/SIFT = SIFT/SIFT
	 * FAST/BRIEF = GFTT/ORB
	 */
	/*
	gfttdetector = new SiftFeatureDetector;
	siftextractor = new SiftDescriptorExtractor;

	fastdetector = new GoodFeaturesToTrackDetector;
	briefextractor = new OrbDescriptorExtractor;
	*/
}

Feature::~Feature(){}


void Feature::setGFTTFeatures(Mat img)
{
	gfttdetector->detect(img, this->gfttkeypoints);
}


void Feature::setSIFTDescriptors(Mat img)
{
	this->siftextractor->compute(img, this->gfttkeypoints, this->siftdescriptors);
}


void Feature::setFASTFeatures(Mat img)
{
	fastdetector->detect(img, this->fastkeypoints);
}


void Feature::setBRIEFDescriptors(Mat img)
{
	this->briefextractor->compute(img, this->fastkeypoints, this->briefdescriptors);
}


vector<KeyPoint> Feature::getGFTTFeatures()
{
	return this->gfttkeypoints;
}


vector<KeyPoint> Feature::getFASTFeatures()
{
	return this->fastkeypoints;
}


Mat Feature::getSIFTDescriptors()
{
	return this->siftdescriptors;
}


Mat Feature::getBRIEFDescriptors()
{
	return this->briefdescriptors;
}
