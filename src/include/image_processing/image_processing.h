/*
 * image_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_IMAGE_PROCESSING_H_
#define INCLUDE_IMAGE_PROCESSING_H_

#include <opencv2/opencv.hpp>
#define IMAGE_TYPE CV_8U

using namespace cv;
using namespace std;
namespace voronoi_art {

class image_processing {
public:
	image_processing();
	virtual ~image_processing();
	static Mat image_gradient(const Mat& image);
	static Mat image_laplacian(const Mat& image);
	static string type2str(int type);
	static vector<Point> high_gradient_points(const Mat& grad, const float& threshold);
};

} /* namespace voronoi_art */

#endif /* INCLUDE_IMAGE_PROCESSING_H_ */
