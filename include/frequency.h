#pragma once
#include<opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>

#include "image_io.h"
#include "auxiliary.h"

struct cValue {
	float real = 0.0;
	float imaginary = 0.0;
};

cValue complexAdd(cValue& a, cValue& b);

cValue complexSub(cValue& a, cValue& b);

cValue complexMult(cValue& a, cValue& b);

cValue complexNeg(cValue& a);


class ComplexMat {

public:

	int n = 0;
	int m = 0;
	cValue** mat;

	ComplexMat(int n, int m) { // n rows m colms
		mat = new cValue*[n];
		for (int i = 0; i < n; i++) {
			mat[i] = new cValue[m];
		}
	}

	~ComplexMat() {
		for (int  i = 0; i < n; i++) {
			delete[] mat[i];
		}
		delete[] mat;
	}

};

void FFT_array(cValue* arr);