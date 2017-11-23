/*
 * image_processing.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#include "image_processing/image_processing.h"
#include <iostream>
#include <string>
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

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
Mat image_processing::image_gradient(const Mat& image) {
	Mat src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	cvtColor(image, src_gray, CV_RGB2GRAY);
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
vector<Point> image_processing::high_gradient_points(const Mat& grad,
		const int& threshold) {
	vector<Point> rtn;
	for(int y = 0; y < grad.rows; y++){
		for(int x = 0; x < grad.cols; x++){
			Scalar intensity = grad.at<uchar>(y,x);
			if(intensity[0]>=threshold){
				rtn.push_back(Point(x,y));
			}
		}
	}
	return rtn;
}
} /* namespace voronoi_art */


