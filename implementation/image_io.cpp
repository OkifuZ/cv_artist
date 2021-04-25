#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "../include/image_io.h"
#include "../include/auxiliary.h"
#include "../include/frequency.h"

MyImageMatrix::MyImageMatrix(std::vector<int> shape_) {
	this->valid = false;
	if (shape_.size() < 2) {
		return;
	}
	this->shape = shape_;
	if (shape.size() == 2) {
		shape.push_back(1);
	}
	int channels = shape[2];
	this->mat = std::vector<std::vector<my_pixel>>(shape[0], 
		std::vector<my_pixel>(shape[1], my_pixel(std::vector<uchar>(channels))));
	this->valid = true;
}

MyImageMatrix::MyImageMatrix(const ComplexMat& ano) {
	this->valid = true;
	this->shape = std::vector<int>{ ano.n, ano.m, 1 };
	int channels = 1;
	this->mat = std::vector<std::vector<my_pixel>>(ano.n, std::vector<my_pixel>(ano.m));
	for (int i = 0; i < ano.n; i++) {
		for (int j = 0; j < ano.m; j++) {
			this->mat[i][j] = my_pixel( floor(ano.mat[i][j].real) );
		}
	}
}

void MyImageMatrix::get_histogram_vec(std::vector<float>& hist_vec, int which_channel) {
	if (this->distribution_table[which_channel].empty()) {
		get_pixel_value_distribution(*this, which_channel);
	}
	hist_vec = this->distribution_table[which_channel];
}

int read_img(std::string img_path, MyImageMatrix& img_mat, const std::string &type) {
	cv::Mat image;
	if (type == "RGB") {
		image = cv::imread(PIC_BASE + img_path, cv::IMREAD_COLOR);
	}
	else if (type == "GREY") {
		image = cv::imread(PIC_BASE + img_path, cv::IMREAD_GRAYSCALE);
	}
	else {
		std::cout << type + "? TYPE error! -> no such type" << std::endl;
		return -1;
	}
	if (image.empty()) {
		std::cout << img_path + " can not be loaded" << std::endl;
		return -1;
	}
	int channels = image.channels();
	img_mat.mat = std::vector<std::vector<my_pixel>>(image.rows,
		std::vector<my_pixel>(image.cols, my_pixel(std::vector<uchar>(channels))));
	if (channels == 3) {
		for (int i = 0; i < image.rows; i++) {
			cv::Vec3b* row = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < image.cols; j++) {
				cv::Vec3b& ch = row[j];
				for (int k = 0; k < channels; k++) {
					img_mat.mat[i][j].rgb[k] = ch[k];
				}
			}
		}
	}
	else if (channels == 1) {
		for (int i = 0; i < image.rows; i++) {
			uchar* row = image.ptr<uchar>(i);
			for (int j = 0; j < image.cols; j++) {
				img_mat.mat[i][j].rgb[0] = row[j];
			}
		}
	}
	img_mat.shape = std::vector<int>{ image.rows, image.cols, image.channels() };
	img_mat.valid = true;
	return 0;
}


int write_img(std::string img_path, const MyImageMatrix& img_mat, bool show) {
	if (!img_mat.valid) {
		std::cout << "can not write an unvalid image to " + img_path << std::endl;
		return -1;
	}
	int rows = img_mat.shape[0];
	int cols = img_mat.shape[1];
	int channels = img_mat.shape[2];
	std::cout << channels << std::endl;
	cv::Mat image(rows, cols, CV_8UC(channels), cv::Scalar::all(128));
	if (channels == 3) {
		for (int i = 0; i < rows; i++) {
			cv::Vec3b* row = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < cols; j++) {
				cv::Vec3b& ch = row[j];
				for (int k = 0; k < channels; k++) {
					// rgb to bgr
					ch[k] = img_mat.mat[i][j].rgb[k];
				}
			}
		}
	}
	else if (channels == 1) {
		for (int i = 0; i < rows; i++) {
			uchar* row = image.ptr<uchar>(i);
			for (int j = 0; j < cols; j++) {
				row[j] = img_mat.mat[i][j].rgb[0];
			}
		}
	}
	else {
		std::cout << "channel error! Write image failed" << std::endl;
	}
	if (show) {
		cv::imshow("processed image", image);
	}
	if (img_path != "") {
		cv::imwrite(PIC_BASE + img_path, image);
	}
	return 0;
}

