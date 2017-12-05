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
#include <iostream>
#include <Eigen/Dense>
namespace voronoi_art {
struct point_slope {
	Eigen::Vector2d x;
	double y;
};
typedef struct point_slope point_slope_t;
voronoi_processing::voronoi_processing(const Mat& input_image,
		const std::vector<point_type>& site_points, bool extract_delaunay) {
	vd_.reset(new VD);
	input_image_ = input_image.clone();
	input_image_pixels_ = image_processing::image_to_point_vector(input_image_);
	site_points_ = std::vector<point_type>(site_points);
	construct_voronoi(site_points_.begin(), site_points_.end(), vd_.get());
	if (extract_delaunay) {
		delaunay_triangulation_.reset(new delaunay_triangulation);
		extract_delaunay_triangulation();
	}
}
voronoi_processing::~voronoi_processing() {
	// TODO Auto-generated destructor stub
}
//Eigen::Vector2d po
void voronoi_processing::add_delaunay_half_segments(
		VD::const_cell_iterator& it) {
	point_type site_point = point_type(site_points_[it->source_index()]);
	voronoi_processing::delaunay_vertex u = boost::add_vertex<
			voronoi_processing::delaunay_triangulation>(
			*delaunay_triangulation_);
	(*delaunay_triangulation_)[u] = site_point;
	const voronoi_edge<VD::coordinate_type>* edge = it->incident_edge();
	do {
		edge = edge->next();
		const voronoi_vertex<VD::coordinate_type>* v0 = edge->vertex0();
		const voronoi_vertex<VD::coordinate_type>* v1 = edge->vertex1();
		if (v0 and v1) {
			point_type p0(v0->x(), (v0->y()));
			point_type p1(v1->x(), (v1->y()));
			point_type intersection;
			if (p1.x() == p0.x()) {
				//edge is a vertical line segment.
			     intersection = point_type(p1.x(), site_point.y());
			} else if (p1.y() == p0.y()) {
				//edge is a horizontal line segment.
			     intersection = point_type(site_point.x(), p1.y());
			} else {
				/*
				 * intersection is the solution to a system of linear equations
				 * y-y_p = m * (x - x_p)
				 * y-y_p = m*x - m*x_p
				 * -m*x + y = y_p - m*x_p
				 *
				 */
				Eigen::Vector2d X, Y;
				Eigen::Matrix2d A = Eigen::Matrix2d::Zero();
				double m_edge = (p1.y() - p0.y()) / (p1.x() - p0.x());
				double m_line = -1.0 / m_edge;
				A(0, 0) = -m_edge;
				A(0, 1) = 1.0;
				Y[0] = p0.y() - m_edge * p0.x();

				A(1, 0) = -m_line;
				A(1, 1) = 1.0;
				Y[1] = site_point.y() - m_line * site_point.x();
				X = A.inverse() * Y;
				intersection = point_type(X.x(), X.y());
			}
			voronoi_processing::delaunay_vertex v = boost::add_vertex<
					voronoi_processing::delaunay_triangulation>(
					*delaunay_triangulation_);
			(*delaunay_triangulation_)[v] = intersection;

			delaunay_edge e;
			bool b;
			boost::tie(e, b) = boost::add_edge<
					voronoi_processing::delaunay_triangulation>(u, v,
					*delaunay_triangulation_);
			(*delaunay_triangulation_)[e] = input_image_.at<Vec3b>(
					cv_float_point(site_point.x(), site_point.y()));
		}
	} while (edge != it->incident_edge());
}
void voronoi_processing::extract_delaunay_triangulation() {

	for (VD::const_cell_iterator it = vd_->cells().begin();
			it != vd_->cells().end(); ++it) {

		if ((not it->is_degenerate())) {
			add_delaunay_half_segments(it);
		}
	}
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

std::shared_ptr<const voronoi_art::VD> voronoi_processing::get_voronoi_diagram() const {
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
	const voronoi_edge<VD::coordinate_type>* edge = cell.incident_edge();
	do {
		edge = edge->next();
		const voronoi_vertex<VD::coordinate_type>* v0 = edge->vertex0();
		if (!v0) {
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
		const voronoi_vertex<double>* v0 = edge->vertex0();
		if (!v0) {
			continue;
		}
		rtn.push_back(voronoi_vertex_to_cv_point(edge->vertex0()));
	} while (edge != cell.incident_edge());
	return rtn;
}
Scalar average_color(const vector<Scalar>& colors) {
	unsigned long r = 0, g = 0, b = 0;
	for (const Scalar& color : colors) {
		r += color[0];
		g += color[1];
		b += color[2];
	}
	float size = (float) colors.size();
	r = std::round((float) r / size);
	g = std::round((float) g / size);
	b = std::round((float) b / size);
	return Scalar(r, g, b, 1.0);
}
vector<cv_int_point> extractIntPoints(const vector<Pixel>& pixels) {
	vector<cv_int_point> rtn(pixels.size());
	for (unsigned int i = 0; i < rtn.size(); ++i) {
		rtn[i] = pixels[i].get_int_point();
	}
	return rtn;
}

void voronoi_processing::draw_cell(voronoi_art::VD::const_cell_iterator& cell,
		vector<Pixel>& pixels_copy, Mat& image) const {
	vector<cv_int_point> cv_poly_int = voronoi_cell_to_cv_int_polygon(*cell);
	point_type pt = site_points_[cell->source_index()];
	Mat mask(image.size(),CV_8U, Scalar::all(0.0));
	cv::fillConvexPoly(mask, &cv_poly_int[0], cv_poly_int.size(), Scalar(255.0,0.0,0.0,1.0));
	Scalar color = cv::mean(input_image_,mask);

	cv::fillConvexPoly(image, &cv_poly_int[0], cv_poly_int.size(), color);

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

void voronoi_processing::draw_edge(const voronoi_art::VD::edge_type& edge,
		Mat& image) const {

	const VD::vertex_type* v0 = edge.vertex0();
	Scalar color;
	if (v0 and edge.next()->vertex0()) {
		int r = std::round(site_points_[edge.cell()->source_index()].y());
		int c = std::round(site_points_[edge.cell()->source_index()].x());
		color = input_image_.at<Vec3b>(r, c);

		line(image, voronoi_vertex_to_cv_point(v0),
				voronoi_vertex_to_cv_point(edge.next()->vertex0()), color);
	}

}
void voronoi_processing::draw_edges(Mat& image) const {
	for (VD::const_edge_iterator it = vd_->edges().begin();
			it != vd_->edges().end(); ++it) {

		if (it->is_primary() and (not it->is_infinite())) {
			draw_edge(*it, image);
		}
	}
}

void voronoi_processing::draw_delaunay_edges(Mat& image) const {

	std::pair<boost::graph_traits<delaunay_triangulation>::edge_iterator,
			boost::graph_traits<delaunay_triangulation>::edge_iterator> range =
			boost::edges(*delaunay_triangulation_);
	boost::graph_traits<delaunay_triangulation>::edge_iterator it = range.first;
	boost::graph_traits<delaunay_triangulation>::edge_iterator end =
			range.second;
	while (it != end) {
		point_type a = (*delaunay_triangulation_)[it->m_source];
		point_type b = (*delaunay_triangulation_)[it->m_target];

		cv_float_point acv(a.x(), a.y());
		cv_float_point bcv(b.x(), b.y());
		Scalar* color = (Scalar*) (it->m_eproperty);
		line(image, acv, bcv, *color);
		it++;
	}
}
voronoi_processing::delaunay_triangulation_ptr voronoi_processing::get_delaunay_triangulation() const {
	return delaunay_triangulation_ptr(delaunay_triangulation_);
}

void voronoi_processing::draw_delaunay_cells(Mat& image) const {
}
} /* namespace voronoi_art */

