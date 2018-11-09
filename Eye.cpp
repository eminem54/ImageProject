
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