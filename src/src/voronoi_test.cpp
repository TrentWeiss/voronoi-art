#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <voronoi_processing/voronoi_processing.h>
namespace fs = boost::filesystem;

using namespace voronoi_art;

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

int main(int argc, char* argv[])
{
	std::string image_name;
	int threshold;
	if (argc > 1) {
		threshold = std::atoi(argv[1]);
	}
	else{
		threshold=175;
	}
	if(argc > 2){
		image_name = std::string(argv[2]);
	}else{
		image_name = "eagle.jpg";
	}
    fs::path full_path = fs::path("..") / fs::path("data")  / fs::path(image_name);
	Mat image;
	image = imread(full_path.string(), IMREAD_UNCHANGED); // Read the file
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Mat grad = image_processing::image_gradient(image);
	std::vector<cv::Point> high_gradient_points =
			image_processing::high_gradient_points(grad,threshold);
    std::vector<point_type> points(high_gradient_points.size());

    for(const cv::Point& point: high_gradient_points){
    	points.push_back(voronoi_processing::cv_point_to_voronoi(point));
    }
    VD vd;  construct_voronoi(points.begin(), points.end(), &vd);
    std::cout<<"VD has " << vd.edges().size() << " edges." <<std::endl;
    std::cout<<"Image is: " << grad.rows << " X " << grad.cols<<std::endl;
    Mat display(grad.size(),CV_8UC1,cv::Scalar(0,0,0));
    voronoi_processing::draw_voronoi_edges(display,vd);
    namedWindow("Voronoi Art", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Voronoi Art", display);                // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window

    return 0;
}
