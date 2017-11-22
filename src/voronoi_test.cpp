#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/polygon/voronoi.hpp>
using namespace boost::polygon;
namespace boost{
namespace polygon{
struct Point {
  int a;
  int b;
  Point (int x, int y) : a(x), b(y) {}
};
template <>
struct geometry_concept<Point> { typedef point_concept type; };

template <>
struct point_traits<Point> {
  typedef double coordinate_type;

  static inline coordinate_type get(const Point& point, orientation_2d orient) {
    return (orient == HORIZONTAL) ? point.a : point.b;
  }
};
}
}
int iterate_primary_edges1(const voronoi_diagram<double>& vd) {
  int result = 0;
  for (voronoi_diagram<double>::const_edge_iterator it = vd.edges().begin();
       it != vd.edges().end(); ++it) {
	  if (it->is_primary())
		  result++;
    if (it->is_primary() && it->vertex0()!=NULL)
    {
    	std::cout<< "(" << it->vertex0()->x() <<", "<<it->vertex0()->y()<<")" <<std::endl;
    }
  }
  return result;
}
int main(int argc, char** argv) {
	std::vector<Point> points;
	points.push_back(Point(0.0, 0.0));
	points.push_back(Point(1.0, 6.0));
	points.push_back(Point(4.0, 8.0));
	points.push_back(Point(-9.0, 2.0));
	voronoi_diagram<double> vd;
	construct_voronoi(points.begin(), points.end(),&vd);
	std::cout<<"vd contains: " << vd.edges().size() <<  " edges."<<std::endl;
	int res = iterate_primary_edges1(vd);
	std::cout<<"found " << res <<" primary edges" << std::endl;
	return 0;
}
