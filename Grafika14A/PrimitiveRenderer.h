#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <allegro5/allegro.h>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>

class Point2D;

class PrimitiveRenderer {
public:
	PrimitiveRenderer(ALLEGRO_DISPLAY* display);
	~PrimitiveRenderer();

	void drawPoint(int x, int y, ALLEGRO_COLOR color);
	void drawLineIncremental(int x0, int y0, int x1, int y1, ALLEGRO_COLOR color);
	void drawPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color);
	void drawClosedPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color);
	void drawCircleSymmetric(int x0, int y0, int radius, ALLEGRO_COLOR color);
	void drawEllipse(int x0, int y0, int rx, int ry, ALLEGRO_COLOR color);
	void drawPolygon(const std::vector<Point2D>& points, ALLEGRO_COLOR color);
	bool segmentsIntersect(const Point2D& p1, const Point2D& q1, const Point2D& p2, const Point2D& q2);

private:
	ALLEGRO_DISPLAY* display;
};

#endif 
