#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <vector>

#define PIC_BASE ""
#define MY_PI   3.141592653589

int joint_histograms(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist);
int joint_histograms_type2(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist);

int add_title(const std::string& title, cv::Mat& src, cv::Mat& dst_);

bool is_power_2(int n) {
	while (n) {
		if (n & 1) return false;
		n = n >> 1;
	}
	return true;
}