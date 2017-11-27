#include <iostream>
#include <string>

#include <voronoi_art/voronoi_processing/voronoi_processing.h>
#include <boost/program_options.hpp>
namespace voronoi_art{
int iterate_primary_edges1(const VD& vd) {
  int result = 0;
  for (VD::const_edge_iterator it = vd.edges().begin();
       it != vd.edges().end(); ++it) {

    if (it->is_primary() && it->vertex0()!=NULL && it->vertex1()!=NULL)
    {
    	result++;
    	std::cout<< "(" << it->vertex0()->x() <<", "<<it->vertex0()->y()<<")"
    			", (" << it->vertex1()->x() <<", "<<it->vertex1()->y()<<")"<<std::endl;
    }
  }
  return result;
}
}

namespace po = boost::program_options;
using namespace voronoi_art;

int main(int argc, char* argv[])
{
	std::string image_name;
	float float_threshold, float_prob;
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help,h", "produce help message")
	    ("input_image,i", po::value<string>(&image_name), "sets input image. Currently, only .jpg files are supported.")
		("gradient_threshold,g",po::value<float>(&float_threshold)->default_value(0.0),"sets the threshold for selecting pixels based on image gradient on a 0-1 scale. Default: 0.25")
		("random_threshold,r",po::value<float>(&float_prob)->default_value(0.75),"sets the probability that a pixel will randomly be excluded. Default: 0.75")
	;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	if(vm.count("help") or vm.count("H")){
		desc.print(std::cout,25);
		exit(0);
	}
	if(image_name.compare("")==0){
		std::cerr<<"Input jpg must be specified."<<std::endl;
		desc.print(std::cerr,25);
		exit(-1);
	}
	Mat image;
	image = imread(image_name, IMREAD_UNCHANGED); // Read the file
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Size size(600,600);
	Mat image_resized,sharpenned_image;
	resize(image,image_resized,size);//resize image
	voronoi_art::image_processing im_proc;
	sharpenned_image = im_proc.sharpen(image_resized);
	std::vector<cv::Point> site_points;
	vector<PixelFunctor> filters;
	filters.push_back(im_proc.gradient_threshold(sharpenned_image,std::round(255.0*float_threshold)));
	filters.push_back(im_proc.laplacian_threshold(sharpenned_image,std::round(255.0*float_threshold)));
	filters.push_back(im_proc.random_dropout(float_prob));

	site_points=im_proc.filter_intersection(sharpenned_image,filters);
	std::cout <<"Extracted " << site_points.size() << " points." <<std::endl;
    std::vector<point_type> points(site_points.size());

    for(const cv::Point& point: site_points){
    	points.push_back(voronoi_processing::cv_point_to_voronoi(point));
    }
    VD vd;  construct_voronoi(points.begin(), points.end(), &vd);
    std::cout<<"VD has " << vd.edges().size() << " edges." <<std::endl;
    Mat display(image_resized.size(),CV_8UC3,Scalar::all(0));
    Mat input_clone=image_resized.clone();
    //display=input_clone;
    voronoi_processing::draw_edges(display, sharpenned_image, vd, points);
    namedWindow("Voronoi Art", WINDOW_AUTOSIZE);
    imshow("Voronoi Art", display);
    namedWindow("Input Image", WINDOW_AUTOSIZE);
    imshow("Input Image", sharpenned_image);
	waitKey(0);
    return 0;
}
