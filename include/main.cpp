#include <string>
#include <iostream>
#include <vector>
#include "../include/auxiliary.h"
#include "../include/image_io.h"
#include "../include/image_proc.h"
#include "../include/histogram.h"
#include "../include/arg_opt.h"
#include "../include/frequency.h"
#include "../include/display.h"

//#define TEST

using namespace std;



int main(int argc, char *argv[]) {

#ifdef TEST
	do {
		// test case
	} while (0);
	exit(0);
#endif // TEST


	vector<vector<string>> args;
	int cur = 0;
	get_all_opt(argc, argv, args);

	vector<string> display_names;
	vector<MyImageMatrix> display_imgs;

	ControlBlock cb;
	get_info(args, cb);

	MyImageMatrix src_image;
	read_img(cb.pic_path, src_image, cb.mode);

	string img_name = get_path_name(cb.pic_path);
	if (img_name == "") {
		arg_error("invalid file path: \' " + cb.pic_path + " \'");
	}

	display_imgs.push_back(src_image);
	display_names.push_back(img_name);

	MyImageMatrix tar_img;

	//process
	if (cb.fft) {
		if (cb.mode == "RGB") {
			arg_error("FFT only support GREYSCALE image, plz use \'--mode GREY\' instead");
		}
		auto cmat = FFT_image(src_image);
		auto cmat_pixel = get_pixel_trans(*cmat, true);
		tar_img = MyImageMatrix(*cmat_pixel);
		display_names.push_back(img_name + "_fft");
		display_imgs.push_back(tar_img);

		auto i_cmat = get_abs(*IFFT_mat(*cmat));
		debug_complexMat(*i_cmat);
		
		display_imgs.push_back(MyImageMatrix(*i_cmat));
		display_names.push_back(img_name + "_idft");
	}
	else if (cb.dct) {
		if (cb.mode == "dct") {
			arg_error("FFT only support GREYSCALE image, plz use \'--mode GREY\' instead");
		}
		auto cmat = DCT_image(src_image);
		auto cmat_pixel = get_pixel_trans(*cmat, true);
		tar_img = MyImageMatrix(*cmat_pixel);
		display_names.push_back(img_name + "_dct");
		display_imgs.push_back(tar_img);

		auto i_cmat = get_pixel_trans(*DCT_mat(*cmat), true);
		display_imgs.push_back(MyImageMatrix(*i_cmat));
		display_names.push_back(img_name + "_idct");
	}
	else if (cb.he) {

	}
	else if (cb.cs) {

	}
	else {

	}

	// save

	if (cb.save_path != "") {
		write_img(cb.save_path, tar_img, false);
	}
	else {
		write_img("./" + img_name + ".jpg", tar_img, false);
	}

	// display
	if (!cb.silence) {
		if (cb.fft || cb.dct) {
			compare_display_imgs(display_imgs, display_names);
		}
		cv::waitKey(0);
	}


	//// read a image
	//group = args[cur++];
	//if (group[0] != "-f") {
	//	arg_error("You need to specify the INPUT IMAGE PATH");
	//}
	//if (group.size() > 2) {
	//	arg_error("Only ONE image should be specified after -f");
	//}
	//string file = group[1];
	//group = args[cur++];
	//if (group[0] != "--mode") {
	//	arg_error("You need to specify a READ MODE : 'GREY' or 'RGB'");
	//}
	//if (group.size() != 2) {
	//	arg_error("Only ONE image should be specified after --mode");
	//}
	//string mode = group[1];
	//if (mode != "GREY" && mode != "RGB") {
	//	arg_error("You need to specify a READ MODE : 'GREY' or 'RGB'");
	//}
	//MyImageMatrix image;
	//read_img(file, image, mode);
	//// process the image
	//group = args[cur++];
	//string process = group[0];
	//MyImageMatrix tar_image;
	//if (process == "--he") {
	//	if (group.size() > 1) {
	//		arg_error("No parameter should follow --he");
	//	}
	//	
	//	histogram_equalization(image, tar_image);
	//}
	//else if (process == "--cs") {
	//	if (group.size() != 5) {
	//		arg_error("Four parameters should follow --he : a, b, c and d (all numbers between 0~255)");
	//	}
	//	if (!all_number(group[1]) || !all_number(group[2]) || !all_number(group[3]) || !all_number(group[4])) {
	//		arg_error("Only number should follow --he");
	//	}
	//	int a = stoi(group[1]);
	//	int b = stoi(group[2]);
	//	int c = stoi(group[3]);
	//	int d = stoi(group[4]);
	//	contrast_strech_type1(image, tar_image, a, b, c, d);
	//}
	//else if (process == "--fft") {
	//	if (mode == "RGB") {
	//		arg_error("FFT only support GREYSCALE image, use \'--mode GREY\'");
	//	}
	//	if (group.size() > 1) {
	//		arg_error("No parameter should follow --fft");
	//	}
	//	auto cmat = FFT_image(image);
	//	auto cmat_pixel = get_pixel_trans(*cmat, true);
	//	tar_image = MyImageMatrix(*cmat_pixel);
	//}
	//else if (process == "--dct") {
	//	if (mode == "RGB") {
	//		arg_error("DCT only support GREYSCALE image, use \'--mode GREY\'");
	//	}
	//	if (group.size() > 1) {
	//		arg_error("No parameter should follow --dct");
	//	}
	//	auto cmat = DCT_image(image);
	//	auto cmat_pixel = get_pixel_trans(*cmat, true);
	//	tar_image = MyImageMatrix(*cmat_pixel);
	//}
	//else {
	//	arg_error("You need to specify a PROCESS OPTION: --he, --cs, --fft or --dct");
	//}

	//// save the processed image
	//if (cur >= args.size()) {
	//	write_img("processed_image.jpg", tar_image, true);
	//	return 0;
	//}
	//group = args[cur++];
	//if (group[0] == "--save") {
	//	write_img(group[1], tar_image, true);
	//	if (cur < args.size()) group = args[cur++];
	//	else return 0;
	//}
	//else write_img("", tar_image, true);

	//// histogram
	//if (group[0] == "--hist") {
	//	if (image.channels() == 1) {
	//		get_pixel_value_distribution(image, 0);
	//		get_pixel_value_distribution(tar_image, 0);
	//		vector<float> dist;
	//		image.get_histogram_vec(dist, 0);
	//		MyHistogram hist1(dist);
	//		cv::Mat h_m1;
	//		draw_histogram(hist1, h_m1, "", "white");
	//		tar_image.get_histogram_vec(dist, 0);
	//		MyHistogram hist2(dist);
	//		cv::Mat a_m1;
	//		draw_histogram(hist2, a_m1, "", "white");
	//		std::vector<cv::Mat> hists;
	//		hists.push_back(h_m1);
	//		hists.push_back(a_m1);
	//		cv::Mat res;
	//		joint_histograms_type2(hists, 1, 2, res);
	//		cv::imshow("histograms comparision", res);
	//		cv::waitKey(0);
	//	}
	//	else if (image.channels() == 3) {
	//		get_pixel_value_distribution(image, 0);
	//		get_pixel_value_distribution(image, 1);
	//		get_pixel_value_distribution(image, 2);
	//		get_pixel_value_distribution(tar_image, 0);
	//		get_pixel_value_distribution(tar_image, 1);
	//		get_pixel_value_distribution(tar_image, 2);
	//		vector<float> dist;
	//		image.get_histogram_vec(dist, 0);
	//		MyHistogram hist1(dist);
	//		cv::Mat h_m1, h_m2, h_m3;
	//		draw_histogram(hist1, h_m1, "", "green");
	//		image.get_histogram_vec(dist, 1);
	//		MyHistogram hist2(dist);
	//		draw_histogram(hist2, h_m2, "", "blue");
	//		image.get_histogram_vec(dist, 2);
	//		MyHistogram hist3(dist);
	//		draw_histogram(hist3, h_m3, "", "red");
	//		cv::Mat a_m1, a_m2, a_m3;
	//		tar_image.get_histogram_vec(dist, 0);
	//		MyHistogram hist4(dist);
	//		draw_histogram(hist4, a_m1, "", "green");
	//		tar_image.get_histogram_vec(dist, 1);
	//		MyHistogram hist5(dist);
	//		draw_histogram(hist5, a_m2, "", "blue");
	//		tar_image.get_histogram_vec(dist, 2);
	//		MyHistogram hist6(dist);
	//		draw_histogram(hist6, a_m3, "", "red");

	//		std::vector<cv::Mat> hists;
	//		hists.push_back(h_m1);
	//		hists.push_back(h_m2);
	//		hists.push_back(h_m3);
	//		hists.push_back(a_m1);
	//		hists.push_back(a_m2);
	//		hists.push_back(a_m3);

	//		cv::Mat res;
	//		joint_histograms(hists, 2, 3, res);
	//		cv::imshow("histograms comparision", res);



	//		cv::waitKey(0);
	//	}
	//	if (cur < args.size()) group = args[cur++];
	//	else return 0;
	//}
	
	

	return 0;
}