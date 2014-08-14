/*
 * Line.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: mbv26
 */

#include "Line.h"

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color) {
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[6] = x2;
    vertices[7] = y2;
    vertices[2] = vertices[8] = color.R;
    vertices[3] = vertices[9] = color.G;
    vertices[4] = vertices[10] = color.B;
    vertices[5] = vertices[11] = color.A;
}

void Line::draw() {
    glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
}
