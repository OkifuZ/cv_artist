#pragma once

#include<opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include "../include/image_io.h"


class MyHistogram {

	friend int draw_histogram(const MyHistogram& histogram, cv::Mat& ,std::string display_name, const std::string& color, int max_height, int interval,  std::string save_path);
	friend int normalize_histogram(MyHistogram& histogram, int low, int high);
	

public:
	int cols = 0;
	std::vector<float> values{};
	std::string description;

	MyHistogram(int cols_, std::string des_ = "") : cols(cols_), description(des_) {}
	MyHistogram(const std::vector<float>& values_, std::string des_ = "") :
		cols(values_.size()), values(values_), description(des_) {}

	bool is_empty() const { return values.empty(); }
	int size() const { return values.size(); }

private:

};

int normalize_histogram(MyHistogram& histogram, int low, int high);

int draw_histogram(const MyHistogram& histogram,
	cv::Mat&, 
	std::string display_name = "",
	const std::string& color = "white",
	int max_height = 500, int interval = 3,
	std::string save_path = "");

