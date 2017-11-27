/*
 * pixel.h
 *
 *  Created on: Nov 27, 2017
 *      Author: trent
 */

#ifndef INCLUDE_VORONOI_ART_IMAGE_PROCESSING_PIXEL_H_
#define INCLUDE_VORONOI_ART_IMAGE_PROCESSING_PIXEL_H_
#include <opencv/cv.hpp>
namespace voronoi_art {

typedef cv::Point2f cv_float_point;
typedef cv::Point cv_int_point;
class Pixel {
	friend class image_processing;
public:
	Pixel();
	virtual ~Pixel();
	const cv::Scalar& get_color() const;
	const cv_float_point& get_float_point() const;
	const cv_int_point& get_int_point() const;

protected:
	cv_float_point float_point_;
	cv_int_point int_point_;
	cv::Scalar color_;
};

} /* namespace voronoi_art */

#endif /* INCLUDE_VORONOI_ART_IMAGE_PROCESSING_PIXEL_H_ */
