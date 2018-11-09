#include "Utility.h"
#include "ImageConvert.h"
#include "Scaling.h"
#include "Gradient.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2\opencv.hpp>
#include "Eye.h"
#include <algorithm>
using namespace std;
using namespace cv;
#define SIZE 256

//cvtColor(img_ori, img_gray, CV_BGR2GRAY);
//resize(img_gray, img_resize, Size(256, 256), 0, 0, CV_INTER_LINEAR);
//GaussianBlur(iimg_ori1, iiimg_ori1, Size(3, 3), 0, 0);
#define CELLSIZE 4

void collect_trainImage(Mat& trainingData, Mat& labels, int Nimages) {
	for (int i = 1; i < Nimages+1; i++) {
		string fname = format("../ImageExam/ImageProject/aeye/aeye%03d.bmp", i);
		Mat img = imread(fname, 0);
		CV_Assert(img.data);
		Mat img_resize;
		resize(img, img_resize, Size(128, 128), 0, 0, CV_INTER_LINEAR);


		Mat temp = img_resize.reshape(1, 1);
		int label = (i < 30) ? 1 : 0;
		trainingData.push_back(temp);
		labels.push_back(label);
	}
	trainingData.convertTo(trainingData, CV_32FC1);
}
void write_traindata(string fn, Mat trainingData, Mat classes) {
	FileStorage fs(fn, FileStorage::WRITE);
	fs << "TrainingData" << trainingData;
	fs << "classes" << classes;
	fs.release();
}

void read_trainData(string fn, Mat& trainingData, Mat& labels = Mat()) {
	FileStorage fs(fn, FileStorage::READ);
	CV_Assert(fs.isOpened());

	fs["TrainingData"] >> trainingData;
	fs["classes"] >> labels;
	fs.release();
	trainingData.convertTo(trainingData, CV_32FC1);
}

Ptr<ml::SVM> SVM_create(int type, int max_iter, double epsilon) {
	Ptr<ml::SVM> svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setGamma(1);
	svm->setC(1);
	TermCriteria criteria(type, max_iter, epsilon);
	svm->setTermCriteria(criteria);
	return svm;
}

CV_WRAP static Ptr<ml::SVM> load(const String &fn)
{
	return Algorithm::load<ml::SVM>(fn);
}

int main() {

	Mat trainingData, labels;
	read_trainData("../ImageExam/SVMDATA.xml", trainingData, labels);

	Ptr<ml::SVM> svm = SVM_create(CV_TERMCRIT_ITER, 1000, 0);
	svm->train(trainingData, ml::ROW_SAMPLE, labels);

	//Ptr<ml::SVM> svm = ml::StatModel::load<ml::SVM>("../SVMtrain.xml");
	//Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>("../SVMtrain.xml");

	Mat image = imread("../ImageExam/ImageProject/aeye/aeye013.bmp", 1);
	Mat img_resize;
	resize(image, img_resize, Size(128, 128), 0, 0, CV_INTER_LINEAR);

	CV_Assert(img_resize.data);
	Mat compare = img_resize.reshape(1, 1);
	compare.convertTo(compare, CV_32FC1);
	
	Mat result;
	svm->predict(compare, result);
	if (result.at<float>(0) == 1) {
		printf("성공이다임마\n");
	}
	else {
		printf("다시해라임마\n");
	}
	return 0;
}


/*	Mat trainingData, labels;
	collect_trainImage(trainingData, labels, 38);
	write_traindata("SVMDATA.xml", trainingData, labels);
	
	학습데이터 생성*/

/*	Mat trainingData, labels;
	read_trainData("../ImageExam/SVMDATA.xml", trainingData, labels);

	Ptr<ml::SVM> svm = SVM_create(CV_TERMCRIT_ITER, 1000, 0);
	svm->train(trainingData, ml::ROW_SAMPLE, labels);
	svm->save("../SVMtrain.xml");
	
	슈퍼학습*/