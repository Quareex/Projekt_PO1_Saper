#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include <queue>

#define M_PI 3.14159265358979323846

PrimitiveRenderer::PrimitiveRenderer() {}

PrimitiveRenderer::~PrimitiveRenderer() {}

void PrimitiveRenderer::clear() {
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));

    
}

void PrimitiveRenderer::drawPoint(int x, int y, ALLEGRO_COLOR color) {
	al_put_pixel(x, y, color);
}

void PrimitiveRenderer::drawLineIncremental(int x0, int y0, int x1, int y1, ALLEGRO_COLOR color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (abs(dy) <= abs(dx)) {
        // Slope |m| <= 1, increment x
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        float m = static_cast<float>(dy) / static_cast<float>(dx);
        float y = y0;

        for (int x = x0; x <= x1; ++x) {
            drawPoint(x, static_cast<int>(std::round(y)), color);
            y += m;
        }
    }
    else {
        // Slope |m| > 1, increment y
        if (y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            dx = x1 - x0;
            dy = y1 - y0;
        }

        float m = static_cast<float>(dx) / static_cast<float>(dy);
        float x = x0;

        for (int y = y0; y <= y1; ++y) {
            drawPoint(static_cast<int>(std::round(x)), y, color);
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
        return;  // Not enough points to form a polygon
    }

    // Draw all edges of the polygon by connecting consecutive points
    for (size_t i = 0; i < numPoints; ++i) {
        Point2D start = points[i];
        Point2D end = points[(i + 1) % numPoints];  // Wrap around to the first point
        drawLineIncremental(
            static_cast<int>(start.getX()), static_cast<int>(start.getY()),
            static_cast<int>(end.getX()), static_cast<int>(end.getY()), color
        );
    }
}

bool colorsAreEqual(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2) {
    float tolerance = 0.1f;
    return (fabs(color1.r - color2.r) < tolerance &&
        fabs(color1.g - color2.g) < tolerance &&
        fabs(color1.b - color2.b) < tolerance);
}

void PrimitiveRenderer::floodFill(int startX, int startY, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR boundaryColor, ALLEGRO_BITMAP* targetBitmap) {
    // Get color of the starting pixel
    ALLEGRO_COLOR startColor = al_get_pixel(targetBitmap, startX, startY);

    // If the starting color is already the fill color, no need to do anything
    if (colorsAreEqual(startColor, fillColor)) return;

    int width = al_get_bitmap_width(targetBitmap);
    
    int height = al_get_bitmap_height(targetBitmap);
    

    al_lock_bitmap(targetBitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);

    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));

    // Stack to store the pixels to process
    std::queue<std::pair<int, int>> queue;
    queue.push({ startX, startY });

    std::vector<std::pair<int, int>> pixelsToFill;

    // Loop while there are pixels in the queue
    while (!queue.empty()) {
        // Get the current coordinates
        std::pair<int, int> current = queue.front();
        queue.pop();

        int currX = current.first;
        int currY = current.second;

        
        // Skip if the current pixel is either the boundary or already filled
        if (currX < 0 || currY < 0 || currX >= width || currY >= height || visited[currX][currY]) {
            continue;
        }

        ALLEGRO_COLOR currColor = al_get_pixel(targetBitmap, currX, currY);

        // If it's the boundary color or already the fill color, skip this pixel
        if (colorsAreEqual(currColor, boundaryColor) || colorsAreEqual(currColor, fillColor)) {
            continue;
        }

        // Fill the current pixel with the fill color
        
        visited[currX][currY] = true;

        pixelsToFill.push_back({ currX, currY });
        // Push the 4 neighboring pixels (North, South, East, West) to the stack if they haven't been filled
        queue.push({ currX, currY - 1 }); // North
        queue.push({ currX, currY + 1 }); // South
        queue.push({ currX - 1, currY }); // West
        queue.push({ currX + 1, currY }); // East
    }
    for (const auto& pixel : pixelsToFill) {
        al_put_pixel(pixel.first, pixel.second, fillColor);
    }
   
    al_unlock_bitmap(targetBitmap);
    al_flip_display();
}