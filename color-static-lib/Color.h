// (c) 2021 Nithisha Nantha Kumar
// This code is licensed under MIT license (see LICENSE.md for details)

#pragma once

#ifndef Color_H
#define Color_H

#include "opencv2/opencv.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

using namespace std;
using namespace cv;

#define COLOR_BRIGHTNESS_RANGE 110
#define COLOR_DIFFERENCE_RANGE 220
#define IMAGE_DATASET_SIZE 1195
#define K 10

struct pixel {
	int red;
	int green;
	int blue;
};

class color {

private:

	Mat source_img; // Source Image Pixel Matrix
	vector<pixel> palette; // Color palette of the source image
	vector<int> indices; // Indices of each pixel in the color palette
	vector<int> labels; // Labels for performing k-means

	//EFFECTS: returns true if the pixel is present in the palette & 
	//         false otherwise
	bool isPresent(pixel& ob);

	//EFFECTS: calculates & returns the brightness of the pixel's color
	double calculate_brightness(pixel& ob);

	//EFFECTS: calculates & returns the brightness difference of 
	//         the 2 pixels' colors
	double calculate_brightness_difference(pixel& ob1, pixel& ob2);

	//EFFECTS: calculates & returns the color difference of 
	//         the 2 pixels' colors
	double calculate_color_difference(pixel& ob1, pixel& ob2);

	//EFFECTS: returns true if an object of ob1's color is visible when 
	//         placed over an object of ob2's color or vice versa. 
	// (Color Visiblity Algorithm: https://www.w3.org/TR/AERT/#color-contrast)
	bool areColorsVisible(pixel& ob1, pixel& ob2);

	//MODIFIES: source_img
	//EFFECTS: opens a random image from the dataset
	void open_image();

	//MODIFIES: labels, source_img
	//EFFECTS: performs k-means clustering to reduce the color of the image
	// Refer to: https://t.ly/Xti4
	void reduce_color();

	//MODIFIES: palette
	//EFFECTS: stores the colors of the image in palette
	void get_palette();

	//MODIFIES: indices
	//EFFECTS: returns a random visible color combination from the palette
	vector<pixel> get_color_combo();

public:

	//MODIFIES: source_img, palette, indices, labels
	//EFFECTS: returns visible color combination 
	vector<pixel> generate_color_combo();
	
};
#endif // Color_H