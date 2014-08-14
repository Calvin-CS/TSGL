/*
 * Polyline.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: mbv26
 */

#include "Polyline.h"

Polyline::Polyline(int v) {
    if (v < 2) throw std::out_of_range("Cannot have a line with fewer than 2 vertices.");
    length = v;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
}

Polyline::~Polyline() {
    delete vertices;
}

void Polyline::addNextVertex(int x, int y, const ColorFloat &color) {
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

void Polyline::draw() {
    if (!init) return;
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, length);
}
