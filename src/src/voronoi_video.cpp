#include <string>
#include "voronoi_art/image_processing/image_processing.h"
#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include <boost/program_options.hpp>

#include <iostream>
namespace po = boost::program_options;
using namespace voronoi_art;

int main(int argc, char** argv) {
	std::string image_name;
	float float_threshold, float_prob;
	int frame_rate;
	bool delaunay;
	std::string output_video;
	po::options_description desc("Allowed options");
	desc.add_options()
			("help,h", "Print help message and exit, all other options are ignored")
			("output_video,o", po::value<string>(&output_video), "file to write the output to. If not set, no file is written.")
			("frame_rate,f", po::value<int>(&frame_rate)->default_value(7), "Frames per second for the output video. Irrelevant if output_video is not specified.")
			("gradient_threshold,g",po::value<float>(&float_threshold)->default_value(0.0), "Threshold for the image gradient. Pixels with a gradient above this value will be selected as site points.")
			("random_threshold,r",po::value<float>(&float_prob)->default_value(0.97),"sets the probability that a pixel will randomly NOT be selected as a site point.")
			("delaunay,d","Render the extracted Delaunay Triangulation instead of the voronoi diagram.");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	if (vm.count("help") or vm.count("H")) {
		desc.print(std::cout, 30);
		exit(0);
	}
	delaunay = vm.count("delaunay")>0;
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
	std::shared_ptr<VideoWriter> video;
	if(vm.count("output_video")){
		video.reset(new VideoWriter(output_video +".avi",CV_FOURCC('M','J','P','G'), frame_rate, size));
	}

	vector<PixelFunctor> filters;
	if(float_prob>0.0){
		filters.push_back(im_proc.random_dropout(float_prob));
	}
	if(float_threshold>0.0){
		filters.push_back(im_proc.gradient_threshold(sharpenned_image,std::round(255.0 * float_threshold)));
	}
	while(true){
		Mat image,voronoi_image;
		cap >> image; // get a new frame from camera
		resize(image, image_resized, size);
		sharpenned_image = im_proc.sharpen(image_resized);
		cv_points = im_proc.filter_intersection(sharpenned_image, filters);
		std::vector<point_type> site_points(cv_points.size());

		for (const cv::Point& point : cv_points) {
			site_points.push_back(
					voronoi_processing::cv_point_to_voronoi(point));
		}



		Mat display(sharpenned_image.size(), sharpenned_image.type(), Scalar::all(0));
		vp.reset(new voronoi_processing(sharpenned_image, site_points,delaunay));
		if(delaunay){
			vp->draw_delaunay_edges(display);
		}else{
			vp->draw_edges(display);
		}
//        cvtColor(frame, edges, COLOR_BGR2GRAY);
//        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//        Canny(edges, edges, 0, 30, 3);
		imshow("feed", sharpenned_image);
		imshow("voronoi", display);
		if(!(!video)){
			video->write(display);
		}
		char c = (char)waitKey(1);
		    if( c == 27 )
		      break;
	}
	  // When everything done, release the video capture and write object
	  cap.release();
	  video->release();

	  // Closes all the windows
	  destroyAllWindows();	return 0;
}
