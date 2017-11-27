/*
 * voronoi_processing.h
 *
 *  Created on: Nov 22, 2017
 *      Author: trent
 */

#ifndef INCLUDE_VORONOI_PROCESSING_H_
#define INCLUDE_VORONOI_PROCESSING_H_
#include <voronoi_art/image_processing/image_processing.h>
#include <boost/polygon/voronoi.hpp>
#ifdef USE_STD_FUNCTION
#include <functional>
#else
#include <boost/function.hpp>
#endif
namespace voronoi_art {
using namespace boost::polygon;

typedef voronoi_diagram<double> VD;
typedef double coordinate_type;
typedef point_data<coordinate_type> point_type;
class voronoi_processing {
public:
#ifdef USE_STD_FUNCTION
	typedef std::function<void (VD::const_edge_iterator& edge, const VD& vd_)> iterate_edges_func;
#else
	typedef boost::function<void(VD::const_edge_iterator& edge, const VD& vd_)> iterate_edges_func;
#endif

	voronoi_processing(const cv::Mat& input_image,
			const std::vector<point_type>& site_points);

	virtual ~voronoi_processing();
	void draw_edges(Mat& out_image);

	void draw_cells(cv::Mat& image);
	static cv::Point voronoi_vertex_to_cv_point(const VD::vertex_type* vertex);
	static point_type cv_point_to_voronoi(const cv::Point& pt);
	const voronoi_art::VD* get_voronoi_diagram() const;
	const std::vector<point_type>& get_site_points() const;

private:
	void draw_edge(voronoi_art::VD::const_edge_iterator& edge, cv::Mat& image);
	void draw_cell(voronoi_art::VD::const_cell_iterator& cell, cv::Mat& image);

	voronoi_art::VD* vd_;
	std::vector<point_type> site_points_;
	cv::Mat input_image_;
};

} /* namespace voronoi_art */

#endif /* INCLUDE_VORONOI_PROCESSING_H_ */
