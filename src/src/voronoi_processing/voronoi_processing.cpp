/*
 * voronoi_processing.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#include "voronoi_art/voronoi_processing/voronoi_processing.h"
#include "voronoi_art/image_processing/image_processing.h"
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
	input_image_pixels_ = image_processing::image_to_point_vector(input_image_);
	site_points_ = std::vector<point_type>(site_points);
	construct_voronoi(site_points_.begin(), site_points_.end(), vd_);
}
voronoi_processing::~voronoi_processing() {
	// TODO Auto-generated destructor stub
	delete vd_;
}

point_type voronoi_processing::cv_point_to_voronoi(const cv_float_point& pt) {
	return point_type(pt.x, pt.y);
}

cv_float_point voronoi_processing::voronoi_vertex_to_cv_point(
		const VD::vertex_type* vertex) {
	float x = (float) vertex->x();
	float y = (float) vertex->y();
	return cv_float_point(x, y);
}

const voronoi_art::VD* voronoi_processing::get_voronoi_diagram() const {
	return vd_;
}

const std::vector<point_type>& voronoi_processing::get_site_points() const {
	return site_points_;
}
inline double euclidean_distance(const point_type& a, const point_type& b) {
	return std::sqrt(std::pow(a.x() - b.x(), 2) + std::pow(a.y() - b.y(), 2));
}
bool sort_functor(const point_type& a, const point_type& b,
		const point_type& reference_point) {
	return euclidean_distance(a, reference_point)
			< euclidean_distance(b, reference_point);
}
vector<cv_int_point> voronoi_processing::voronoi_cell_to_cv_int_polygon(
		const VD::cell_type cell) {

	vector<cv_int_point> rtn;
	const voronoi_edge<double>* edge = cell.incident_edge();
	do {
		edge = edge->next();
		const voronoi_vertex<double>*  v0 = edge->vertex0();
		if(!v0){
			continue;
		}
		cv_float_point pt = voronoi_vertex_to_cv_point(edge->vertex0());
		rtn.push_back(cv_int_point(std::round(pt.x), std::round(pt.y)));
	} while (edge != cell.incident_edge());
	return rtn;
}
vector<cv_float_point> voronoi_processing::voronoi_cell_to_cv_float_polygon(
		const VD::cell_type cell) {

	vector<cv_float_point> rtn;
	const voronoi_edge<double>* edge = cell.incident_edge();
	do {
		edge = edge->next();
		const voronoi_vertex<double>*  v0 = edge->vertex0();
		if(!v0){
			continue;
		}
		rtn.push_back(voronoi_vertex_to_cv_point(edge->vertex0()));
	} while (edge != cell.incident_edge());
	return rtn;
}
Scalar average_color(const vector<Scalar>& colors){
	unsigned long r=0, g=0, b=0;
	for(const Scalar& color: colors){
		r+=color[0];
		g+=color[1];
		b+=color[2];
	}
	float size = (float)colors.size();
	r=std::round((float)r/size);
	g=std::round((float)g/size);
	b=std::round((float)b/size);
	return Scalar(r, g, b, 1.0);
}
vector<cv_int_point> extractIntPoints(const vector<Pixel>& pixels){
	vector<cv_int_point> rtn(pixels.size());
	for(unsigned int i = 0; i < rtn.size(); ++i){
		rtn[i]=pixels[i].get_int_point();
	}
	return rtn;
}
void voronoi_processing::draw_cell(voronoi_art::VD::const_cell_iterator& cell,
		vector<Pixel>& pixels_copy, Mat& image) const {
	vector<cv_int_point> cv_poly_int = voronoi_cell_to_cv_int_polygon(*cell);
	point_type pt = site_points_[cell->source_index()];

	Scalar color = input_image_.at<Vec3b>(std::round(pt.y()),std::round(pt.x()));

	cv::fillConvexPoly(image,&cv_poly_int[0],cv_poly_int.size(),color);

}
void voronoi_processing::draw_cells(Mat& image) const {

	vector<Pixel> pixels_copy(input_image_pixels_);
	for (VD::const_cell_iterator it = vd_->cells().begin();
			it != vd_->cells().end(); ++it) {

		if (it->contains_point() and (not it->is_degenerate())) {

			draw_cell(it, pixels_copy, image);
		}
	}
}

void voronoi_processing::draw_edge(voronoi_art::VD::const_edge_iterator& edge,
		Mat& image) const {

	const VD::vertex_type* v0 = edge->vertex0();
	Scalar color;
	if (v0) {
		int r = std::round(site_points_[edge->cell()->source_index()].y());
		int c = std::round(site_points_[edge->cell()->source_index()].x());
		color = input_image_.at<Vec3b>(r, c);
		line(image, voronoi_vertex_to_cv_point(v0),
				voronoi_vertex_to_cv_point(edge->next()->vertex0()), color);
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
