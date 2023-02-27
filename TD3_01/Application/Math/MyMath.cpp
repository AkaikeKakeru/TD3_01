#include "MyMath.h"

float ConvertToRadian(float degree) {
	const float PI = 3.14159265f;

	float radian = 0;
	radian = degree * (PI / 180);

	return radian;
}

float AbsoluteValue(float f) {
	f = (f * f) / f;
	return f;
}

float SquareRoot(float f) {
	const float EPSILON = 0.00001f;

	float x = 1, y = 0;

	//ニュートン法を用いる
	while (true) {
		x = x - (x * x - f) / (2 * x);
		y = x * x - f;
		if ((y <= EPSILON) && (y >= -EPSILON)){
			break;
		}
	}

	return x;
}
