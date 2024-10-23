#include "PrimitiveRenderer.h"
#include "Point2D.h"

#define M_PI 3.14159265358979323846

PrimitiveRenderer::PrimitiveRenderer(ALLEGRO_DISPLAY* display)
	: display(display){}

PrimitiveRenderer::~PrimitiveRenderer() {}

void PrimitiveRenderer::drawPoint(int x, int y, ALLEGRO_COLOR color) {
	al_put_pixel(x, y, color);
}

void PrimitiveRenderer::drawLineIncremental(int x0, int y0, int x1, int y1, ALLEGRO_COLOR color) {

    int dx = x1 - x0;
    int dy = y1 - y0;

    if (abs(dy) <= abs(dx)) {
        // |m| <= 1, zwiększając x o 1 za każdym razem
        if (x0 > x1) {
            // Jeśli x0 jest większe niż x1, zamień punkty miejscami
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        float m = static_cast<float>(dy) / static_cast<float>(dx);
        float y = y0;

        for (int x = x0; x <= x1; ++x) {
            drawPoint(x, std::round(y), color);
            y += m;
        }
    }
    else {
        // |m| > 1, zwiększając y o 1 za każdym razem
        if (y0 > y1) {
            // Jeśli y0 jest większe niż y1, zamień punkty miejscami
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        float m = static_cast<float>(dx) / static_cast<float>(dy);
        float x = x0;

        for (int y = y0; y <= y1; ++y) {
            drawPoint(std::round(x), y, color);
            x += m;
        }
    }
}

void PrimitiveRenderer::drawPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color) {
    if (points.size() < 2) return; // Nie można narysować linii łamanej z mniej niż dwóch punktów

    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLineIncremental(static_cast<int>(points[i].getX()), static_cast<int>(points[i].getY()),
            static_cast<int>(points[i + 1].getX()), static_cast<int>(points[i + 1].getY()), color);
    }
}

void PrimitiveRenderer::drawClosedPolyline(const std::vector<Point2D>& points, ALLEGRO_COLOR color) {
    if (points.size() < 3) return; // Nie można narysować zamkniętej linii łamanej z mniej niż trzech punktów

    // Rysowanie otwartej linii przerywanej
    drawPolyline(points, color);

    // Połącz ostatni i pierwszy punkt
    drawLineIncremental(static_cast<int>(points.back().getX()), static_cast<int>(points.back().getY()),
        static_cast<int>(points.front().getX()), static_cast<int>(points.front().getY()), color);
}

void PrimitiveRenderer::drawCircleSymmetric(int x0, int y0, int radius, ALLEGRO_COLOR color) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    // Ośmiokrotna symetria
    auto drawSymmetricPoints = [&](int x, int y) {
        drawPoint(x0 + x, y0 + y, color);
        drawPoint(x0 - x, y0 + y, color);
        drawPoint(x0 + x, y0 - y, color);
        drawPoint(x0 - x, y0 - y, color);
        drawPoint(x0 + y, y0 + x, color);
        drawPoint(x0 - y, y0 + x, color);
        drawPoint(x0 + y, y0 - x, color);
        drawPoint(x0 - y, y0 - x, color);
        };

    drawSymmetricPoints(x, y);

    // Algorytm rysowania okręgu
    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;

        drawSymmetricPoints(x, y);
    }
}

void PrimitiveRenderer::drawEllipse(int x0, int y0, int rx, int ry, ALLEGRO_COLOR color) {
    float step = 0.01; // Krok dla obliczeń kąta (im mniejszy, tym gładsza elipsa)
    for (float alpha = 0; alpha <= M_PI / 2; alpha += step) {
        int x = static_cast<int>(rx * cos(alpha));
        int y = static_cast<int>(ry * sin(alpha));

        // Poczwórna symetria
        drawPoint(x0 + x, y0 + y, color); // Prawa dolna ćwiartka
        drawPoint(x0 + x, y0 - y, color); // Prawa górna ćwiartka
        drawPoint(x0 - x, y0 + y, color); // Lewa dolna ćwiartka
        drawPoint(x0 - x, y0 - y, color); // Lewa górna ćwiartka
    }
}

// Funkcja pomocnicza do określania orientacji trzech punktów
int orientation(const Point2D& p, const Point2D& q, const Point2D& r) {
    float val = (q.getY() - p.getY()) * (r.getX() - q.getX()) -
        (q.getX() - p.getX()) * (r.getY() - q.getY());

    if (val == 0) return 0;  // Punkty są współliniowe
    return (val > 0) ? 1 : 2; // 1 - zgodnie z ruchem wskazówek zegara, 2 - przeciwnie do ruchu wskazówek zegara
}

// Funkcja pomocnicza sprawdzająca, czy punkt q leży na odcinku pr
bool onSegment(const Point2D& p, const Point2D& q, const Point2D& r) {
    return q.getX() <= std::max(p.getX(), r.getX()) && q.getX() >= std::min(p.getX(), r.getX()) &&
        q.getY() <= std::max(p.getY(), r.getY()) && q.getY() >= std::min(p.getY(), r.getY());
}

// Funkcja pomocnicza do sprawdzania przecięcia dwóch segmentów
bool PrimitiveRenderer::segmentsIntersect(const Point2D& p1, const Point2D& q1, const Point2D& p2, const Point2D& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;

    // Wyjątki
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // 
}

void PrimitiveRenderer::drawPolygon(const std::vector<Point2D>& points, ALLEGRO_COLOR color) {
    size_t numPoints = points.size();

    if (numPoints < 3) {
        return;
    }


    // Rysowanie wielokonta
    for (size_t i = 0; i < numPoints - 1; ++i) {
        Point2D start = points[i];
        Point2D end = points[(i + 1) % numPoints];
        drawLineIncremental(static_cast<int>(start.getX()), static_cast<int>(start.getY()),
            static_cast<int>(end.getX()), static_cast<int>(end.getY()), color);
    }
    drawLineIncremental(static_cast<int>(points[numPoints - 1].getX()), static_cast<int>(points[numPoints - 1].getY()),
        static_cast<int>(points[0].getX()), static_cast<int>(points[0].getY()), color);
}