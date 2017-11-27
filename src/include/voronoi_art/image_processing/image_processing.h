/*
 * image_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_IMAGE_PROCESSING_H_
#define INCLUDE_IMAGE_PROCESSING_H_

#include <opencv/cv.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
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
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<float> dist;
	float random_float01();
	bool random_threshold(const Mat& mat, const unsigned int& r, const unsigned int& c, const float& threshold);
public:
	image_processing();
	virtual ~image_processing();

	string type2str(int type);
	Mat sharpen(const Mat& image);
	Mat image_gradient(const Mat& image);
	Mat image_laplacian(const Mat& image);
	vector<Point> filter_intersection(const Mat& input, const vector<PixelFunctor>& filters);
	vector<Point> filter_union(const Mat& input, const vector<PixelFunctor>& filters);
	PixelFunctor gradient_threshold(const Mat& input, const unsigned int& threshold);
	PixelFunctor laplacian_threshold(const Mat& input, const unsigned int& threshold);
	PixelFunctor random_dropout(const float& dropout_prob);
};

} /* namespace voronoi_art */

#endif /* INCLUDE_IMAGE_PROCESSING_H_ */
