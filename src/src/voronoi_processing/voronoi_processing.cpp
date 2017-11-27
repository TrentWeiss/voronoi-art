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
namespace voronoi_art {

voronoi_processing::voronoi_processing(const Mat& input_image,
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

point_type voronoi_processing::cv_point_to_voronoi(const Point& pt) {
	return point_type(pt.x, pt.y);
}

Point voronoi_processing::voronoi_vertex_to_cv_point(
		const VD::vertex_type* vertex) {
	int x = std::round(vertex->x());
	int y = std::round(vertex->y());
	return Point(x, y);
}

const voronoi_art::VD* voronoi_processing::get_voronoi_diagram() const {
	return vd_;
}

const std::vector<point_type>& voronoi_processing::get_site_points() const {
	return site_points_;
}
inline double euclidean_distance(const point_type& a, const point_type& b) {
	return std::sqrt(std::pow(a.x()- b.x(),2) + std::pow(a.y()- b.y(),2));
}
bool sort_functor(const point_type& a, const point_type& b,
		const point_type& reference_point) {
	return euclidean_distance(a,reference_point) < euclidean_distance(b,reference_point);
}
void voronoi_processing::draw_cell(voronoi_art::VD::const_cell_iterator& cell,
		Mat& image) const {
	vector<point_type> site_points_copy(site_points_);

	std::vector<Scalar> input_scalars;
	unsigned int source_index = cell->source_index();
	for(unsigned int r = 0; r < image.rows; r++){
		for(unsigned int c = 0; c < image.cols; c++){
			point_type temp((double)c,(double)r);

		}
	}
	image.at<Vec3b>(0,0);
	std::min_element(site_points_copy.begin(), site_points_copy.end(),
			boost::bind(sort_functor, _1, _2,
					site_points_[cell->source_index()]));
}
void voronoi_processing::draw_cells(Mat& image) const {
	for (VD::const_cell_iterator it = vd_->cells().begin();
			it != vd_->cells().end(); ++it) {

		if (it->contains_point() and (not it->is_degenerate())) {
			draw_cell(it, image);
		}
	}
}

void voronoi_processing::draw_edge(voronoi_art::VD::const_edge_iterator& edge,
		Mat& image) const {

	const VD::vertex_type* v0 = edge->vertex0();
	const VD::vertex_type* v1 = edge->vertex1();
	Scalar color;
	if (v0 && v1) {
		int r = site_points_[edge->cell()->source_index()].y();
		int c = site_points_[edge->cell()->source_index()].x();
		color = input_image_.at<Vec3b>(r, c);
		line(image, voronoi_vertex_to_cv_point(v0),
				voronoi_vertex_to_cv_point(v1), color);
	}

}
void voronoi_processing::draw_edges(Mat& image) const {
	for (VD::const_edge_iterator it = vd_->edges().begin();
			it != vd_->edges().end(); ++it) {

		if (it->is_primary() and (not it->is_infinite())) {
			draw_edge(it, image);
		}
	}
}
} /* namespace voronoi_art */
