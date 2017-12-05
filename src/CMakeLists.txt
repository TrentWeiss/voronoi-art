cmake_minimum_required(VERSION 2.8.3)
project(voronoi_art)

add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
set(BUILD_SHARED_LIBS true)
find_package( OpenCV REQUIRED )
find_package( Eigen3 REQUIRED )
find_package( Boost 1.58 COMPONENTS program_options REQUIRED )
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})
add_library(${PROJECT_NAME}_image_processing
 src/image_processing/image_processing.cpp
 src/image_processing/pixel.cpp
)
target_link_libraries(${PROJECT_NAME}_image_processing 
 ${OpenCV_LIBS}
)
add_library(${PROJECT_NAME}_voronoi_processing
 src/voronoi_processing/voronoi_processing.cpp
)
target_link_libraries(${PROJECT_NAME}_voronoi_processing 
 ${PROJECT_NAME}_image_processing 
 ${Boost_LIBRARIES}
)


add_executable(${PROJECT_NAME} src/voronoi_art.cpp)
target_link_libraries(${PROJECT_NAME} 
 ${PROJECT_NAME}_image_processing
 ${PROJECT_NAME}_voronoi_processing
 )

add_executable(${PROJECT_NAME}_video src/voronoi_video.cpp)
target_link_libraries(${PROJECT_NAME}_video 
 ${PROJECT_NAME}_image_processing
 ${PROJECT_NAME}_voronoi_processing
 )

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_voronoi_processing ${PROJECT_NAME}_image_processing
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )
install(DIRECTORY include/voronoi_art DESTINATION include )

