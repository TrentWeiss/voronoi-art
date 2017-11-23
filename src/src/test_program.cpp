#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/polygon/voronoi.hpp>
namespace fs = boost::filesystem;
using namespace cv;
using namespace std;
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
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

	Mat src_gray;
	Mat grad;
	const char* window_name = "Sobel Demo - Simple Edge Detector";
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	cvtColor(image, src_gray, CV_BGR2GRAY);
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	imshow(window_name, grad);
	cout << "Type of greyscale image: " << type2str(src_gray.type()) << endl;
	cout << "Type of input image: " << type2str(image.type()) << endl;
	Vec3b vec = image.at<Vec3b>(605,652);
	uchar r = vec.val[0];
	uchar g = vec.val[1];
	uchar b = vec.val[2];
    cout<<"Type of gradient: " << type2str(grad.type()) << endl;
	cout<<"(r, g, b) of input at (605,652)"<< (unsigned int)r << ", " << (unsigned int)g << ", " << (unsigned int)b << endl;
	Scalar intensity = grad.at<uchar>(605,652);
	cout<<"Intensity of gradient at (605,652): "<< intensity.val[0] << endl;

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
