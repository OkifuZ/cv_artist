#include <string>
#include <iostream>
#include <vector>
#include "../include/auxiliary.h"
#include "../include/image_io.h"
#include "../include/image_proc.h"
#include "../include/frequency.h"


int get_pixel_value_distribution(MyImageMatrix& src_img, int which_channel) {
	if (!src_img.valid) {
		std::cout << "can not count greyscale distribution on an empty image!" << std::endl;
		return -1;
	}
	if (which_channel >= src_img.shape[2] || which_channel < 0) {
		std::cout << "invalid target channel : " << which_channel << std::endl;
		return -1;
	}
	if (src_img.distribution_table[which_channel].empty()) {
		int rows = src_img.shape[0];
		int cols = src_img.shape[1];
		std::vector<int> count_vec(256, 0);
		int total_pixel = src_img.shape[0] * src_img.shape[1];
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				count_vec[src_img.mat[i][j].rgb[which_channel]] ++;
			}
		}
		std::vector<float> distribution_vec(256, 0.0);
		for (int i = 0; i < 256; i++) {
			distribution_vec[i] = 1.0 * count_vec[i] / total_pixel;
		}
		src_img.distribution_table[which_channel] = distribution_vec;
	}
	
	return 0;
}

int histogram_equalization(MyImageMatrix& src_img, MyImageMatrix& tar_img) {
	if (!src_img.valid) {
		std::cout << "can not perform histogram equalization on an empty image!" << std::endl;
		return -1;
	}
	int channels = src_img.shape[2];
	for (int k = 0; k < channels; k++) {
		if (src_img.hist_equ_table[k].empty()) {
			src_img.hist_equ_table[k] = std::vector<int>(256, 0);
			if (src_img.distribution_table[k].empty()) {
				get_pixel_value_distribution(src_img, k);
			}
			std::vector<float> distribution(src_img.distribution_table[k].begin(), src_img.distribution_table[k].end());
			for (int i = 1; i < 256; i++) {
				distribution[i] += distribution[i - 1];
			}
			for (int i = 0; i < 256; i++) {
				src_img.hist_equ_table[k][i] = distribution[i] * 256;
			}
		}
	}
	int rows = src_img.shape[0];
	int cols = src_img.shape[1];
	tar_img.shape = src_img.shape;
	tar_img.mat = std::vector<std::vector<my_pixel>>(rows, std::vector<my_pixel>(cols));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < channels; k++) {
				int src_val = src_img.mat[i][j].rgb[k];
				tar_img.mat[i][j].rgb[k] = src_img.hist_equ_table[k][src_val];
			}
		}
	}
	tar_img.valid = true;
	return 0;
}

int contrast_strech_type1(MyImageMatrix& src_img, MyImageMatrix& tar_img,
	int from_low, int from_high, int to_low, int to_high, int mode) {
	if (!src_img.valid) {
		std::cout << "can not constrast strech an empty image!" << std::endl;
		return -1;
	}
	from_low = std::max(0, from_low);
	to_low = std::max(0, to_low);
	from_high = std::min(255, from_high);
	to_high = std::min(255, to_high);
	if (from_high < from_low) {
		std::cout << "invalid mapping interval in contrast strech!" << std::endl;
		return -1;
	}
	float slope = from_high - from_low == 0 ? 0 : 1.0 * (to_high - to_low) / (from_high - from_low);
	float intercept = to_high - slope * from_high;
	contrast_strech_type2(src_img, tar_img, slope, intercept, from_low, from_high);
	return 0;
}

int contrast_strech_type2(MyImageMatrix& src_img, MyImageMatrix& tar_img,
	float slope, float intercept, int from_low, int from_high, int mode) {
	if (!src_img.valid) {
		std::cout << "can not constrast strech an empty image!" << std::endl;
		return -1;
	}
	from_low = std::max(0, from_low);
	from_high = std::min(255, from_high);
	if (from_high < from_low) {
		std::cout << "invalid mapping interval in contrast strech!" << std::endl;
		return -1;
	}
	std::vector<float> map_table(256, 0);
	float y_coord_a = slope * from_low + intercept;
	float y_coord_b = slope * from_high + intercept;
	float slope_1 = from_low == 0 ? 0 : y_coord_a / from_low;
	float slope_2 = slope;
	float slope_3 = from_high == 255 ? 0 : (255 - y_coord_b) / (255 - from_high);
	for (int i = 0; i < 256; i++) {
		if (i < from_low) {
			map_table[i] = mode == 0 ? slope_1 * i : y_coord_a;
		}
		else if (i >= from_low && i < from_high) {
			map_table[i] = slope_2 * i + intercept;
		}
		else {
			map_table[i] = mode == 0 ? slope_3 * (i - from_high) + y_coord_b : y_coord_b;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (map_table[i] < 0) map_table[i] = 0;
		if (map_table[i] > 255.9) map_table[i] = 255.9;
	}
	int rows = src_img.shape[0];
	int cols = src_img.shape[1];
	int channels = src_img.channels();
	tar_img.shape = src_img.shape;
	tar_img.mat = std::vector<std::vector<my_pixel>>(rows, std::vector<my_pixel>(cols, my_pixel()));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < channels; k++) {
				int src_val = src_img.mat[i][j].rgb[k];
				tar_img.mat[i][j].rgb[k] = map_table[ceil(src_val)];
			}
		}
	}
	tar_img.valid = true;
	return 0;
}

ComplexMat* FFT_image(MyImageMatrix& src_img) {
	int n = src_img.shape[0];
	int m = src_img.shape[1];
	auto src_mat = ComplexMat(n, m);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			src_mat.mat[i][j] = { (float)(src_img.mat[i][j].rgb[0]), 0 };
		}
	}
	
	auto src_mat_center = get_centralized(src_mat);

	auto res = FFT_mat(*src_mat_center);
	return res;
}

ComplexMat* DCT_image(MyImageMatrix& src_img) {
	int n = src_img.shape[0];
	int m = src_img.shape[1];
	auto src_mat = ComplexMat(n, m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			src_mat.mat[i][j] = { (float)(src_img.mat[i][j].rgb[0]), 0 };
		}
	}

	auto res = DCT_mat(src_mat);
	return res;
}