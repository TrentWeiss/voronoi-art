/*
 * voronoi_processing.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include <math.h>
#include <boost/bind.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/within.hpp>
typedef boost::geometry::model::d2::point_xy<double> point_poly;
namespace voronoi_art {


voronoi_processing::voronoi_processing(const cv::Mat& input_image,
		const std::vector<point_type>& site_points) {
	vd_ = new VD();
	input_image_ = input_image.clone();
	site_points_ = std::vector<point_type>(site_points);
	construct_voronoi(site_points_.begin(), site_points_.end(), vd_);
}
voronoi_processing::~voronoi_processing() {
	// TODO Auto-generated destructor stub
	delete vd_;
}

point_type voronoi_processing::cv_point_to_voronoi(const cv::Point& pt) {
	return point_type(pt.x, pt.y);
}

cv::Point voronoi_processing::voronoi_vertex_to_cv_point(
		const VD::vertex_type* vertex) {
	int x = std::round(vertex->x());
	int y = std::round(vertex->y());
	return cv::Point(x, y);
}

const voronoi_art::VD* voronoi_processing::get_voronoi_diagram() const {
	return vd_;
}

const std::vector<point_type>& voronoi_processing::get_site_points() const {
	return site_points_;
}

void voronoi_processing::draw_cells(cv::Mat& image) {
}

void voronoi_processing::draw_cell(voronoi_art::VD::const_cell_iterator& cell,
		cv::Mat& image) {

}
void voronoi_processing::draw_edge(voronoi_art::VD::const_edge_iterator& edge,
		cv::Mat& image) {

	const VD::vertex_type* v0 = edge->vertex0();
	const VD::vertex_type* v1 = edge->vertex1();
	Scalar color;
	if (v0 && v1) {
		int r = site_points_[edge->cell()->source_index()].y();
		int c = site_points_[edge->cell()->source_index()].x();
		color = input_image_.at<Vec3b>(r, c);
		cv::line(image, voronoi_vertex_to_cv_point(v0),
				voronoi_vertex_to_cv_point(v1), color);
	}

}
void voronoi_processing::draw_edges(Mat& image) {
	for (VD::const_edge_iterator it = vd_->edges().begin();
			it != vd_->edges().end(); ++it) {

		if (it->is_primary()) {
			draw_edge(it, image);
		}
	}
}
} /* namespace voronoi_art */
