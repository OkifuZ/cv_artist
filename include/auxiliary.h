#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <vector>


class ComplexMat;

#define PIC_BASE ""
#define MY_PI   3.141592653589

int joint_histograms(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist);
int joint_histograms_type2(std::vector<cv::Mat>& hist_vec, int col, int row, cv::Mat& res_hist);

int add_title(const std::string& title, cv::Mat& src, cv::Mat& dst_);

bool is_power_2(int n);

void debug_complexMat(const ComplexMat& cmat);

std::string get_path_name(const std::string& path_name);

void arg_error(std::string s);

bool all_number(std::string s);

struct ControlBlock {
	std::string pic_path;
	std::string mode;
	bool fft = false;
	bool dct = false;
	bool he = false;
	bool cs = true;
	int a, b, c, d;
	bool silence = false;
	std::string save_path;
};

void get_info(const std::vector<std::vector<std::string>>& args, ControlBlock& con_blc);