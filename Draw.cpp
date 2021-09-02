// (c) 2021 Nithisha Nantha Kumar
// This code is licensed under MIT license (see LICENSE.md for details)

#include "Draw.h"

point draw::solve_equation(equation& eq1, equation& eq2) {
	// Refer to: http://www.mathnstuff.com/math/algebra/adeterm.htm
	point solution;
	solution.x = ((eq1.c * eq2.b) - (eq1.b * eq2.c)) /
		         ((eq1.a * eq2.b) - (eq1.b * eq2.a));
	solution.y = ((eq1.a * eq2.c) - (eq1.c * eq2.a)) / 
		         ((eq1.a * eq2.b) - (eq1.b * eq2.a));
	return solution;
}

equation draw::construct_equation_from_pts(point& pt1, point& pt2) {
	// (y - y1) / (y2 - y1) = (x - x1) / (x2 - x1) is used
	// to calculate the equation joining two points with 
	// coordinates (x1, y1) and (x2, y2). When rearranged to
	// ax + by = c form:
	equation eq;
	eq.a = 1 / (pt2.x - pt1.x);
	eq.b = -1 / (pt2.y - pt1.y);
	eq.c = (pt1.x / (pt2.x - pt1.x)) - (pt1.y / (pt2.y - pt1.y));
	return eq;
}

point draw::rotatePoint(point& original, point& rotateAbt, double angleInRadians) {
	// Refer to: https://tinyurl.com/58pm4x98
	point rotatedPoint;
	rotatedPoint.x = cos(angleInRadians) * (original.x - rotateAbt.x)
		- sin(angleInRadians) * (original.y - rotateAbt.y) + rotateAbt.x;
	rotatedPoint.y = sin(angleInRadians) * (original.x - rotateAbt.x)
		+ cos(angleInRadians) * (original.y - rotateAbt.y) + rotateAbt.y;
	return rotatedPoint;
}

bool draw::isCollinear(equation& eq1, equation& eq2) {

	// 2 lines are collinear if their slopes (-a/b) 
	// & their y-intercepts are =
	// 2 lines are parallel if their slopes are
	// = but y-intercepts are !=
    // We aren't comparing y-intercepts as the lines
	// cannot be parallel & thus only check slope equality.

	// To prevent division of infinity
	if (isinf(eq2.a) && !isinf(eq2.b) && isinf(eq1.a) && !isinf(eq1.b)) {
		return true;
	}

	// Casting to float to reduce precision from double for almost collinear lines.
	return fabs(float(-eq2.a / eq2.b) - float(-eq1.a / eq1.b)) < 0.0001;
}

bool draw::isHorizontal(equation& eq) {
	// Slope of a horizontal line (-a/b) is 0.
	return (-eq.a / eq.b) == 0;
}

bool draw::isVertical(equation& eq) {
	// Slope of a vertical line (-a/b)  is infinity.
	return isinf(-eq.a / eq.b);
}

point draw::move_along_line(point& v1, point& v2, point midPoint,
	                        double distance) {
	// Refer to: https://tinyurl.com/fxescdja
	point pt;
	pt.x = v2.x - v1.x;
	pt.y = v2.y - v1.y;
	double magnitude = sqrt((pt.x * pt.x) + (pt.y * pt.y));
	// normalize vector
	pt.x /= magnitude;
	pt.y /= magnitude;
	midPoint.x += (distance * pt.x);
	midPoint.y += (distance * pt.y);
	return midPoint;
}

point draw::findIntersection(point& eq1pt1, point& eq1pt2, 
	                         point& eq2pt1, point& eq2pt2) {
	point solution;
	equation eq1 = construct_equation_from_pts(eq1pt1, eq1pt2);
	equation eq2 = construct_equation_from_pts(eq2pt1, eq2pt2);

	if (isCollinear(eq1, eq2)) {
		solution.x = eq1pt2.x;
		solution.y = eq1pt2.y;
	}
	else if (isHorizontal(eq1) && isVertical(eq2)) {
		solution.x = eq2pt2.x;
		solution.y = eq1pt2.y;
	}
	else if (isVertical(eq1) && isHorizontal(eq2)) {
		solution.x = eq1pt2.x;
		solution.y = eq2pt2.y;
	}
	else if (!isHorizontal(eq1) && isVertical(eq2)) {
		solution.x = eq2pt2.x;
		solution.y = ((-eq1.a / eq1.b) * eq2pt2.x) + (eq1.c / eq1.b);
	}
	else if (isVertical(eq1) && !isHorizontal(eq2)) {
		solution.x = eq1pt2.x;
		solution.y = ((-eq2.a / eq2.b) * eq1pt2.x) + (eq2.c / eq2.b);
	}
	else if (!isVertical(eq2) && isHorizontal(eq1)) {
		solution.x = ((-eq2.b / eq2.a) * (eq1pt2.y)) + (eq2.c / eq2.a);
		solution.y = eq1pt2.y;
	}
	else if (!isVertical(eq1) && isHorizontal(eq2)) {
		solution.x = ((-eq1.b / eq1.a) * (eq2pt2.y)) + (eq1.c / eq1.a);
		solution.y = eq2pt2.y;
	}
	else {
		solution = solve_equation(eq1, eq2);
	}

	return solution;
}

void draw::calculate_center_vertices(int numSides) {
	vertices.clear();
	// Finding coordinates of the polygon's vertices.
	double radius = sideLength / (2 * sin(PF_PI / numSides));
	for (int i = 0; i < numSides; i++) {
		point vertex;
		vertex.x = center.x + radius * cos((PF_PI / 2) +
			PF_PI / numSides * (1 + 2 * i));
		vertex.y = center.y + radius * sin((PF_PI / 2) +
			PF_PI / numSides * (1 + 2 * i));
		vertices.push_back(vertex);
	}
	sort_vertices();
}

void draw::calculate_midpoints(int numSides) {
	midpoints.clear();
	point lastVertex;

	// Midpoint of two points with coordinates (x1, y1) and
	// (x2, y2) is ((x1 + x2) / 2, (y1 + y2) / 2).

	// Calculating midpoints of each side of the polygon:

	for (int i = 0; i < numSides - 1; i++) {
		point vertex;
		if (mode == 1) {
			vertex.x = (vertices[i].x + vertices[i + 1].x) / 2.0;
			vertex.y = (vertices[i].y + vertices[i + 1].y) / 2.0;
		}
		else if (mode == 2) {
			vertex.x = (centerPolygonVertices[i].x +
				        centerPolygonVertices[i + 1].x) / 2.0;
			vertex.y = (centerPolygonVertices[i].y + 
				        centerPolygonVertices[i + 1].y) / 2.0;
		}
		else {
			vertex.x = (originPolygonVertices[i].x + 
				        originPolygonVertices[i + 1].x) / 2.0;
			vertex.y = (originPolygonVertices[i].y + 
				        originPolygonVertices[i + 1].y) / 2.0;
		}
		midpoints.push_back(vertex);
	}
	if (mode == 1) {
		lastVertex.x = (vertices[0].x + vertices[numSides - 1].x) / 2.0;
		lastVertex.y = (vertices[0].y + vertices[numSides - 1].y) / 2.0;
	}
	else if (mode == 2) {
		lastVertex.x = (centerPolygonVertices[0].x + 
			            centerPolygonVertices[numSides - 1].x) / 2.0;
		lastVertex.y = (centerPolygonVertices[0].y + 
			            centerPolygonVertices[numSides - 1].y) / 2.0;
	}
	else {
		lastVertex.x = (originPolygonVertices[0].x + 
			            originPolygonVertices[numSides - 1].x) / 2.0;
		lastVertex.y = (originPolygonVertices[0].y + 
			            originPolygonVertices[numSides - 1].y) / 2.0;
	}
	midpoints.push_back(lastVertex);
}

void draw::draw_ngon_star(int numSides) {

	point lastVertex, solution, eq1pt1, 
		  eq1pt2, eq2pt1, eq2pt2;

	// Delta factors:
	int df1, df2, df3, df4;

	// Moves rays away from the midpoint.
	if (deltaMode == DELTA_AWAY) {
		df1 = 1;
		df2 = df3 = df4 = -1;
	}
	// Moves rays towards the midpoint.
	else {
		df1 = -1;
		df2 = df3 = df4 = 1;
	}

	calculate_midpoints(numSides);

	for (int i = 0; i < numSides - 1; i++) {

		// A vertex adjacent to the midpoint of
		// a polygon's side is rotated by the given angle.
		// An equation of a line is formed with this rotated
		// vertex and the midpoint adjacent to it.
		// Another equation of a line is formed with the
		// same rotated vertex and the second midpoint
		// adjacent to it.
		// The intersection point of both of these lines are
		// then calculated.

		eq1pt1 = midpoints[i];
		eq2pt1 = midpoints[i + 1];

		if (mode == 1) {
			eq1pt2 = rotatePoint(vertices[i + 1], midpoints[i],
				-angleInDegrees * PF_PI / 180);
			eq2pt2 = rotatePoint(vertices[i + 1], midpoints[i + 1],
				angleInDegrees * PF_PI / 180);
			eq1pt1 = move_along_line(vertices[i], vertices[i + 1], eq1pt1,
				df1 * delta / 2.0);
			eq2pt1 = move_along_line(vertices[i + 1], eq2pt1, eq2pt1,
				df2 * delta / 2.0);
		}
		else if (mode == 2) {
			eq1pt2 = rotatePoint(centerPolygonVertices[i + 1], midpoints[i],
				-angleInDegrees * PF_PI / 180);
			eq2pt2 = rotatePoint(centerPolygonVertices[i + 1], midpoints[i + 1],
				angleInDegrees * PF_PI / 180);
			eq1pt1 = move_along_line(centerPolygonVertices[i], 
				                     centerPolygonVertices[i + 1], eq1pt1,
				df1 * delta / 2.0);
			eq2pt1 = move_along_line(centerPolygonVertices[i + 1], eq2pt1, eq2pt1,
				df2 * delta / 2.0);
		}
		else {
			eq1pt2 = rotatePoint(originPolygonVertices[i + 1], midpoints[i],
				-angleInDegrees * PF_PI / 180);
			eq2pt2 = rotatePoint(originPolygonVertices[i + 1], midpoints[i + 1],
				angleInDegrees * PF_PI / 180);
			eq1pt1 = move_along_line(originPolygonVertices[i], 
				                     originPolygonVertices[i + 1], eq1pt1,
				df1 * delta / 2.0);
			eq2pt1 = move_along_line(originPolygonVertices[i + 1], eq2pt1, eq2pt1,
				df2 * delta / 2.0);
		}

		solution = findIntersection(eq1pt1, eq1pt2, eq2pt1, eq2pt2);

		cairo_move_to(cr, eq1pt1.x, eq1pt1.y);
		cairo_line_to(cr, solution.x, solution.y);
		cairo_move_to(cr, eq2pt1.x, eq2pt1.y);
		cairo_line_to(cr, solution.x, solution.y);
		cairo_identity_matrix(cr);
	}
	
	eq1pt1 = midpoints[numSides - 1];
	eq2pt1 = midpoints[0];

	if (mode == 1) {
	
		eq1pt2 = rotatePoint(vertices[0], midpoints[numSides - 1],
			-angleInDegrees * PF_PI / 180);
		eq2pt2 = rotatePoint(vertices[0], midpoints[0],
			angleInDegrees * PF_PI / 180);
		eq1pt1 = move_along_line(vertices[0], vertices[numSides - 1], eq1pt1,
			df3 * delta / 2.0);
		eq2pt1 = move_along_line(vertices[0], eq2pt1, eq2pt1,
			df4 * delta / 2.0);
	}
	else if (mode == 2) {
	
		eq1pt2 = rotatePoint(centerPolygonVertices[0], midpoints[numSides - 1],
			-angleInDegrees * PF_PI / 180);
		eq2pt2 = rotatePoint(centerPolygonVertices[0], midpoints[0],
			angleInDegrees * PF_PI / 180);
		eq1pt1 = move_along_line(centerPolygonVertices[0], 
			                     centerPolygonVertices[numSides - 1], eq1pt1,
			df3 * delta / 2.0);
		eq2pt1 = move_along_line(centerPolygonVertices[0], eq2pt1, eq2pt1,
			df4 * delta / 2.0);
	}
	else {
	
		eq1pt2 = rotatePoint(originPolygonVertices[0], midpoints[numSides - 1],
			-angleInDegrees * PF_PI / 180);
		eq2pt2 = rotatePoint(originPolygonVertices[0], midpoints[0],
			angleInDegrees * PF_PI / 180);

		eq1pt1 = move_along_line(originPolygonVertices[0],
			                     originPolygonVertices[numSides - 1], eq1pt1,
			df3 * delta / 2.0);
		eq2pt1 = move_along_line(originPolygonVertices[0], eq2pt1, eq2pt1,
			df4 * delta / 2.0);
	}

	solution = findIntersection(eq1pt1, eq1pt2, eq2pt1, eq2pt2);

	cairo_move_to(cr, eq1pt1.x, eq1pt1.y);
	cairo_line_to(cr, solution.x, solution.y);
	cairo_move_to(cr, eq2pt1.x, eq2pt1.y);
	cairo_line_to(cr, solution.x, solution.y);
	
	set_pattern_properties();
	cairo_stroke_preserve(cr);
}

void draw::draw_shape() {
	if (!hideTiling) {
		if (mode == 1) {
			cairo_move_to(cr, vertices[0].x, vertices[0].y);

			for (int i = 1; i < int(vertices.size()); i++) {
				cairo_line_to(cr, vertices[i].x, vertices[i].y);
			}
			cairo_line_to(cr, vertices[0].x, vertices[0].y);
		}
		else if (mode == 2) {
			cairo_move_to(cr, centerPolygonVertices[0].x, centerPolygonVertices[0].y);

			for (int i = 1; i < int(centerPolygonVertices.size()); i++) {
				cairo_line_to(cr, centerPolygonVertices[i].x, centerPolygonVertices[i].y);
			}
			cairo_line_to(cr, centerPolygonVertices[0].x, centerPolygonVertices[0].y);
		}
		else {
			cairo_move_to(cr, originPolygonVertices[0].x, originPolygonVertices[0].y);

			for (int i = 1; i < int(originPolygonVertices.size()); i++) {
				cairo_line_to(cr, originPolygonVertices[i].x, originPolygonVertices[i].y);
			}
			cairo_line_to(cr, originPolygonVertices[0].x, originPolygonVertices[0].y);
		}
		cairo_stroke_preserve(cr);
	}
}

void draw::draw_ngon(int numSides) {

	calculate_center_vertices(numSides);

	if (!hideTiling) {
		draw_shape();
	}

}

void draw::set_pattern_properties() {
	cairo_set_source_rgba(cr, patternColor.red / 255.0, 
		                      patternColor.green / 255.0,
		                      patternColor.blue / 255.0, 1.0);
	cairo_set_line_width(cr, patternWidth);
}


void draw::set_bg_color()
{	cairo_set_source_rgba(cr, bgColor.red / 255.0, 
	                          bgColor.green / 255.0,
		                      bgColor.blue / 255.0, 1.0);
	cairo_rectangle(cr, 0, 0, double(output->width), double(output->height));
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}

double draw::findApothem(double radius) {
	// The apothem is a line segment that connects
	// the midpoint of a polygon's side to the 
	// center of the polygon.
	// It is perpendicular to the side it emerges 
	// from and bisects the same.
	// A right angled triangle is formed by the
	// radius of the polygon (hypotenuse), half
	// the side length of the polygon (base)
	// and the apothem (height).
	// We apply pythogoras theorem in this triangle
	// to find the length of the apothem.
	return sqrt(pow(radius, 2) - pow(sideLength / 2.0, 2));
}

point draw::findCenter(point& v0, point& v1, int numSides, 
	                   int numSidesCenter, int side) {

	double radius = sideLength / (2.0 * (sin(PF_PI / numSides)));
	int factor = 1;

	if (side < numSidesCenter / 2 || side == numSidesCenter) {
		factor = -1;
	}

	// Find midpoint of line joining v0 and v1:
	point midPoint;
	midPoint.x = (v0.x + v1.x) / 2.0;
	midPoint.y = (v0.y + v1.y) / 2.0;

	equation eq = construct_equation_from_pts(v0, v1);

	// We find a point that is the apothem length away
	// from the midpoint of the polygon's side, which is the 
	// center of the polygon.

	if (isHorizontal(eq)) {
		midPoint.y += findApothem(radius) * factor;
	}
	else if (isVertical(eq)) {
		midPoint.x += findApothem(radius) * factor;
	}
	else {

		// Calculate slope of the side:
		double sideSlope = (v1.y - v0.y) / (v1.x - v0.x);

		// The apothem is perpendicular to the side so its slope would be
		// the -ve reciprocal of the slope of the side it emerges from.
		double apothemSlope = -1 / sideSlope;

		// Refer to: https://tinyurl.com/4pfbcb4j

		midPoint.x += findApothem(radius) *
			          sqrt(1 / (1 + pow(apothemSlope, 2))) * factor;
		midPoint.y += apothemSlope * findApothem(radius) * 
			          sqrt(1 / (1 + pow(apothemSlope, 2))) * factor;
	}

	return midPoint;
}

void draw::calculate_vertices_known(point v0, point v1, int numSides, 
	                                int numSidesCenter, int side) {
	center = findCenter(v0, v1, numSides, numSidesCenter, side);
	if (mode == 1) {
		vertices.clear();
		vertices.push_back(v0);
		vertices.push_back(v1);
	}
	else if (mode == 2) {
		centerPolygonVertices.clear();
		centerPolygonVertices.push_back(v0);
		centerPolygonVertices.push_back(v1);
	}
	else {
		originPolygonVertices.clear();
		originPolygonVertices.push_back(v0);
		originPolygonVertices.push_back(v1);
	}

	// Refer to: https://tinyurl.com/mwkruy
    double r = sqrt(pow((v0.x - center.x), 2) + pow((v0.y - center.y), 2));
	double a = atan2((v0.y - center.y), (v0.x - center.x));
	for (int i = 2; i < numSides; i++) {
		point vertex;
		vertex.x = center.x + r * cos(a + 2 * PF_PI * i / numSides);
		vertex.y = center.y + r * sin(a + 2 * PF_PI * i / numSides);
		if (mode == 1) {
			vertices.push_back(vertex);
		}
		else if (mode == 2) {
			centerPolygonVertices.push_back(vertex);
		}
		else {
			originPolygonVertices.push_back(vertex);
		}
	}
	sort_vertices();
}

void draw::sort_vertices() {

	std::map<double, point> verticeAngle;

	// Refer to: https://tinyurl.com/6aa5hsbf

	if (mode == 1) {
		for (int i = 0; i < int(vertices.size()); i++) {
			verticeAngle[atan2(vertices[i].x - center.x, 
				         vertices[i].y - center.y)] = vertices[i];
		}
		vertices.clear();
	}
	else if (mode == 2) {
		for (int i = 0; i < int(centerPolygonVertices.size()); i++) {
			verticeAngle[atan2(centerPolygonVertices[i].x - center.x,
		    centerPolygonVertices[i].y - center.y)] = centerPolygonVertices[i];
		}
		centerPolygonVertices.clear();
	}
	else {
		for (int i = 0; i < int(originPolygonVertices.size()); i++) {
			verticeAngle[atan2(originPolygonVertices[i].x - center.x, 
            originPolygonVertices[i].y - center.y)] = originPolygonVertices[i];
		}
		originPolygonVertices.clear();
	}

	for (std::map<double, point>::iterator it = verticeAngle.begin();
		it != verticeAngle.end(); it++) {

		if (mode == 1) {
			vertices.push_back(it->second);
		}
		else if (mode == 2) {
			centerPolygonVertices.push_back(it->second);
		}
		else {
			originPolygonVertices.push_back(it->second);
		}
	}
}

bool draw::attach_shape(int numSidesShapeToAttach, int numSidesCenter, 
	                    int side) {

	if (mode == 1) {

		if (side == numSidesCenter) {

			calculate_vertices_known(vertices[numSidesCenter - 1], 
				                     vertices[0],
				                     numSidesShapeToAttach, numSidesCenter, side);
		}
		else if (side != numSidesCenter) {

			calculate_vertices_known(vertices[side - 1], 
				                     vertices[side],
			                        numSidesShapeToAttach, numSidesCenter, side);
		}
	}
	else if (mode == 2) {

		if (side == numSidesCenter) {

			calculate_vertices_known(centerPolygonVertices[numSidesCenter - 1], 
				                     centerPolygonVertices[0],
				                     numSidesShapeToAttach, numSidesCenter, side);
		}
		else if (side != numSidesCenter) {

			calculate_vertices_known(centerPolygonVertices[side - 1], 
				                     centerPolygonVertices[side],
			                       	 numSidesShapeToAttach, numSidesCenter, side);
		}
	}
	else {

		if (side == numSidesCenter) {

			calculate_vertices_known(originPolygonVertices[numSidesCenter - 1], 
				                     originPolygonVertices[0],
				                     numSidesShapeToAttach, numSidesCenter, side);
		}
		else if (side != numSidesCenter) {

			calculate_vertices_known(originPolygonVertices[side - 1], 
				                     originPolygonVertices[side],
				                     numSidesShapeToAttach, numSidesCenter, side);
		}

	}

	// Indicates to stop attaching shapes when all vertices of the polygon to be
	// attached are outside the output.
	if (!areVerticesBounded()) {
		return false;
	}

	if (!hideTiling) {
		draw_shape();
	}
	return true;
}

bool draw::isWithinBounds(point& pt) {	
	return (pt.x > 0 && pt.x < double(output->width))
		&& (pt.y > 0 && pt.y < double(output->height));
}

bool draw::areVerticesBounded() {
	bool indicator = false;
	if (mode == 1) {
		for (int i = 0; i < int(vertices.size()); i++) {
			if (isWithinBounds(vertices[i])) {
				indicator = true;
				break;
			}
		}
	}
	else if (mode == 2) {
		for (int i = 0; i < int(centerPolygonVertices.size()); i++) {
			if (isWithinBounds(centerPolygonVertices[i])) {
				indicator = true;
				break;
			}
		}

	}
	else {
		for (int i = 0; i < int(originPolygonVertices.size()); i++) {
			if (isWithinBounds(originPolygonVertices[i])) {
				indicator = true;
				break;
			}
		}
	}
	return indicator;
}


int draw::find_vertex_from_center_ngon(int index) {

	for (int i = 0; i < int(centerPolygonVertices.size()); i++) {

		// Find the position of the point at index from centerPolygon 
		// in centerPolygonVertices:
		if (centerPolygonVertices[i].x == centerPolygon[index].x &&
			centerPolygonVertices[i].y == centerPolygon[index].y) {
			int position;

			// Position of point before the the one
			// at index in centerPolygon is stored.
			if (i == 0) {
				position = int(centerPolygonVertices.size()) - 1;
			}
			else {
				position = i - 1;
			}

			point temp = centerPolygonVertices[position];

			for (int j = 0; j < int(centerPolygon.size()); j++) {
				// If centerPolygon contains the point temp, then
				// return the position of the point after the point
				// from centerPolygon at index.
				if (temp.x == centerPolygon[j].x && temp.y == centerPolygon[j].y) {
					return i + 1;
				}
			}

			// Otherwise, return the position of the point before the point
			// from centerPolygon at index.
			return position;

		}
	}
	return -1;
}

void draw::pattern() {
	if (outputMode == MODE_CENTERED) {
		draw_ngon(numSidesCenterMode);
		draw_ngon_star(numSidesCenterMode);
	}
	else if (outputMode == MODE_MULTIPLE) {
		if (tilingChoice == TILING_666) {
			regular_tiling(6);
		}
		else if (tilingChoice == TILING_444) {
			regular_tiling(4);
		}
		else if (tilingChoice == TILING_488) {
			four_eight_eight();
		}
		else if (tilingChoice == TILING_31212) {
			three_twelve_twelve();
		}
		else if (tilingChoice == TILING_4612) {
			four_six_twelve();
		}
	}
}





