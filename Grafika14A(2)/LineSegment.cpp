#include "LineSegment.h"

LineSegment::LineSegment(const Point2D& start, const Point2D& end)
    : start(start), end(end) {}

Point2D LineSegment::getStart() const {
    return start;
}

Point2D LineSegment::getEnd() const {
    return end;
}

void LineSegment::setStart(const Point2D& point) {
    start = point;
}

void LineSegment::setEnd(const Point2D& point) {
    end = point;
}

void LineSegment::draw(PrimitiveRenderer& renderer, ALLEGRO_COLOR color, bool useIncremental) const {
    if (useIncremental) {
        renderer.drawLineIncremental(static_cast<int>(start.getX()), static_cast<int>(start.getY()),
            static_cast<int>(end.getX()), static_cast<int>(end.getY()), color);
    }
}