/*
					License Agreement
					For Open Source Computer Vision Library
					(3-clause BSD License)
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

   * Redistribution's of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

   * Redistribution's in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   * The name of the copyright holders may not be used to endorse or promote products
     derived from this software without specific prior written permission.

 This software is provided by the copyright holders and contributors "as is" and
 any express or implied warranties, including, but not limited to, the implied
 warranties of merchantability and fitness for a particular purpose are disclaimed.
 In no event shall the Intel Corporation or contributors be liable for any direct,
 indirect, incidental, special, exemplary, or consequential damages
 (including, but not limited to, procurement of substitute goods or services;
 loss of use, data, or profits; or business interruption) however caused
 and on any theory of liability, whether in contract, strict liability,
 or tort (including negligence or otherwise) arising in any way out of
 the use of this software, even if advised of the possibility of such damage.



 *  Created on: Nov 22, 2017
 *      Author: Trent Weiss
 */

#include "voronoi_art/image_processing/image_processing.h"
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <ctime>
namespace voronoi_art {

image_processing::image_processing() : gen(std::time(0)), dist(0.0, 1.0){
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
				rtn.push_back(cv_float_point(c, r));
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
					rtn.push_back(cv_float_point(c, r));
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
bool image_processing::random_threshold(const Mat& mat, const unsigned int& r, const unsigned int& c, const float& threshold){
	return dist(gen)>=threshold;
}

vector<Pixel> image_processing::image_to_point_vector(const Mat& image) {
	vector<Pixel> rtn(image.rows * image.cols);
	unsigned int index = 0;
	for(unsigned int r = 0; r < image.rows; ++r){
		for(unsigned int c = 0; c < image.cols; ++c){
			rtn[index].float_point_=cv_float_point((float)c, (float)r);
			rtn[index].int_point_=cv_int_point(c, r);
			rtn[index++].color_=image.at<Vec3b>(r,c);
		}
	}
	return rtn;
}

PixelFunctor image_processing::random_dropout(const float& dropout_prob){
	return boost::bind(&image_processing::random_threshold, this, _1,_2,_3,dropout_prob);
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
bool gt_threshold(const Mat& input, const unsigned int& r,
		const unsigned int& c, const unsigned int& threshold) {
	Scalar intensity = input.at<Vec3b>(r, c);
	return intensity.ddot(Scalar(1.0,1.0,1.0,0.0))/3.0 >= (double)threshold;

}
bool lt_threshold(const Mat& input, const unsigned int& r,
		const unsigned int& c, const unsigned int& threshold) {
	Scalar intensity = input.at<Vec3b>(r, c);
	return intensity.ddot(Scalar(1.0,1.0,1.0,0.0))/3.0 < (double)threshold;

}
PixelFunctor image_processing::pixel_lt(const Mat& input,
		const unsigned int& threshold) {
	return boost::bind(lt_threshold,input,_2,_3,threshold);
}
PixelFunctor image_processing::pixel_gt(const Mat& input,
		const unsigned int& threshold) {
	return boost::bind(gt_threshold,input,_2,_3,threshold);
}
} /* namespace voronoi_art */


