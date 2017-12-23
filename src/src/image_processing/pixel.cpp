/*
License Agreement 

								3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:



    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    Neither the names of the copyright holders nor the names of the contributors may be used to endorse or promote products derived from this software without specific prior written permission.



This software is provided by the copyright holders and contributors “as is” and any express or implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. 

In no event shall copyright holders or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; 

or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

 
 *
 *  Created on: Nov 27, 2017
 *      Author: Trent Weiss
 */

#include <voronoi_art/image_processing/pixel.h>

namespace voronoi_art {

Pixel::Pixel() {
	// TODO Auto-generated constructor stub

}

Pixel::~Pixel() {
	// TODO Auto-generated destructor stub
}
const cv::Scalar& Pixel::get_color() const {
	return color_;
}

const cv_float_point& Pixel::get_float_point() const {
	return float_point_;
}

const cv_int_point& Pixel::get_int_point() const {
	return int_point_;
}

} /* namespace voronoi_art */


