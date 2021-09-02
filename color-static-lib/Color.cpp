// (c) 2021 Nithisha Nantha Kumar
// This code is licensed under MIT license (see LICENSE.md for details)

#include "pch.h"
#include "framework.h"
#include "Color.h"


bool color::isPresent(pixel& ob) {
	for (int i = int(palette.size()) - 1; i >= 0; i--) {
		if (palette[i].red == ob.red &&
			palette[i].green == ob.green &&
			palette[i].blue == ob.blue) {
			return true;
		}
	}

	return false;
}

double color::calculate_brightness(pixel& ob) {
	return (ob.red * 299 + ob.green * 587 + ob.blue * 114) / 1000.0;
}
double color::calculate_brightness_difference(pixel& ob1, pixel& ob2) {
	return fabs(calculate_brightness(ob1) - calculate_brightness(ob2));
}

double color::calculate_color_difference(pixel& ob1, pixel& ob2) {
	return (max(ob1.red, ob2.red) - min(ob1.red, ob2.red)) +
		(max(ob1.green, ob2.green) - min(ob1.green, ob2.green)) +
		(max(ob1.blue, ob2.blue) - min(ob1.blue, ob2.blue));
}

bool color::areColorsVisible(pixel& ob1, pixel& ob2) {
	return (calculate_brightness_difference(ob1, ob2) > COLOR_BRIGHTNESS_RANGE
		&& calculate_color_difference(ob1, ob2) > COLOR_DIFFERENCE_RANGE);
}

void color::open_image() {
	srand((unsigned)time(0));
	string backslash = "\\";
	int randomNumber = (rand() % IMAGE_DATASET_SIZE) + 1;
	string path = "C:" + backslash + "Users" + backslash + "nithi" +
		           backslash + "Desktop" + backslash + "AfterEffectsSDK" +
		           backslash + "Examples" + backslash + "Template" + backslash +
		           "Pattern Plugin" + backslash + "ImageDataset" + backslash + 
		           "frame (" +  to_string(randomNumber) + ").jpg";
	source_img = imread(path);
}

void color::reduce_color() {
	// Serialize the image
	cv::Mat data = source_img.reshape(1, source_img.total());
	data.convertTo(data, CV_32F);

	// Perform k-means clustering
	labels.clear();
	cv::Mat3f centers;
	cv::kmeans(data, K, labels, cv::TermCriteria(),
		1, cv::KMEANS_PP_CENTERS, centers);

	// Reduce the colors of the image
	for (int i = 0; i < (int)labels.size(); ++i) {
		data.at<float>(i, 0) = centers.at<float>(labels[i], 0);
		data.at<float>(i, 1) = centers.at<float>(labels[i], 1);
		data.at<float>(i, 2) = centers.at<float>(labels[i], 2);
	}

	// De-serialize the image
	cv::Mat destination = data.reshape(3, source_img.rows);
	destination.convertTo(source_img, CV_8UC3);
}

void color::get_palette() {
	palette.clear();
	for (size_t i = 0; i < source_img.rows; i++) {
		for (size_t j = 0; j < source_img.cols; j++) {
			Vec3b color = source_img.at<Vec3b>(i, j);
			pixel obj;
			obj.red = int(color[2]);
			obj.green = int(color[1]);
			obj.blue = int(color[0]);
			if (!isPresent(obj)) {
				palette.push_back(obj);
			}
		}
	}

}

vector<pixel> color::get_color_combo() {

	// Store indices of each color in the palette 
	indices.clear();
	for (int i = 0; i < int(palette.size()); i++) {
		indices.push_back(i);
	}

	srand((unsigned)time(0));
	random_shuffle(palette.begin(), palette.end());
	random_shuffle(indices.begin(), indices.end());

	pixel color1, color2;
	bool indicator = false;

	while (!indicator) {

		// Choose a random color in the palette
		int randomNumber = (rand() % (int(palette.size()) - 1));
		randomNumber = indices[randomNumber];
		random_shuffle(palette.begin(), palette.end());

		for (int i = 0; i < int(palette.size()); i++) {
			// Find another color compatible with the given color from the palette
			if (i != randomNumber &&
				areColorsVisible(palette[i], palette[randomNumber])) {
				color1 = palette[i];
				color2 = palette[randomNumber];
				indicator = true;
				break;
			}

		}
	}

	// Sorting colors in ascending order of their brightness
	if (calculate_brightness(color1) > calculate_brightness(color2)) {
		return { color1, color2 };
	}

	return { color2, color1 };
}

vector<pixel> color::generate_color_combo() {
	open_image();
	reduce_color();
	get_palette();
	return get_color_combo();
}