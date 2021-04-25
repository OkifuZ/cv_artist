#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>

class ComplexMat;


struct my_pixel {
	std::vector<uchar> rgb{ 0, 0, 0 };
	int type = 0; // 0 rgb, 1 grey

	my_pixel() = default;
	my_pixel(uchar color) {
		rgb[0] = color;
	}
	my_pixel(std::vector<uchar>&& color) {
		if (color.size() == 1) {
			rgb[0] = color[0];
			type = 1;
		}
		else if (color.size() == 3) {
			rgb[0] = color[0];
			rgb[1] = color[1];
			rgb[2] = color[2];
			type = 0;
		}
	}
	
};


class MyImageMatrix {
	friend int read_img(std::string img_path, MyImageMatrix& img_mat, const std::string& type);
	friend int histogram_equalization(MyImageMatrix& src_img, MyImageMatrix& tar_img);
	friend int get_pixel_value_distribution(MyImageMatrix& src_img, int which_channel);

public:
	MyImageMatrix() = default;
	MyImageMatrix(std::vector<int> shape);
	MyImageMatrix(const ComplexMat& cmat);
	
	bool is_grey_img() const { return valid ? shape[2] == 1 : false; }
	int channels() const { return valid ? shape[2] : -1; }
	void get_histogram_vec(std::vector<float> &hist_vec, int which_channel);

	std::vector<int> shape{};
	std::vector<std::vector<my_pixel>> mat;
	bool valid = false;

private:
	std::vector<std::vector<int>> hist_equ_table = std::vector<std::vector<int>>(3);
	std::vector<std::vector<float>> distribution_table = std::vector<std::vector<float>>(3);


};


int read_img(std::string img_path, MyImageMatrix& img_mat, const std::string& type);

int write_img(std::string img_path, const MyImageMatrix& img_mat, bool show);
