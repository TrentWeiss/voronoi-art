/*
								License Agreement 
								3-Clause BSD License
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the names of the copyright holders nor the names of the contributors may be used to endorse or promote products derived from this software without specific prior written permission.

This software is provided by the copyright holders and contributors “as is” and any express or implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. 
In no event shall copyright holders or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; 
or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.
 *  Created on: Nov 22, 2017
 *      Author: Trent Weiss
 */

#ifndef INCLUDE_IMAGE_PROCESSING_H_
#define INCLUDE_IMAGE_PROCESSING_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "voronoi_art/image_processing/pixel.h"
#define IMAGE_TYPE CV_8U

using namespace cv;
using namespace std;
#ifdef USE_STD_FUNCTION
#include <functional>
typedef function<bool (const Mat& mat, const unsigned int& r, const unsigned int& c)> PixelFunctor;

#else
#include <boost/function.hpp>
typedef boost::function<bool (const Mat& mat, const unsigned int& r, const unsigned int& c)> PixelFunctor;

#endif
namespace voronoi_art {
//struct Pixel{
//
//	cv::Point point_;
//	Scalar color_;
//};
class image_processing {
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<float> dist;
	bool random_threshold(const Mat& mat, const unsigned int& r, const unsigned int& c, const float& threshold);
public:
	image_processing();
	virtual ~image_processing();

	string type2str(int type);
	static Mat sharpen(const Mat& image);
	static Mat image_gradient(const Mat& image);
	static Mat image_laplacian(const Mat& image);
	static vector<Pixel> image_to_point_vector(const Mat& image);
	static vector<Point> filter_intersection(const Mat& input, const vector<PixelFunctor>& filters);
	static vector<Point> filter_union(const Mat& input, const vector<PixelFunctor>& filters);
	static PixelFunctor pixel_gt(const Mat& input, const unsigned int& threshold);
	static PixelFunctor pixel_lt(const Mat& input, const unsigned int& threshold);
	static PixelFunctor gradient_threshold(const Mat& input, const unsigned int& threshold);
	static PixelFunctor laplacian_threshold(const Mat& input, const unsigned int& threshold);
	PixelFunctor random_dropout(const float& dropout_prob);
};

} /* namespace voronoi_art */

#endif /* INCLUDE_IMAGE_PROCESSING_H_ */
