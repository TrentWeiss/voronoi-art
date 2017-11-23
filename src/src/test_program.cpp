#include <image_processing/image_processing.h>
#include <boost/filesystem.hpp>
#include <boost/polygon/voronoi.hpp>
namespace fs = boost::filesystem;
using namespace cv;
using namespace voronoi_art;
using namespace std;

int main(int argc, char** argv) {
	std::string image_name;
	if (argc > 1) {
		image_name = argv[1];
	}
	else{
		image_name = "eagle.jpg";
	}
    fs::path full_path = fs::path("..") / fs::path("data")  / fs::path(image_name);
	Mat image;
	image = imread(full_path.string(), IMREAD_UNCHANGED); // Read the file
	if (image.empty())                      // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image);                // Show our image inside it.


	const char* window_name = "Sobel Demo - Simple Edge Detector";
	Mat grad = image_processing::image_gradient(image);
	imshow(window_name, grad);
	cout << "Type of input image: " << image_processing::type2str(image.type()) << endl;
	Vec3b vec = image.at<Vec3b>(605,652);
	uchar r = vec.val[0];
	uchar g = vec.val[1];
	uchar b = vec.val[2];
    cout<<"Type of gradient: " << image_processing::type2str(grad.type()) << endl;
	cout<<"(r, g, b) of input at (605,652)"<< (unsigned int)r << ", " << (unsigned int)g << ", " << (unsigned int)b << endl;
	Scalar intensity = grad.at<uchar>(605,652);
	cout<<"Intensity of gradient at (605,652): "<< intensity.val[0] << endl;

	waitKey(0); // Wait for a keystroke in the window
	return 0;

}
