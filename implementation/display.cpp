#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "../include/display.h"
#include "../include/image_io.h"

using namespace std;



void compare_display_imgs(vector<MyImageMatrix>& imgs, vector<string> names) {
	std::vector<cv::Mat> hlist;
	cv::Mat res;
	cv::Mat tem;
	int borderType = cv::BORDER_CONSTANT;
	for (int i = 0; i < imgs.size(); i++) {
		MyImageMatrix& img = imgs[i];
		img2cvmat(img, tem);
		cv::copyMakeBorder(tem, tem, 10, 10, 5, 5, borderType, cv::Scalar::all(40));
		string name = names[i];
		cv::copyMakeBorder(tem, tem, 40, 0, 0, 0, borderType, cv::Scalar::all(200));
		cv::putText(tem, name, cv::Point2i(20, 25), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar::all(20), 1.0, cv::LINE_AA);
		hlist.push_back(tem);
	}
	cv::hconcat(hlist, res);

	cv::imshow("res", res);

}

void img2cvmat(const MyImageMatrix& img, cv::Mat& mat) {
	int rows = img.shape[0];
	int cols = img.shape[1];
	int channels = img.shape[2];
	cout << channels << endl;
	cv::Mat image(rows, cols, CV_8UC(channels), cv::Scalar::all(128));
	if (channels == 3) {
		for (int i = 0; i < rows; i++) {
			cv::Vec3b* row = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < cols; j++) {
				cv::Vec3b& ch = row[j];
				for (int k = 0; k < channels; k++) {
					// rgb to bgr
					ch[k] = img.mat[i][j].rgb[k];
				}
			}
		}
	}
	else if (channels == 1) {
		for (int i = 0; i < rows; i++) {
			uchar* row = image.ptr<uchar>(i);
			for (int j = 0; j < cols; j++) {
				row[j] = img.mat[i][j].rgb[0];
			}
		}
	}
	mat = image;
	
	return;
}