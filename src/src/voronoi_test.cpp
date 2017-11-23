#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <voronoi_processing/voronoi_processing.h>

using namespace voronoi_art;

int iterate_primary_edges1(const VD& vd) {
  int result = 0;
  for (VD::const_edge_iterator it = vd.edges().begin();
       it != vd.edges().end(); ++it) {

    if (it->is_primary() && it->vertex0()!=NULL && it->vertex1()!=NULL)
    {
    	result++;
    	std::cout<< "(" << it->vertex0()->x() <<", "<<it->vertex0()->y()<<")"
    			", (" << it->vertex1()->x() <<", "<<it->vertex1()->y()<<")"<<std::endl;
    }
  }
  return result;
}

int main(int argc, char* argv[])
{
    std::vector<point_type> points;

    points.push_back(point_type(4, 1));
    points.push_back(point_type(5, 4));
    points.push_back(point_type(5, 5));
    points.push_back(point_type(2, 6));
    points.push_back(point_type(3, 4));
    points.push_back(point_type(0, 7));
    points.push_back(point_type(2, 5));
    points.push_back(point_type(8, 9));
    points.push_back(point_type(0, 4));
    points.push_back(point_type(2, 7));

    VD vd;  construct_voronoi(points.begin(), points.end(), &vd);
    int num_prints = iterate_primary_edges1(vd);
    std::cout<<"Printed out " << num_prints << " points." <<std::endl;
    std::cout<<"VD has " << vd.edges().size() << " edges." <<std::endl;
    return 0;
}
