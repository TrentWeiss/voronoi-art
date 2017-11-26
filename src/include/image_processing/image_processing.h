/*
 * image_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_IMAGE_PROCESSING_H_
#define INCLUDE_IMAGE_PROCESSING_H_

#include <opencv/cv.hpp>
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
class image_processing {
public:
	image_processing();
	virtual ~image_processing();
	static string type2str(int type);
	static Mat sharpen(const Mat& image);
	static Mat image_gradient(const Mat& image);
	static Mat image_laplacian(const Mat& image);
	static vector<Point> filter_intersection(const Mat& input, const vector<PixelFunctor>& filters);
	static vector<Point> filter_union(const Mat& input, const vector<PixelFunctor>& filters);
	static PixelFunctor gradient_threshold(const Mat& input, const unsigned int& threshold);
	static PixelFunctor laplacian_threshold(const Mat& input, const unsigned int& threshold);
};

} /* namespace voronoi_art */

#endif /* INCLUDE_IMAGE_PROCESSING_H_ */
