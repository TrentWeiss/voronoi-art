#include <string>
#include "voronoi_art/image_processing/image_processing.h"
#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include <boost/program_options.hpp>

#include <iostream>
namespace po = boost::program_options;
using namespace voronoi_art;

int main(int argc, char* argv[]) {
	std::string image_name;
	float float_threshold, float_prob;
	std::string output_image;
	po::options_description desc("Allowed options");
	desc.add_options()("help,h", "produce help message")("input_image,i",
			po::value<string>(&image_name),
			"sets input image. Currently, only .jpg files are supported.")(
			"output_image,o", po::value<string>(&output_image),
			"file to write the output to. If not set, just opens a display window.")
			("gradient_threshold,g",po::value<float>(&float_threshold)->default_value(0.0), "Threshold for the image gradient. Pixels with a gradient above this value will be selected as site points.")
			("random_threshold,r",po::value<float>(&float_prob)->default_value(0.97),"sets the probability that a pixel will randomly NOT be selected as a site point, regardless of the gradient filter.")(
			"draw_edges,e", "Draw the edges of the voronoi diagram")(
			"draw_cells,c", "Draw the cells of the voronoi diagram")(
			"delaunay,d",
			"Use the Delaunay Triangulation instead of the voronoi diagram");
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	if (vm.count("help") or vm.count("H")) {
		desc.print(std::cout, 25);
		exit(0);
	}
	if (image_name.compare("") == 0) {
		std::cerr << "Input jpg must be specified." << std::endl;
		desc.print(std::cerr, 25);
		exit(-1);
	}
	Mat image;
	image = imread(image_name, IMREAD_UNCHANGED); // Read the file
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	Size size(600, 600);
	Mat image_resized, sharpenned_image;
	resize(image, image_resized, size);                      //resize image
	voronoi_art::image_processing im_proc;
	sharpenned_image = im_proc.sharpen(image_resized);
	std::vector<cv::Point> cv_points;
	vector<PixelFunctor> filters;
	filters.push_back(im_proc.random_dropout(float_prob));
	filters.push_back(
			im_proc.gradient_threshold(sharpenned_image,
					std::round(255.0 * float_threshold)));
	filters.push_back(
			im_proc.laplacian_threshold(sharpenned_image,
					std::round(255.0 * float_threshold)));

	cv_points = im_proc.filter_intersection(sharpenned_image, filters);
	std::cout << "Extracted " << cv_points.size() << " points." << std::endl;
	std::vector<point_type> site_points(cv_points.size());

	for (const cv::Point& point : cv_points) {
		site_points.push_back(voronoi_processing::cv_point_to_voronoi(point));
	}
	bool delaunay = vm.count("delaunay") > 0;
	const voronoi_processing vp(sharpenned_image, site_points, delaunay);

	std::cout << "VD has " << vp.get_voronoi_diagram()->edges().size()
			<< " edges." << std::endl;

	if (!(!vp.get_delaunay_triangulation())) {
		std::cout << "Delaunay Triangulation has: "
				<< vp.get_delaunay_triangulation()->m_edges.size() << " edges."
				<< std::endl;
	}
	Mat display(image_resized.size(), image_resized.type(), Scalar::all(0));
	Mat input_clone = image_resized.clone();
	//display=input_clone;
	if (vm.count("draw_edges")) {
		vp.draw_edges(display);
	}
	if (vm.count("draw_cells")) {
		vp.draw_cells(display);
	}
	if (!output_image.empty()) {
		if (delaunay && vm.count("draw_edges")) {
			Mat delaunay_display(image_resized.size(), image_resized.type(),
					Scalar::all(0));
			vp.draw_delaunay_edges(delaunay_display);
			cv::imwrite("delaunay_" + output_image, delaunay_display);
		}
		cv::imwrite(output_image, display);
		cv::imwrite("resized_" + image_name, sharpenned_image);
	} else {
		if (delaunay && vm.count("draw_edges")) {
			Mat delaunay_display(image_resized.size(), image_resized.type(),
					Scalar::all(0));
			vp.draw_delaunay_edges(delaunay_display);
			namedWindow("Delaunay Art", WINDOW_AUTOSIZE);
			imshow("Delaunay Art", delaunay_display);
		}
		namedWindow("Voronoi Art", WINDOW_AUTOSIZE);
		imshow("Voronoi Art", display);
		namedWindow("Input Image", WINDOW_AUTOSIZE);
		imshow("Input Image", sharpenned_image);
		waitKey(0);
	}
	return 0;
}
