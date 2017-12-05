#include <string>
#include "voronoi_art/image_processing/image_processing.h"
#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include <boost/program_options.hpp>

#include <iostream>
namespace po = boost::program_options;
using namespace voronoi_art;

int main(int, char**) {
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	Size size(800, 800);
	Mat image_resized, sharpenned_image(size, IMAGE_TYPE, Scalar::all(0));                     //resize image
	voronoi_art::image_processing im_proc;


	std::vector<cv::Point> cv_points;
	std::shared_ptr<voronoi_processing> vp;
	namedWindow("feed", 1);

	namedWindow("voronoi", 2);
	VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),7, size);

	while(true){
		Mat image,voronoi_image;
		cap >> image; // get a new frame from camera
		resize(image, image_resized, size);
		sharpenned_image = im_proc.sharpen(image_resized);
		vector<PixelFunctor> filters;
		filters.push_back(im_proc.random_dropout(0.9725));
		//filters.push_back(im_proc.gradient_threshold(sharpenned_image,std::round(255.0 * 0.2)));
		cv_points = im_proc.filter_intersection(sharpenned_image, filters);
		std::vector<point_type> site_points(cv_points.size());

		for (const cv::Point& point : cv_points) {
			site_points.push_back(
					voronoi_processing::cv_point_to_voronoi(point));
		}


		Mat display(sharpenned_image.size(), sharpenned_image.type(), Scalar::all(0));
		vp.reset(new voronoi_processing(sharpenned_image, site_points,false));
		vp->draw_edges(display);
//        cvtColor(frame, edges, COLOR_BGR2GRAY);
//        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//        Canny(edges, edges, 0, 30, 3);
		imshow("feed", sharpenned_image);
		imshow("voronoi", display);
		video.write(display);
		char c = (char)waitKey(1);
		    if( c == 27 )
		      break;
	}
	  // When everything done, release the video capture and write object
	  cap.release();
	  video.release();

	  // Closes all the windows
	  destroyAllWindows();	return 0;
}
