#include <vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/auxiliary.h"

using namespace std;
using namespace cv;

int joint_histograms(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist) {
	if (hist_vec.size() != col * row) {
		exit(1);
	}
	std::vector<cv::Mat> row_hists;
	vector<string> titles{ "before", "after" };
	for (int i = 0; i < hist_vec.size(); i += row) {
		Mat col_joint;
		auto b = hist_vec.begin() + i;
		auto e = hist_vec.begin() + i + row;
		std::vector<cv::Mat> col_hists(b, e);
		vconcat(col_hists, col_joint);
		add_title(titles[i / row], col_joint, col_joint);
		row_hists.push_back(col_joint);
	}
	Mat res;
	hconcat(row_hists, res);
	res_hist = res.clone();
	return 0;
}

int joint_histograms_type2(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist) {
	if (hist_vec.size() != col * row) {
		exit(1);
	}
	std::vector<cv::Mat> col_hists;
	vector<string> titles{ "before", "after" };
	for (int i = 0; i < hist_vec.size(); i += col) {
		Mat row_joint;
		auto b = hist_vec.begin() + i;
		auto e = hist_vec.begin() + i + col;
		std::vector<cv::Mat> row_hists(b, e);
		hconcat(row_hists, row_joint);
		add_title(titles[i / col], row_joint, row_joint);
		col_hists.push_back(row_joint);
	}
	Mat res;
	vconcat(col_hists, res);
	res_hist = res.clone();
	return 0;
}

int add_title(const std::string& title, cv::Mat& src, cv::Mat& dst_) {
	int col_title = src.cols / 2 - 30;
	int row_title = 30;
	Mat dst;
	copyMakeBorder(src, dst, 40, 0, 0, 0, cv::BORDER_CONSTANT, Scalar::all(0));
	cv::putText(dst, title, cv::Point2i(col_title, row_title), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar::all(255), 0.4, cv::LINE_AA);
	dst_ = dst.clone();
	return 0;
}

bool is_power_2(int n) {
	while (n) {
		if (n & 1) return false;
		n = n >> 1;
	}
	return true;
}