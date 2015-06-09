/*
 * ColoredPolygon.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: mbv26
 */

#include "ColoredPolygon.h"

namespace tsgl {

ColoredPolygon::ColoredPolygon(int v) {
    if (v < 3) throw std::out_of_range("Cannot have a polygon with fewer than 3 vertices.");
    length = v;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
}

ColoredPolygon::~ColoredPolygon() {
    delete vertices;
}

void ColoredPolygon::addVertex(int x, int y, const ColorFloat &color) {
    if (init) return;
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == size) init = true;
}

void ColoredPolygon::draw() {
    if (!init) return;
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
}

}
