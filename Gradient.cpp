#define _USE_MATH_DEFINES
#include <math.h>

#define SIZE 256

void makeGradientHistogram(unsigned char** gradient, unsigned char** cells, int cellNum, int cellSize) {
	for (int h = 0; h < cellNum; h++) {
		for (int w = 0; w < cellNum; w++) {
			for (int i = 0; i < cellSize; i++) {
				for (int j = 0; j < cellSize; j++) {
					cells[h*cellNum + w][gradient[h*cellSize + i][w*cellSize + j]]++;
				}
			}
		}
	}
}


void calculateGradient(unsigned char** pad, unsigned char** gradient) {
	double dx, dy;
	int angle;
	for (int h = 0; h < SIZE; h++) {
		for (int w = 0; w < SIZE; w++) {
			dx = 0.0; dy = 0.0;
			if (h > 0 && h < SIZE - 1 && w>0 && w < SIZE - 1) {
				dx = pad[h][w + 1] - pad[h][w - 1];
				dy = pad[h - 1][w] - pad[h + 1][w];
				if (dx != 0) {
					angle = (atan((double)dy / dx) * 180.0 / M_PI);
					if (angle < 0) {
						gradient[h][w] = (180 + angle) / 20.0;
					}
					else
						gradient[h][w] = angle / 20.0;
				}
				else {
					dx = 0.00001;
					angle = (atan((double)dy / dx) * 180.0 / M_PI);
					if (angle < 0) {
						gradient[h][w] = (180 + angle) / 20.0;
					}
					else
						gradient[h][w] = angle / 20.0;
				}
			}
			else {
				gradient[h][w] = 4;
			}
		}
	}
}