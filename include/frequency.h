#pragma once
#include<opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <iostream>

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

void printComplex(cValue& c, std::ostream& out);


class ComplexMat {

public:

	int n = 0;
	int m = 0;
	cValue** mat = nullptr;

	ComplexMat() = default;

	ComplexMat(int n_, int m_) { // n rows m colms
		mat = new cValue*[n_];
		for (int i = 0; i < n_; i++) {
			mat[i] = new cValue[m_];
		}
		n = n_;
		m = m_;
	}

	ComplexMat(const ComplexMat& ano);

	void clear() {
		if (mat) {
			for (int i = 0; i < n; i++) {
				delete[] mat[i];
			}
			delete[] mat;
		}
		n = 0;
		m = 0;
	}

	void reshape(int n_, int m_) {
		clear();
		mat = new cValue * [n_];
		for (int i = 0; i < n_; i++) {
			mat[i] = new cValue[m_];
		}
		n = n_;
		m = m_;
	}

	ComplexMat& operator=(const ComplexMat& mat);

	~ComplexMat() {
		clear();
	}

};

ComplexMat* get_transpose(ComplexMat& mat);

ComplexMat* get_pixel_trans(ComplexMat& mat, bool use_log);

ComplexMat* get_abs(ComplexMat& mat);

ComplexMat* get_centralized(ComplexMat& mat);

ComplexMat* get_divided(ComplexMat& mat, float n);


void FFT_array(cValue* arr, int length, bool inverse);

ComplexMat* FFT_mat(const ComplexMat& ori);

ComplexMat* IFFT_mat(const ComplexMat& ori);

ComplexMat* DCT_mat(const ComplexMat& ori);