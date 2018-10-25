//#include "Utility.h";
//#include <iostream>
//#include <opencv2\opencv.hpp>
//using namespace std;
//using namespace cv;
//
//
//int main() {
//	const char* inputPath = "face1.bmp";
//
//	unsigned char* pixels = ReadBMP(inputPath);
//
//	FILE* f = fopen(inputPath, "rb");
//	if (f == NULL)
//		throw "Argument Exception";
//
//	unsigned char info[54];
//	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
//
//											   // extract image height and width from header
//	int nWidth_in = *(int*)&info[18];
//	int nHeight_in = *(int*)&info[22];
//	fclose(f);
//
//	unsigned char** ch_in_R = allocMem(nHeight_in, nWidth_in, 0); 
//	unsigned char** ch_in_G = allocMem(nHeight_in, nWidth_in, 0); 
//	unsigned char** ch_in_B = allocMem(nHeight_in, nWidth_in, 0); 
//
//	for (int i = 0; i < nHeight_in; i++) {
//		for (int j = 0; j < nWidth_in * 3; j += 3) {
//			ch_in_R[i][j / 3] = pixels[j];
//			ch_in_G[i][j / 3] = pixels[j+1];
//			ch_in_B[i][j / 3] = pixels[j+2];
//		}
//	}
//
//	Mat result = Mat(nHeight_in, nWidth_in, CV_8UC3);
//	for (int h = 0; h < nHeight_in; h++) {
//		for (int w = 0; w < nWidth_in; w++) {
//			result.at<Vec3b>(h, w) = Vec3b(ch_in_B[h][w], ch_in_G[h][w], ch_in_R[h][w]);
//		}
//	}
//
//	imshow("result", result);
//	waitKey(0);
//	return 0;
//}