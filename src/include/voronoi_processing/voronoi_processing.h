/*
 * voronoi_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_VORONOI_PROCESSING_H_
#define INCLUDE_VORONOI_PROCESSING_H_
#include <image_processing/image_processing.h>
#include <boost/polygon/voronoi.hpp>
#include <boost/function.hpp>
namespace voronoi_art {
using namespace boost::polygon;

typedef voronoi_diagram<double> VD;
typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
class voronoi_processing {
	typedef boost::function<void (VD::const_edge_iterator& edge)> iterate_edges_func;
public:
	voronoi_processing();
	virtual ~voronoi_processing();
	static void iterate_voronoi_edges(const VD& voronoi_diagram,const iterate_edges_func& func);
	static void draw_voronoi_edges(Mat& image, const VD& vd);

	static void draw_edge(voronoi_art::VD::const_edge_iterator& edge, cv::Mat& image);

	static point_type cv_point_to_voronoi(const cv::Point& pt);

	static cv::Point voronoi_vertex_to_cv_point(const VD::vertex_type* vertex);

};

} /* namespace voronoi_art */

#endif /* INCLUDE_VORONOI_PROCESSING_H_ */
