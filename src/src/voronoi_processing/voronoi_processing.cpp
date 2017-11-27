/*
 * voronoi_processing.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include <math.h>
#include <boost/bind.hpp>
#include <boost/polygon/polygon.hpp>
#include <boost/geometry.hpp>
namespace voronoi_art {

voronoi_processing::voronoi_processing() {
	// TODO Auto-generated constructor stub

}

voronoi_processing::~voronoi_processing() {
	// TODO Auto-generated destructor stub
}

point_type voronoi_processing::cv_point_to_voronoi(const cv::Point& pt) {
	return point_type(pt.x,pt.y);
}

cv::Point voronoi_processing::voronoi_vertex_to_cv_point(
		const VD::vertex_type* vertex) {
	int x = std::round(vertex->x());
	int y = std::round(vertex->y());
	return cv::Point(x, y);
}



void voronoi_processing::iterate_voronoi_edges(
		const VD& voronoi_diagram, const iterate_edges_func& func) {
	  for (VD::const_edge_iterator it = voronoi_diagram.edges().begin();
	       it != voronoi_diagram.edges().end(); ++it) {

	  //  if (it->is_primary())
	  //  {
	    	func(it, voronoi_diagram);
	  //  }
	  }
}
void voronoi_processing::draw_edge(voronoi_art::VD::const_edge_iterator& edge,
		const voronoi_art::VD& vd,
		cv::Mat& image,
		const cv::Mat& in_image,
		const vector<point_type>& points)
{

   const VD::vertex_type* v0 = edge->vertex0();
   const VD::vertex_type* v1 = edge->vertex1();
   Scalar color;
   if(v0 && v1)
   {
	  int r = points[edge->cell()->source_index()].y();
	  int c = points[edge->cell()->source_index()].x();
	//  std::cout<<"Reading pixel."<<std::endl;
	  color = in_image.at<Vec3b>(r,c);
	  cv::line(image,voronoi_vertex_to_cv_point(v0),voronoi_vertex_to_cv_point(v1), color);
   }

}
void voronoi_processing::draw_edges(Mat& image,
		const Mat& in_image,
		const VD& vd, const vector<point_type>& points) {

	iterate_edges_func fun = boost::bind(draw_edge,_1,_2, image, in_image, points);
	iterate_voronoi_edges(vd,fun);
}
} /* namespace voronoi_art */
