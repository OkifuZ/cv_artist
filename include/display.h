#pragma once

#include <iostream>
#include <string>
#include <vector>

class MyImageMatrix;

void compare_display_imgs(std::vector<MyImageMatrix>& imgs, std::vector<std::string> names);

void img2cvmat(const MyImageMatrix& img, cv::Mat& mat);