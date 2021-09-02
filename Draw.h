// (c) 2021 Nithisha Nantha Kumar
// This code is licensed under MIT license (see LICENSE.md for details)

#pragma once

#ifndef Draw_H
#define Draw_H

#include "Pattern.h"
#include <vector>
#include <map>

struct point {
	double x;
	double y;
};

// stores an equation in ax + by  = c form
struct equation {
	double a;
	double b;
	double c;
};


class draw {

private:

	std::vector<point> vertices;
	// Stores vertices of the current polygon to which
	// other polygons have to be attached
	std::vector<point> centerPolygon;
	std::vector<point> centerPolygonVertices;
	// Stores vertices of the polygon drawn in the center of
	// the output (first polygon in tiling)
	std::vector<point> originPolygon;
	std::vector<point> originPolygonVertices;
	std::vector<point> midpoints;
	point center;
	cairo_t* cr;
	PF_LayerDef* output;
	// Mode can take values of 1, 2, 3.
	// When mode == 1, calculated vertices
	// are stored in the vertices vector.
	// Similarly, when mode == 2, calculated
	// vertices are stored in centerPolygonVertices
	// and when mode == 3, vertices are stored in
	// originPolygonVertices.
	int mode;

	/*Parameter Values*/
	double sideLength;
	bool hideTiling;
	PF_Pixel bgColor;
	PF_Pixel patternColor;
	double patternWidth;
	int deltaMode;
	double delta;
	double angleInDegrees;
	int numSidesCenterMode;
	int outputMode;
	int tilingChoice;

	//REQUIRES: non-parallel, non-horizontal, non-vertical & non-collinear 
	//          linear equations of 2 lines.
	//EFFECTS: returns the intersection point of both lines by solving 
	//         the equation using determinants.
	point solve_equation(equation& eq1, equation& eq2);

	//EFFECTS: returns an equation in ax + by = c form from
	//         the two points.
	equation construct_equation_from_pts(point& pt1, point& pt2);

	//REQUIRES: angle in radians
	//EFFECTS: returns a point rotated about another point by the given angle.
	point rotatePoint(point& original, point& rotateAbt, double angleInRadians);

	//REQUIRES: 2 linear equations representing non-parallel lines.
	//EFFECTS: returns true if the two lines are collinear & false otherwise.
	bool isCollinear(equation& eq1, equation& eq2);

	//EFFECTS: returns true if the line formed by 2 points is 
	//         horizontal and false otherwise.
	bool isHorizontal(equation& eq);

	//EFFECTS: returns true if the line formed by 2 points is 
	//          vertical and false otherwise.
	bool isVertical(equation& eq);

	//EFFECTS: returns midPoint moved by a distance delta 
	//         along the line formed by v1 and v2.
	point move_along_line(point& v1, point& v2, 
		                  point midPoint, double distance);

	//REQUIRES: the lines formed by each of the two points 
	//          should not be parallel.
	//EFFECTS:  returns the intersection point of two lines.
	point findIntersection(point& eq1pt1, point& eq1pt2, 
		                   point& eq2pt1, point& eq2pt2);

	//REQUIRES: numSides is an integer greater than 2.
	//MODIFIES: vertices
	//EFFECTS:  calculates the coordinates of a polygon's vertices 
    //          given the default center of the polygon & stores it 
	//          in vertices.
	void calculate_center_vertices(int numSides);


	//REQUIRES: numSides is an integer greater than 2.
	//MODIFIES: midpoints
	//EFFECTS: calculates the midpoints of each of the sides
	//         of the polygon & stores it in midpoints.
	void calculate_midpoints(int numSides);

	//REQUIRES: numSides is an integer greater than 2.
	//MODIFIES: cr, midpoints
	//EFFECTS: implements the Hankin's polygons-in-contact algorithm.
	// Refer to: https://dl.acm.org/doi/pdf/10.5555/1089508.1089538
	//           https://www.youtube.com/watch?v=ld4gpQnaziU
	void draw_ngon_star(int numSides);

	//REQUIRES: numSides >= 3
	//MODIFIES: cr, vertices
	//EFFECTS: calculates vertices of the polygon with the default center 
	//         & draws it. 
	void draw_ngon(int numSides);

	//MODIFIES: cr
	//EFFECTS: draws a polygon in the current mode when tiling is not hidden. 
	void draw_shape();

	//EFFECTS: returns the length of the polygon's apothem given the
	//         polygon's radius.
	double findApothem(double radius);

	//REQUIRES: numSides >= 3, numSidesCenter >= 3, 
	//          1 <= side <= numSidesCenter
	//EFFECTS: returns the center of the polygon to be attached to another 
	//         polygon which is referred to as the center polygon.
	point findCenter(point& v0, point& v1, int numSides, 
		             int numSidesCenter, int side);

	//REQUIRES: numSides >= 3, numSidesCenter >= 3, 
	//          1 <= side <= numSidesCenter
	//MODIFIES: vertices, centerPolygonVertices, originPolygonVertices
	//EFFECTS: calculates the rest of the vertices of a polygon 
	//         with 2 known vertices.
	void calculate_vertices_known(point v0, point v1, int numSides, 
		                          int numSidesCenter, int side);

	//MODIFIES: vertices, centerPolygonVertices, originPolygonVertices
    //EFFECTS: sorts the vertices of the polygon in counter-clockwise order.
	void sort_vertices();

	//REQUIRES: numSidesShapeToAttach >= 3, numSidesCenter >= 3, 
	//          1 <= side <= numSidesCenter
	//MODIFIES: vertices, centerPolygonVertices, originPolygonVertices, cr
	//EFFECTS:  attaches a polygon to the given side of another polygon 
	//          which is referred to as the center polygon.
	/**This function only works for sides of the center polygon whose apothem
	   is not vertical and not horizontal**/
	bool attach_shape(int numSidesShapeToAttach, int numSidesCenter, 
		              int side);

	//EFFECTS: returns true if the given point is within the bounds of the output
	//         & false otherwise.
	bool isWithinBounds(point& pt);

	//EFFECTS: returns true if at least one point is within the bounds 
	//         of the output in vertices, centerPolygonVertices, or
	//         originPolygonVertices & false otherwise.
	bool areVerticesBounded();

	//REQUIRES: valid index of point in centerPolygon (this point should
	//          be a vertex of the current polygon that is to be attached to 
	//          the centerPolygon),
	// 	        centerPolygon to contain vertices of a polygon to which 
	//          the current polygon is to be attached,
	// 	        centerPolygonVertices to contain vertices of a polygon already 
	//          attached to the centerPolygon & located adjacent to the current
	//          polygon that is now to be attached to the centerPolygon.
	//EFFECTS:  returns the position of the next vertex (in counter-clockwise 
	//          order) of the current polygon to be attached to the centerPolygon 
	//          from centerPolygonVertices.
    /**This function is useful to calculate the vertices of a polygon that is to
	   be attached to a side of the centerPolygon whose apothem is vertical 
	   or horizontal. It finds vertices of the current polygon from the existing 
	   vertices of its adjacent polygons.**/
	int find_vertex_from_center_ngon(int index);

	//REQUIRES: numSides = 4 || numSides = 6
	//MODIFIES: cr, vertices, originPolygonVertices, 
	//          originPolygon, centerPolygon, 
	//          centerPolygonVertices, midpoints
	//EFFECTS:  implements Hankin's Polygons-in-contact algorithm in 
	//          4.4.4 or 6.6.6 tiling.
	void regular_tiling(int numSides);


	//MODIFIES: cr, vertices, originPolygonVertices, 
	//          originPolygon, centerPolygon, 
	//          centerPolygonVertices, midpoints
	//EFFECTS:  implements Hankin's Polygons-in-contact algorithm in 4.8.8 tiling.
	void four_eight_eight();


	//MODIFIES: cr, vertices, originPolygonVertices, 
	//          originPolygon, centerPolygon, 
	//          centerPolygonVertices, midpoints
	//EFFECTS:  executes an inner loop for implementing Hankin's 
	//          polygons-in-contact algorithm in 4.6.12 tiling .
	void four_six_twelve_inner_loop(bool left = true);


	//MODIFIES: cr, vertices, originPolygonVertices, 
	//          originPolygon, centerPolygon, 
	//          centerPolygonVertices, midpoints
	//EFFECTS:  implements Hankin's Polygons-in-contact algorithm in 4.6.12 tiling.
	void four_six_twelve();

	//MODIFIES: cr, vertices, originPolygonVertices, 
    //          originPolygon, centerPolygon, 
    //          centerPolygonVertices, midpoints
	//EFFECTS:  implements Hankin's Polygons-in-contact algorithm in 3.12.12 tiling.
	void three_twelve_twelve();

	//MODIFIES: cr
	//EFFECTS: sets pattern properties based on selected parameters.
	void set_pattern_properties();

	//MODIFIES: cr 
	//EFFECTS: sets the background color of the composition 
	//         based on selected parameters.
	void set_bg_color();

public:
	
	draw(cairo_t* cr_in, PF_LayerDef* output_in, PF_ParamDef* params[]) 
		: cr(cr_in), output(output_in) {
		center.x = double(output->width) * 0.5;
		center.y = double(output->height) * 0.5;
		outputMode = params[Pattern_CHOOSEMODE]->u.pd.value;
		tilingChoice = params[Pattern_CHOOSETILING]->u.pd.value;
		sideLength = params[Pattern_SIDELENGTH]->u.fs_d.value;
		hideTiling = params[Pattern_HIDETILINGCHKBOX]->u.bd.value;
		bgColor = params[Pattern_BGCOLOR]->u.cd.value;
		set_bg_color();
		patternColor = params[Pattern_PATTERNCOLOR]->u.cd.value;
		patternWidth = params[Pattern_PATTERNWIDTH]->u.fs_d.value;
		deltaMode = params[Pattern_CHOOSEDELTA]->u.pd.value;
		delta = params[DELTA_ID]->u.fs_d.value;
		angleInDegrees = params[ANGLE_ID]->u.fs_d.value;
		numSidesCenterMode = params[Pattern_NUMSIDES]->u.fs_d.value;
		mode = 1;
	}

	void pattern();
};
#endif // Draw_H