/*
 * pixel.cpp
 *
 *  Created on: Nov 27, 2017
 *      Author: trent
 */

#include <voronoi_art/image_processing/pixel.h>

namespace voronoi_art {

Pixel::Pixel() {
	// TODO Auto-generated constructor stub

}

Pixel::~Pixel() {
	// TODO Auto-generated destructor stub
}
const cv::Scalar& Pixel::get_color() const {
	return color_;
}

const cv_float_point& Pixel::get_float_point() const {
	return float_point_;
}

const cv_int_point& Pixel::get_int_point() const {
	return int_point_;
}

} /* namespace voronoi_art */


