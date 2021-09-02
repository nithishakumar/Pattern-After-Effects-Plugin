// (c) 2021 Nithisha Nantha Kumar
// This code is licensed under MIT license (see LICENSE.md for details)


#include "Draw.h"


/* All tiling functions start by drawing a polygon at the center of the output.
   This is the originPolygon. 
   1st and 2nd while loop: Polygons are attached to the left and right side
	                       of the origin polygon. 
   3rd while loop: polygons are attached diagonally upward to the originPolygon. 
	               Each polygon attached is the centerPolygon. The inner loops 
				   attach shapes to the left and right sides of the centerPolygon.
   The 4th while loop performs the same functions as the 3rd but attaches polygons
   diagonally downward to the originPolygon.
*/

void draw::regular_tiling(int numSides) {
	// represents sides to attach shapes to
	int a, b, c, d;

	if (numSides == 6) {
		// hexagonal tiling
		a = 2, b = 5, c = 1, d = 4;
	}
	else {
		// square tiling
		a = 1, b = 3, c = 2, d = 4;
	}

	draw_ngon(numSides);
	draw_ngon_star(numSides);
	originPolygon = vertices;
	originPolygonVertices = vertices;

	while (attach_shape(numSides, numSides, a)) {
		draw_ngon_star(numSides);
	}
	mode = 3;
	while (attach_shape(numSides, numSides, b)) {
		draw_ngon_star(numSides);
	}

	originPolygonVertices = originPolygon;

	while (attach_shape(numSides, numSides, c)) {
		draw_ngon_star(numSides);
		mode = 2;
		centerPolygonVertices = originPolygonVertices;
		while (attach_shape(numSides, numSides, a)) {
			draw_ngon_star(numSides);
		}
		centerPolygonVertices = originPolygonVertices;
		while (attach_shape(numSides, numSides, b)) {
			draw_ngon_star(numSides);
		}
		mode = 3;
	}

	originPolygonVertices = originPolygon;

	while (attach_shape(numSides, numSides, d)) {
		draw_ngon_star(numSides);
		mode = 2;
		centerPolygonVertices = originPolygonVertices;
		while (attach_shape(numSides, numSides, a)) {
			draw_ngon_star(numSides);
		}
		centerPolygonVertices = originPolygonVertices;
		while (attach_shape(numSides, numSides, b)) {
			draw_ngon_star(numSides);
		}
		mode = 3;
	}

	mode = 1;
}


void draw::four_eight_eight() {
	draw_ngon(8);
	draw_ngon_star(8);
	originPolygon = vertices;
	attach_shape(4, 8, 2);
	draw_ngon_star(4);
	vertices = originPolygon;
	attach_shape(8, 8, 3);
	draw_ngon_star(8);
	centerPolygon = vertices;
	attach_shape(4, 8, 2);
	draw_ngon_star(4);
	vertices = centerPolygon;

	while (attach_shape(8, 8, 3)) {
		draw_ngon_star(8);
		originPolygonVertices = vertices;
		mode = 3;
		attach_shape(4, 8, 2);
		draw_ngon_star(4);
		mode = 1;
	}
	vertices = centerPolygon;

	while (attach_shape(8, 8, 1)) {
		draw_ngon_star(8);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 3;
		originPolygonVertices = vertices;
		attach_shape(4, 8, 2);
		draw_ngon_star(4);
		mode = 2;
		while (attach_shape(8, 8, 3)) {
			draw_ngon_star(8);
			originPolygonVertices = centerPolygonVertices;
			mode = 3;
			attach_shape(4, 8, 2);
			draw_ngon_star(4);
			mode = 2;
		}
		centerPolygonVertices = centerPolygon;
		mode = 2;
		while (attach_shape(8, 8, 7)) {
			draw_ngon_star(8);
			originPolygonVertices = centerPolygonVertices;
			mode = 3;
			attach_shape(4, 8, 2);
			draw_ngon_star(4);
			mode = 2;
		}
		mode = 1;
	}

	mode = 1;
	vertices = originPolygon;
	attach_shape(8, 8, 7);
	draw_ngon_star(8);
	centerPolygon = vertices;
	attach_shape(4, 8, 2);
	draw_ngon_star(4);
	vertices = centerPolygon;

	while (attach_shape(8, 8, 7)) {
		draw_ngon_star(8);
		originPolygonVertices = vertices;
		mode = 3;
		attach_shape(4, 8, 2);
		draw_ngon_star(4);
		mode = 1;
	}
	vertices = centerPolygon;
	while (attach_shape(8, 8, 5)) {
		draw_ngon_star(8);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 3;
		originPolygonVertices = vertices;
		attach_shape(4, 8, 2);
		draw_ngon_star(4);
		mode = 2;
		while (attach_shape(8, 8, 3)) {
			draw_ngon_star(8);
			originPolygonVertices = centerPolygonVertices;
			mode = 3;
			attach_shape(4, 8, 2);
			draw_ngon_star(4);
			mode = 2;
		}
		centerPolygonVertices = centerPolygon;
		mode = 2;
		while (attach_shape(8, 8, 7)) {
			draw_ngon_star(8);
			originPolygonVertices = centerPolygonVertices;
			mode = 3;
			attach_shape(4, 8, 2);
			draw_ngon_star(4);
			mode = 2;
		}
		attach_shape(4, 8, 2);
		draw_ngon_star(4);
		mode = 1;
	}
}

void draw::four_six_twelve_inner_loop(bool left) {

	bool attachShapeValue = true;
	int counter = 0;
	// represents sides to attach shapes to
	int a, b;

	// implements right loop
	if (!left) {
		a = 1, b = 4;
	}
	// implements left loop
	else {
		a = 3, b = 10;
	}

	while (counter < 2) {
		attachShapeValue = attach_shape(12, 4, a);
		originPolygonVertices.clear();
		draw_ngon_star(12);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 2;
		originPolygonVertices.push_back(centerPolygonVertices[0]);
		originPolygonVertices.push_back(centerPolygonVertices[11]);
		centerPolygonVertices = centerPolygon;
		attach_shape(6, 12, 11);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(11)]);
		centerPolygonVertices = centerPolygon;
		attach_shape(6, 12, 1);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(0)]);
		centerPolygonVertices = centerPolygon;
		attach_shape(4, 12, 2);
		draw_ngon_star(4);
		mode = 1;
		attach_shape(4, 12, b);
		draw_ngon_star(4);
		mode = 3;
		draw_shape();
		draw_ngon_star(4);
		mode = 1;
		if (!attachShapeValue) {
			counter++;
		}
	
	}
	

}

void draw::four_six_twelve() {
	draw_ngon(12);
	draw_ngon_star(12);
	originPolygon = vertices;
	attach_shape(4, 12, 12);
	draw_ngon_star(4);
	vertices = originPolygon;
	attach_shape(6, 12, 1);
	draw_ngon_star(6);
	vertices = originPolygon;
	attach_shape(6, 12, 11);
	draw_ngon_star(6);
	vertices = originPolygon;

	attach_shape(4, 12, 10);
	draw_ngon_star(4);
	four_six_twelve_inner_loop(false);
	vertices = originPolygon;

	attach_shape(4, 12, 4);
	draw_ngon_star(4);
	four_six_twelve_inner_loop();

	vertices = originPolygon;
	std::vector<point> temp;

	while (attach_shape(4, 12, 2)) {
		draw_ngon_star(4);
		attach_shape(12, 4, 1);
		draw_ngon_star(12);
		originPolygonVertices.clear();
		temp = vertices;
		centerPolygonVertices = temp;
		centerPolygon = temp;
		mode = 2;
		originPolygonVertices.push_back(centerPolygonVertices[0]);
		originPolygonVertices.push_back(centerPolygonVertices[11]);
		centerPolygonVertices = temp;
		attach_shape(6, 12, 11);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(11)]);
		centerPolygonVertices = temp;
		attach_shape(6, 12, 1);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(0)]);
		mode = 3;
		draw_shape();
		draw_ngon_star(4);
		vertices = temp;
		mode = 1;
		attach_shape(4, 12, 10);
		draw_ngon_star(4);
		four_six_twelve_inner_loop(false);
		vertices = temp;
		attach_shape(4, 12, 4);
		draw_ngon_star(4);
		four_six_twelve_inner_loop();
		
		vertices = temp;
	}

	vertices = originPolygon;
	while (attach_shape(4, 12, 8)) {
		draw_ngon_star(4);
		attach_shape(12, 4, 3);
		draw_ngon_star(12);
		originPolygonVertices.clear();
		temp = vertices;
		centerPolygonVertices = temp;
		centerPolygon = temp;
		mode = 2;
		originPolygonVertices.push_back(centerPolygonVertices[0]);
		originPolygonVertices.push_back(centerPolygonVertices[11]);
		centerPolygonVertices = temp;
		attach_shape(6, 12, 11);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(11)]);
		centerPolygonVertices = temp;
		attach_shape(6, 12, 1);
		draw_ngon_star(6);
		originPolygonVertices.push_back(
			centerPolygonVertices[find_vertex_from_center_ngon(0)]);
		mode = 3;
		draw_shape();
		draw_ngon_star(4);
		vertices = temp;
		mode = 1;
		attach_shape(4, 12, 10);
		draw_ngon_star(4);
		four_six_twelve_inner_loop(false);
		vertices = temp;
		attach_shape(4, 12, 4);
		draw_ngon_star(4);
		four_six_twelve_inner_loop();
		vertices = temp;
	}
}

void draw::three_twelve_twelve() {

	draw_ngon(12);
	draw_ngon_star(12);
	originPolygon = vertices;
	attach_shape(3, 12, 1);
	draw_ngon_star(3);
	vertices = originPolygon;
	attach_shape(3, 12, 11);
	draw_ngon_star(3);
	vertices = originPolygon;
	std::vector<point> temp;

	while (attach_shape(12, 12, 10)) {
		draw_ngon_star(12);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 2;
		attach_shape(3, 12, 1);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		attach_shape(3, 12, 11);
		draw_ngon_star(3);
		mode = 1;
	}
	vertices = originPolygon;
	while (attach_shape(12, 12, 4)) {
		draw_ngon_star(12);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 2;
		attach_shape(3, 12, 1);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		attach_shape(3, 12, 11);
		draw_ngon_star(3);
		mode = 1;
	}

	vertices = originPolygon;

	while (attach_shape(12, 12, 2)) {
		draw_ngon_star(12);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 2;
		attach_shape(3, 12, 1);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		attach_shape(3, 12, 11);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		while (attach_shape(12, 12, 10)) {
			draw_ngon_star(12);
			temp = centerPolygonVertices;
			attach_shape(3, 12, 1);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
			attach_shape(3, 12, 11);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
		}
		vertices = centerPolygon;
		while (attach_shape(12, 12, 4)) {
			draw_ngon_star(12);
			temp = centerPolygonVertices;
			attach_shape(3, 12, 1);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
			attach_shape(3, 12, 11);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
		}
		mode = 1;
	}

	vertices = originPolygon;

	while (attach_shape(12, 12, 8)) {
		draw_ngon_star(12);
		centerPolygon = vertices;
		centerPolygonVertices = vertices;
		mode = 2;
		attach_shape(3, 12, 1);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		attach_shape(3, 12, 11);
		draw_ngon_star(3);
		centerPolygonVertices = centerPolygon;
		while (attach_shape(12, 12, 10)) {
			draw_ngon_star(12);
			temp = centerPolygonVertices;
			attach_shape(3, 12, 1);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
			attach_shape(3, 12, 11);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
		}
		vertices = centerPolygon;
		while (attach_shape(12, 12, 4)) {
			draw_ngon_star(12);
			temp = centerPolygonVertices;
			attach_shape(3, 12, 1);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
			attach_shape(3, 12, 11);
			draw_ngon_star(3);
			centerPolygonVertices = temp;
		}
		mode = 1;
	}

}