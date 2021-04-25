#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include "../include/histogram.h"
#include "../include/auxiliary.h"


int normalize_histogram(MyHistogram& histogram, int low, int high) {
	if (histogram.is_empty()) {
		std::cout << "can not normalize an empty histogram : " + histogram.description 
			<< std::endl;
		return -1;
	}
	double largest_value = 0;
	for (auto it = histogram.values.begin(); it != histogram.values.end(); it++) {
		largest_value = largest_value > *it ? largest_value : *it;
	}
	int expand_size = high - low;
	for (auto it = histogram.values.begin(); it != histogram.values.end(); it++) {
		*it = (*it / largest_value) * expand_size + low;
	}
	return 0;
}

int draw_histogram(const MyHistogram& histogram, 
	cv::Mat &hist_res,
	std::string display_name,
	const std::string& color, 
	int max_height, int interval,
	std::string save_path) {
	if (histogram.is_empty()) {
		std::cout << "can not draw an empty histogram : " + histogram.description 
			<< std::endl;
		return -1;
	}
	cv::Scalar hist_color;
	if (color == "white") hist_color = { 220, 220, 220 };
	else if (color == "red") hist_color = { 0, 0, 180 };
	else if (color == "green") hist_color = { 0, 180, 0 };
	else if (color == "blue") hist_color = { 180, 0, 0 };
	else hist_color = { 220, 220, 220 };
	MyHistogram copy_hist = histogram;
	int hisHeight = max_height, hisWidth = copy_hist.size() * (interval+2);
	normalize_histogram(copy_hist, 0, hisHeight - 20);
	cv::Mat hist_opencv(hisHeight, hisWidth, CV_8UC3, cv::Scalar::all(0));
	int rows_before = hist_opencv.rows;
	int line_num = 5;
	for (int i = 1; i < line_num; i++) {
		int row_line = rows_before - (rows_before / 5) * i;
		cv::line(hist_opencv, cv::Point(0, row_line), cv::Point(hist_opencv.cols, row_line), cv::Scalar::all(64), 3);
	}
	for (int i = 0; i < copy_hist.size(); i++) {
		int val = copy_hist.values[i];
		cv::rectangle(hist_opencv, 
			cv::Point(i * (interval + 2), hist_opencv.rows),
			cv::Point(i * (interval + 2) + 3, hist_opencv.rows - val), cv::Scalar(hist_color), cv::FILLED);
	}

	// post process
	cv::copyMakeBorder(hist_opencv, hist_opencv, 0, 0, 20, 20, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::copyMakeBorder(hist_opencv, hist_opencv, 0, 2, 2, 0, cv::BORDER_CONSTANT, cv::Scalar::all(256));
	cv::copyMakeBorder(hist_opencv, hist_opencv, 30, 30, 50, 20, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	int max_row = hist_opencv.rows;
	int max_col = hist_opencv.cols;
	int num1_row = max_row - 15;
	int num1_col = 70;
	int num2_row = num1_row;
	int num2_col = max_col - 50;
	int num3_col = (-num1_col + num2_col) / 4 + num1_col;
	int num4_col = (-num1_col + num2_col) / 2 + num1_col;
	int num5_col = (-num1_col + num2_col) / 4 * 3 + num1_col;
	cv::putText(hist_opencv, "0", cv::Point2i(num1_col, num1_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	cv::putText(hist_opencv, "255", cv::Point2i(num2_col, num2_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	cv::putText(hist_opencv, "64", cv::Point2i(num3_col, num2_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	cv::putText(hist_opencv, "128", cv::Point2i(num4_col, num2_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	cv::putText(hist_opencv, "192", cv::Point2i(num5_col, num2_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);

	int num6_col = 20;
	int num6_row = max_row - 35;
	int num7_row = 55;
	cv::putText(hist_opencv, "0.0", cv::Point2i(num6_col, num6_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	cv::putText(hist_opencv, "1.0", cv::Point2i(num6_col, num7_row), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar::all(255), 0.5, cv::LINE_AA);
	
	int str_row = 20;
	cv::putText(hist_opencv, "Relative Proportion", cv::Point2i(num6_col, str_row), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(255), 0.4, cv::LINE_AA);

	cv::copyMakeBorder(hist_opencv, hist_opencv, 10, 20, 0, 5, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	int str1_row = hist_opencv.rows - 15;
	int str1_col = hist_opencv.cols / 2 - 20;
	cv::putText(hist_opencv, "Pixel Value", cv::Point2i(str1_col, str1_row), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(255), 0.4, cv::LINE_AA);

	float scale = 0.6;
	cv::Mat new_hist;
	cv::resize(hist_opencv, new_hist, cv::Size(round(hist_opencv.cols * scale), round(hist_opencv.rows * scale)));
	cv::copyMakeBorder(new_hist, new_hist, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar::all(255));


	if (display_name != "") {
		cv::imshow(display_name, new_hist);
	}
	if (save_path != "") {
		cv::imwrite(PIC_BASE + save_path, new_hist);
	}
	hist_res = new_hist.clone();
	return 0;
}

//int main() {
//	MyHistogram mh({1,2,3,4,5,6,7,8,9,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 ,2,3,4,5,6,7,8,9 });
//	draw_histogram(mh, "red", 500, 5, "grey hist1");
//	draw_histogram(mh, "green", 500, 5, "grey hist2");
//	draw_histogram(mh, "blue", 500, 5, "grey hist3");
//	std::cout << "waiting for a keyboard hit" << std::endl;
//	cv::waitKey(0);
//}