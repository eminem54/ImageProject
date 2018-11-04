
/*
face2  2 2 0 2 1 1      1 1 1 0 0 0 3 0 3 3 3 1 0   3 1 0 2 3 3 0 2 3 2 3 0 0 3 0 2 3 0 0 0
face5  2 0 0 3 1        0 1 3 3 0 0 0 3 3           3 0 2 3 1    1 1 0 3 0 0 3 1 3 3
face6  2 2 2 1 2        1 0 2 1 0 3 2               2 2 0 1 1    1 0 3 0 0 0 3 1 1 1 1 0 0 0 0 0 0 
face7  2 0 1 2 0        3 3 0 0 3 3 3               2 1 2 3 0    1 2 0 0 0 0 3 0 0 3 1 1 1 1 1 1 1 - 1 1
face8  0 1 1 - 3  0 0   0 2 1 1 1 1 2 2 2 0         3 3 3 0 0   1 1    3 3 3 3 3 3 2 2 2 
face9  1 2 2 1 1        1 0 3 1 0                   2 2 1 1 1 1 1 1 0 0 1 3 0 1 0 0 1
face10 0 2 1 3 1        3 3 2 0 3 3 2               3 1 0 2 2 1 2 3 0 0 3 3 0 0 0 0 3 - 1 3 0 0 1
face12 3 3 3 3 3        3 0 1 0 3 0 - - 1           1 1 1 1 1 1 0 3 0 0 0 0 0 1 3 3 0 0 

*/

bool isEye(int num0, int num1, int num2, int num3) {
	if (num1 > 1 && num2 > 1) return true;
	if (num0 == 2 && ((num1==1 && num3==1) || (num2==2))) return true;
	if (num1 >1 && ((num3==1&&num2==1) || (num3==1&&num0==1))) return true;
	if (num2 > 3)return true;
	return false;
}

bool isSecondEye(int num0, int num1, int num2, int num3) {
	return false;
}

void countCellValue(unsigned char value, int& num0, int& num1, int& num2, int& num3){
	if ((int)value == 0) {
		num0++;
	}
	else if ((int)value == 1) {
		num1++;
	}
	else if ((int)value == 2) {
		num2++;
	}
	else if ((int)value == 3) {
		num3++;
	}
}