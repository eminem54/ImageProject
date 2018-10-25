#pragma once
#include <iostream>
using namespace std;

unsigned char** allocMem(int height, int width, int val) {
	unsigned char** ret = new unsigned char*[height];
	for (int i = 0; i < height; i++) {
		ret[i] = new unsigned char[width];
		//memset(ret[i], val, sizeof(unsigned char)*width);
	}
	return ret;
}

unsigned char* ReadBMP(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			//cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << endl;
		}
	}
	fclose(f);
	return data;
}


unsigned char** padding(unsigned char** in, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = allocMem(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = in[h][w];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = in[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = in[nHeight - 1][w];
		}
	}
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = in[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = in[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = in[0][0];
			Pad[h + (nHeight - 1)][w] = in[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = in[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = in[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}
