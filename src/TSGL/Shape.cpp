#include "Shape.h"

namespace tsgl {

Shape::Shape(int numVertices) {
    if (numVertices < 2)
      TsglDebug("Cannot have a shape with fewer than 2 vertices.");
    length = numVertices;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
}

Shape::~Shape() {
    delete[] vertices;
}

void Shape::addVertex(int x, int y, const ColorFloat &color) {
    if (init) {
      TsglDebug("Cannot add anymore vertices.");
      return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == size) init = true;
}

void Shape::draw() {
    if (!init) {
      TsglDebug("Cannot draw yet.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, length);
}

float* Shape::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

void Shape::setColor(ColorFloat c) {
  for(int i = 2; i < size; i+=6) { //Set Red values
    vertices[i] = c.R;
  }
  for(int i = 3; i < size; i+=6) { //Set Green values
    vertices[i] = c.G;
  }
  for(int i = 4; i < size; i+=6) { //Set Blue values
    vertices[i] = c.B;
  }
  for(int i = 5; i < size; i+=6) { //Set Alpha values
    vertices[i] = c.A;
  }
}

void Shape::setCenter(int x, int y) {
  int deltaX = x - getX(); //Change for x
  int deltaY = y - getY(); //Change for y

  for(int i = 0; i < length; i++) {
    vertices[i*6]     += deltaX; //Transpose x
    vertices[(i*6)+1] += deltaY; //Transpose y
  }
}

int Shape::getX() {
  float minX, maxX;
  minX = maxX = vertices[0];

  //Find min and max X
  for(int i = 0; i < length; i++) {
    if( vertices[i*6] < minX )
      minX = vertices[i*6];
    else if( vertices[i*6] > maxX )
      maxX = vertices[i*6];
  }

  return (minX+maxX)/2;
}

int Shape::getY() {
  float minY, maxY;
  minY = maxY = vertices[1];

  //Find min and max X
  for(int i = 0; i < length; i++) {
    if( vertices[i*6+1] < minY )
      minY = vertices[i*6+1];
    else if( vertices[i*6+1] > maxY )
      maxY = vertices[i*6+1];
  }

  return (minY+maxY)/2;
}


}
