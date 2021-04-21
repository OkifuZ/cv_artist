#pragma once

#include <iostream>
#include <vector>
#include <string>
//#include "../include/auxiliary.h"
//#include "../include/image_io.h"


int get_pixel_value_distribution(MyImageMatrix& src_img, int which_channel);

int histogram_equalization(MyImageMatrix& src_img, MyImageMatrix& tar_img);


int contrast_strech_type1(MyImageMatrix& src_img, MyImageMatrix& tar_img, 
	int from_low, int from_high, int to_low, int to_high, int mode=0);

int contrast_strech_type2(MyImageMatrix& src_img, MyImageMatrix& tar_img,
	float slope, float intercept, int from_low, int from_high, int mode=0);