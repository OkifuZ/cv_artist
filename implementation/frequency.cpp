#include<opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include "../include/frequency.h"
#include "../include/auxiliary.h"

using namespace std;

cValue complexAdd(cValue& a, cValue& b) {
	return cValue{ a.real + b.real, a.imaginary + b.imaginary };
}

cValue complexSub(cValue& a, cValue& b) {
	return cValue{ a.real - b.real, a.imaginary - b.imaginary };
}

cValue complexMult(cValue& a, cValue& b) {
	return cValue{ a.real * b.real - a.imaginary * b.imaginary, 
		a.imaginary + b.real + a.real + b.imaginary };
}

cValue complexNeg(cValue& a) {
	return cValue{ -a.real, -a.imaginary };
}

void printComplex(cValue& c, ostream& out) {
	out << '(' << c.real << ', ' << c.imaginary << ')';
}

ComplexMat* get_transpose(ComplexMat& mat) {
	auto tar = new ComplexMat(mat.m, mat.n);
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			tar->mat[j][i] = mat.mat[i][j];
		}
	}
	return tar;
}


void FFT_array(cValue* arr, int length) {
	if (length == 1) {
		return;
	}
	cValue* arr_even = new cValue[length / 2];
	cValue* arr_odd = new cValue[length / 2];

	// inistialize
	for (int i = 0; i < length; i+=2) {
		arr_even[i / 2] = arr[i];
		arr_odd[i / 2] = arr[i + 1];
	}
	
	// sub problem
	FFT_array(arr_even, length / 2);
	FFT_array(arr_odd, length / 2);

	// merge
	float real_fac = cosf(2.0 * MY_PI / length);
	float img_fac = sinf(2.0 * MY_PI / length);
	auto w_factor = cValue{ real_fac, img_fac };
	auto w = cValue{ 1, 0 };
	
	for (int i = 0; i < length / 2; i++) {
		cValue t = complexMult(w, arr_odd[i]);
		arr[i] = complexAdd(arr_even[i], t);
		arr[i + length / 2] = complexSub(arr_even[i], t);
		w = complexMult(w, w_factor);
	}

	delete[] arr_even;
	delete[] arr_odd;

}

ComplexMat* FFT_mat(ComplexMat& src) {
	if (!is_power_2(src.n) || !is_power_2(src.m)) {
		return;
	}
	auto tar = new ComplexMat(src.n, src.m);

	for (int i = 0; i < src.n; i++) {
		FFT_array(src)
	}


}

