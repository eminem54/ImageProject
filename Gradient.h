#pragma once

void makeGradientHistogram(unsigned char** gradient, unsigned char** cells, int cellNum, int cellSize);
void calculateGradient(unsigned char** pad, unsigned char** gradient);
