/*
 * image_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_IMAGE_PROCESSING_H_
#define INCLUDE_IMAGE_PROCESSING_H_
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
namespace voronoi_art {

class image_processing {
public:
	image_processing();
	virtual ~image_processing();
	static Mat imageGradient(const Mat& image);
	static string type2str(int type);
};

} /* namespace voronoi_art */

#endif /* INCLUDE_IMAGE_PROCESSING_H_ */
