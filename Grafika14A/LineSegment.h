#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Point2D.h"
#include "PrimitiveRenderer.h"

class LineSegment {
public:
    LineSegment(const Point2D& start, const Point2D& end);

    Point2D getStart() const;
    Point2D getEnd() const;

    void setStart(const Point2D& point);
    void setEnd(const Point2D& point);

    void draw(PrimitiveRenderer& renderer, ALLEGRO_COLOR color, bool useIncremental = true) const;

private:
    Point2D start;
    Point2D end;
};

#endif