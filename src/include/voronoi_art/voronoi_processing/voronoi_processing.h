/*
 * voronoi_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_VORONOI_PROCESSING_H_
#define INCLUDE_VORONOI_PROCESSING_H_
#include <opencv/cv.hpp>
#include <boost/polygon/voronoi.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "voronoi_art/image_processing/pixel.h"
#include <memory>
namespace voronoi_art {
using namespace std;
using namespace cv;
using namespace boost::polygon;

typedef voronoi_diagram<double> VD;
typedef double coordinate_type;
typedef point_data<coordinate_type> point_type;
class voronoi_processing {

public:
	typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, point_type, Scalar > delaunay_triangulation;
	//Some typedefs for simplicity
	typedef boost::graph_traits<delaunay_triangulation>::vertex_descriptor delaunay_vertex;
	typedef boost::graph_traits<delaunay_triangulation>::edge_descriptor delaunay_edge;
	typedef  std::shared_ptr<delaunay_triangulation> delaunay_triangulation_ptr;
	voronoi_processing(const Mat& input_image,
			const std::vector<point_type>& site_points, bool extract_delaunay=false);
	virtual ~voronoi_processing();

	void draw_edges(Mat& out_image) const;
	void draw_cells(Mat& image) const;



	void draw_delaunay_edges(Mat& image) const;
	void draw_delaunay_cells(Mat& image) const;

	static cv_float_point voronoi_vertex_to_cv_point(const VD::vertex_type* vertex);
	static point_type cv_point_to_voronoi(const cv_float_point& pt);
	static vector<cv_float_point> voronoi_cell_to_cv_float_polygon(const VD::cell_type cell);
	static vector<cv_int_point> voronoi_cell_to_cv_int_polygon(const VD::cell_type cell);


	std::shared_ptr<const voronoi_art::VD> get_voronoi_diagram() const;
	const std::vector<point_type>& get_site_points() const;

protected:
	void add_delaunay_half_segments(VD::const_cell_iterator& it);
	void extract_delaunay_triangulation();
	virtual void draw_edge(const voronoi_art::VD::edge_type& edge, Mat& image) const;
	virtual void draw_cell(voronoi_art::VD::const_cell_iterator& cell, vector<Pixel>& pixels_copy, Mat& image) const;

	delaunay_triangulation_ptr delaunay_triangulation_;
	std::shared_ptr<voronoi_art::VD> vd_;
	std::vector<point_type> site_points_;
	std::vector<Pixel> input_image_pixels_;
	std::map<unsigned int, std::vector<Pixel> > cell_map_;
	Mat input_image_;
};

} /* namespace voronoi_art */

#endif /* INCLUDE_VORONOI_PROCESSING_H_ */
