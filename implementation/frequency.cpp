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
		a.imaginary * b.real + a.real * b.imaginary };
}

cValue complexNeg(cValue& a) {
	return cValue{ -a.real, -a.imaginary };
}

void printComplex(cValue& c, ostream& out) {
	out << '(' << c.real << ", " << c.imaginary << ')';
}

ComplexMat& ComplexMat::operator=(const ComplexMat& ano) {
	this->clear();
	this->mat = new cValue*[ano.n];
	for (int i = 0; i < n; i++) {
		mat[i] = new cValue[ano.m];
	}
	this->n = ano.n;
	this->m = ano.m;
	return *this;
}

ComplexMat::ComplexMat(const ComplexMat& ano) {
	this->n = ano.n;
	this->m = ano.m;
	this->mat = new cValue*[ano.n];
	for (int i = 0; i < n; i++) {
		mat[i] = new cValue[ano.m];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mat[i][j] = ano.mat[i][j];
		}
	}
	
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

ComplexMat* get_pixel_trans(ComplexMat& mat, bool use_log) {
	auto tar = new ComplexMat(mat.m, mat.n);
	float max_v = 0.0;
	float min_v = 1000.0;
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			float im = mat.mat[i][j].imaginary;
			float r = mat.mat[i][j].real;
			float v = use_log ? log(im * im + r * r + 1.0) : im * im + r * r;
			max_v = max_v > v ? max_v : v;
			min_v = min_v < v ? min_v : v;
			tar->mat[i][j] = {v, 0};
		}
	}
	float inter_len = max_v - min_v;
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			float pixel_v = (tar->mat[i][j].real - min_v) * 256.0 / inter_len;
			tar->mat[i][j] = { pixel_v, 0 };
		}
	}
	return tar;
}

ComplexMat* get_abs(ComplexMat& mat) {
	auto tar = new ComplexMat(mat.m, mat.n);
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			float im = mat.mat[i][j].imaginary;
			float r = mat.mat[i][j].real;
			float v = sqrt(im * im + r * r);
			tar->mat[i][j] = { v, 0.0 };
		}
	}
	return tar;
}


ComplexMat* get_centralized(ComplexMat& mat) {
	auto tar = new ComplexMat(mat.m, mat.n);
	
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			tar->mat[i][j] = (i + j) & 1 ? complexNeg(mat.mat[i][j]) : mat.mat[i][j];
		}
	}
	return tar;
}

ComplexMat* get_divided(ComplexMat& mat, float n) {
	auto tar = new ComplexMat(mat.m, mat.n);

	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			tar->mat[i][j] = { tar->mat[i][j].imaginary / n , tar->mat[i][j].real / n};
		}
	}
	return tar;
}

void FFT_array(cValue* arr, int length, bool inverse) {
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
	FFT_array(arr_even, length / 2, inverse);
	FFT_array(arr_odd, length / 2, inverse);

	// merge
	float real_fac = cosf(2.0 * MY_PI / length);
	float img_fac = inverse ? -sinf(2.0 * MY_PI / length) : 
		sinf(2.0 * MY_PI / length);
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

ComplexMat* FFT_mat(const ComplexMat& src_) {
	if (!is_power_2(src_.n) || !is_power_2(src_.m)) {
		return nullptr;
	}

	ComplexMat src = src_;

	for (int i = 0; i < src.n; i++) {
		FFT_array(src.mat[i], src.m, false);
	}
	auto trans_mat = get_transpose(src);
	for (int i = 0; i < trans_mat->n; i++) {
		FFT_array(trans_mat->mat[i], trans_mat->m, false);
	}

	auto res = get_transpose(*trans_mat);
	delete trans_mat;
	return res;

}

ComplexMat* IFFT_mat(const ComplexMat& src_) {
	if (!is_power_2(src_.n) || !is_power_2(src_.m)) {
		return nullptr;
	}

	ComplexMat src = src_;

	for (int i = 0; i < src.n; i++) {
		FFT_array(src.mat[i], src.m, true);
		for (int j = 0; j < src.m; j++) {
			src.mat[i][j] = {
				src.mat[i][j].real / src.m,
				src.mat[i][j].imaginary / src.m };
		}
	}
	auto trans_mat = get_transpose(src);
	for (int i = 0; i < trans_mat->n; i++) {
		FFT_array(trans_mat->mat[i], trans_mat->m, true);
		for (int j = 0; j < trans_mat->m; j++) {
			trans_mat->mat[i][j] = { 
				trans_mat->mat[i][j].real / trans_mat->m, 
				trans_mat->mat[i][j].imaginary / trans_mat->m };
		}
	}

	auto res = get_transpose(*trans_mat);

	/*auto res_div = get_divided(*res, src_.m * src_.n);*/

	//delete res;
	delete trans_mat;

	return res;

}


ComplexMat* DCT_mat(const ComplexMat& src_) {
	if (!is_power_2(src_.n) || !is_power_2(src_.m)) {
		return nullptr;
	}

	ComplexMat src = src_;
	cValue* padded = new cValue[src.m * 2]();
	for (int i = 0; i < src.n; i++) {
		for (int j = 0; j < src.m*2; j++) {
			padded[j] = j < src.m ? src.mat[i][j] : cValue();
		}
		FFT_array(padded, src.m*2, false);
		
		float fac = sqrt(2.0 / src.m);
		for (int j = 0; j < src.m; j++) {
			src.mat[i][j] = { padded[j].real * fac , 0};
		}
		src.mat[i][0] = { float(src.mat[i][0].real / sqrt(2)), 0.0 };
	}

	
	auto trans_mat = get_transpose(src);
	
	padded = new cValue[trans_mat->m * 2];
	for (int i = 0; i < trans_mat->n; i++) {
		for (int j = 0; j < trans_mat->m * 2; j++) {
			padded[j] = j < trans_mat->m ? trans_mat->mat[i][j] : cValue();
		}
		FFT_array(padded, trans_mat->m*2, false);
		float fac = sqrt(2.0 / trans_mat->m);
		for (int j = 0; j < trans_mat->m; j++) {
			trans_mat->mat[i][j] = { padded[j].real * fac , 0 };
		}
		trans_mat->mat[i][0] = { float(trans_mat->mat[i][0].real / sqrt(2)), 0.0 };
	}

	delete[] padded;

	auto res = get_transpose(*trans_mat);
	delete trans_mat;
	return res;

}