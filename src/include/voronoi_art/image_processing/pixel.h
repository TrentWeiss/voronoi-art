/*
 
					License Agreement 
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
       Created on: Nov 27, 2017
       Author: Trent Weiss
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
