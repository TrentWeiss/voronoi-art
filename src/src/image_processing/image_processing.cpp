/*
 * image_processing.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#include "image_processing/image_processing.h"
#include <iostream>
#include <string>
#include <boost/bind.hpp>
namespace voronoi_art {

image_processing::image_processing() {
	// TODO Auto-generated constructor stub

}

image_processing::~image_processing() {
	// TODO Auto-generated destructor stub
}
string image_processing::type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:
		r = "8U";
		break;
	case CV_8S:
		r = "8S";
		break;
	case CV_16U:
		r = "16U";
		break;
	case CV_16S:
		r = "16S";
		break;
	case CV_32S:
		r = "32S";
		break;
	case CV_32F:
		r = "32F";
		break;
	case CV_64F:
		r = "64F";
		break;
	default:
		r = "User";
		break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}
Mat image_processing::image_laplacian(const Mat& image) {
	int kernel_size = 3;
	Mat output, src_gray;
	int ddepth = IMAGE_TYPE;
	cvtColor(image, src_gray, CV_RGB2GRAY, 1);
	Laplacian(src_gray, output, ddepth, kernel_size);
	return output;
}
Mat image_processing::image_gradient(const Mat& image) {
	Mat src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = IMAGE_TYPE;
	cvtColor(image, src_gray, CV_RGB2GRAY, 1);
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	return grad;
}

Mat image_processing::sharpen(const Mat& image) {
	Mat rtn;
	GaussianBlur(image, rtn, cv::Size(3, 3), 0.0);
	addWeighted(image, 1.5, rtn, -0.5, 0, rtn);

	return rtn;
}
typedef uchar Pixel;
struct Operator {
	void operator ()(Pixel &pixel, const int * position) const {
	}
};
template<typename pixel_type> inline
unsigned int average(const Mat& mat) {
	unsigned int sum = 0;
	unsigned int num_pixels = mat.rows * mat.cols;
	for (unsigned int r = 0; r < mat.rows; r++) {
		for (unsigned int c = 0; c < mat.cols; c++) {
			Scalar pix = mat.at<pixel_type>(r, c);
			for (unsigned int i = 0; i < 4; i++) {
				sum += pix[i];
			}
		}
	}
	unsigned int average = (unsigned int) ((float) (sum) / (float) (num_pixels));
	return average;
}
vector<Point> image_processing::filter_intersection(const Mat& input,
		const vector<PixelFunctor>& filters) {
	vector<Point> rtn;
	for (unsigned int r = 0; r < input.rows; ++r) {
		for (unsigned int c = 0; c < input.cols; ++c) {
			bool include_pixel = true;
			for (const PixelFunctor filter : filters) {
				if(!filter(input, r, c)){
					include_pixel=false;
					break;
				}
			}
			if(include_pixel){
				rtn.push_back(Point(c, r));
			}
		}
	}
	return rtn;
}
vector<Point> image_processing::filter_union(const Mat& input,
		const vector<PixelFunctor>& filters) {
	vector<Point> rtn;
	for (unsigned int r = 0; r < input.rows; ++r) {
		for (unsigned int c = 0; c < input.cols; ++c) {
			for (const PixelFunctor filter : filters) {
				if (filter(input, r, c)) {
					rtn.push_back(Point(c, r));
					break;
				}
			}
		}
	}
	return rtn;
}
bool simple_threshold(const Mat& input, const unsigned int& r,
		const unsigned int& c, const unsigned int& threshold) {
	Scalar intensity = input.at<uchar>(r, c);
	return intensity[0] >= (double)threshold;

}
PixelFunctor image_processing::gradient_threshold(const Mat& input,
		const unsigned int& threshold) {
	const Mat grad = image_gradient(input);
	return boost::bind(simple_threshold,grad,_2,_3,threshold);
}

PixelFunctor image_processing::laplacian_threshold(const Mat& input,
		const unsigned int& threshold) {
	const Mat laplacian = image_laplacian(input);
	return boost::bind(simple_threshold,laplacian,_2,_3,threshold);
}
} /* namespace voronoi_art */

