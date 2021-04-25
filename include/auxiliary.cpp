#include <vector>
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/auxiliary.h"
#include "../include/frequency.h"

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
	while (n != 1) {
		if (n & 1) return false;
		n = n >> 1;
	}
	return true;
}

void debug_complexMat(const ComplexMat& cmat) {
	float max_v_r;
	float min_v_r;
	float max_v_i;
	float min_v_i;

	for (int i = 0; i < cmat.n; i++) {
		for (int j = 0; j < cmat.m; j++) {
			float r = cmat.mat[i][j].real;
			float imag = cmat.mat[i][j].imaginary;
			if (i == 0 && j == 0) {
				max_v_i = min_v_i = imag;
				max_v_r = min_v_r = imag;
			}
			else {
				max_v_r = max_v_r > r ? max_v_r : r;
				max_v_i = max_v_i > imag ? max_v_i : imag;
				min_v_r = min_v_r < r ? min_v_r : r;
				min_v_i = min_v_i < imag ? min_v_i : imag;
			}
		}
	}
	cout << "**********************\n";
	cout << "max real: " << max_v_r << endl;
	cout << "min real: " << min_v_r << endl;
	cout << "max imag: " << max_v_i << endl;
	cout << "min imag: " << min_v_i << endl;
}

string get_path_name(const string& path_name_) {
	string path_name = path_name_;
	reverse(path_name.begin(), path_name.end());
	string file_name;
	for (char c : path_name) {
		if (c == '\\' || c == '/') break;
		file_name.push_back(c);
	}
	reverse(file_name.begin(), file_name.end());

	if (file_name.find('.') == string::npos) {
		return file_name;
	}

	string new_name;
	for (char c : file_name) {
		if (c == '.') break;
		new_name.push_back(c);
	}

	return new_name;
}

void arg_error(string s) {
	cout << "Parameter Error ! -> " << s << endl;
	exit(1);
}

bool all_number(string s) {
	for (auto& c : s) {
		if (c > '9' || c < '0') return false;
	}
	return true;
}



void get_info(const vector<vector<string>>& args, ControlBlock& con_blc) {
	for (auto& cluster : args) {
		if (cluster[0] == "-f") {
			con_blc.pic_path = cluster[1];
		}
		else if (cluster[0] == "--mode") {
			con_blc.mode = cluster[1];
		}
		else if (cluster[0] == "--fft") {
			con_blc.fft = true;
		}
		else if (cluster[0] == "--he") {
			con_blc.he = true;
		}
		else if (cluster[0] == "--cs") {
			con_blc.cs = true;
			con_blc.a = stoi(cluster[1]);
			con_blc.b = stoi(cluster[2]);
			con_blc.c = stoi(cluster[3]);
			con_blc.d = stoi(cluster[4]);
		}
		else if (cluster[0] == "--dct") {
			con_blc.dct = true;
		}
		else if (cluster[0] == "--silent") {
			con_blc.silence = true;
		}
		else if (cluster[0] == "--save") {
			con_blc.save_path = cluster[1];
		}
	}
}
