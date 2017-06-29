#include "Shape.h"

namespace tsgl {

Shape::Shape(int numVertices, const ColorFloat& c) {
    if (numVertices < 2)
      TsglDebug("Cannot have a shape with fewer than 2 vertices.");
    length = numVertices;
    size = length * 2;
    current = 0;
    vertices = new float[size];
    init = false;
    color = c;
}

Shape::~Shape() {
    delete[] vertices;
}

GLfloat* Shape::getPointerToVerticesArray() {
  return vertices;
}

void Shape::addVertex(int x, int y) {
    if (init) {
      TsglDebug("Cannot add anymore vertices.");
      return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    current += 2;
    if (current == size) init = true;
}

void Shape::setColor(const ColorFloat& c) {
  color = c;
}

void Shape::setCenter(float x, float y) {
  float deltaX = x - getX(); //Change for x
  float deltaY = y - getY(); //Change for y

  for(int i = 0; i < length; i++) {
    vertices[i*2]     += deltaX; //Transpose x
    vertices[(i*2)+1] += deltaY; //Transpose y
  }
}

float Shape::getX() {
  float minX, maxX;
  minX = maxX = vertices[0];

  //Find min and max X
  for(int i = 0; i < length; i++) {
    if( vertices[i*2] < minX )
      minX = vertices[i*2];
    else if( vertices[i*2] > maxX )
      maxX = vertices[i*2];
  }

  return (minX+maxX)/2;
}

float Shape::getY() {
  float minY, maxY;
  minY = maxY = vertices[1];

  //Find min and max X
  for(int i = 0; i < length; i++) {
    if( vertices[i*2+1] < minY )
      minY = vertices[i*2+1];
    else if( vertices[i*2+1] > maxY )
      maxY = vertices[i*2+1];
  }

  return (minY+maxY)/2;
}

void Shape::rotate(float angle) {
  float centerX = getX(), centerY = getY();

  Shape::setCenter(0, 0); //Transpose to center for rotation

  Shape::rotateAroundOrigin(angle);

  Shape::setCenter(centerX, centerY); //Return to original location
}

// void Shape::rotateAround(float angle, float x, float y) { //Needs to be tested
//   float centerX = getX(), centerY = getY();
//
//   Shape::setCenter(centerX-x, centerY-y); //Transpose for origin to be (x, y)
//
//   Shape::rotateAroundOrigin(angle);
//
//   Shape::setCenter(centerX, centerY); //Return to original location
// }

void Shape::rotateAroundOrigin(float angle) {
  for(int i = 0; i < current; i+=2) {
    float x = vertices[i];
    float y = vertices[i+1];
    vertices[i]   = x*cos(angle)-y*sin(angle); //New x
    vertices[i+1] = x*sin(angle)+y*cos(angle); //New y
  }
}

}
