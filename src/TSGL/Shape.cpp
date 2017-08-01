#include "Shape.h"

namespace tsgl {

  Shape::Shape(int numVertices, const ColorFloat& c) {
    attribMutex.lock();
    if (numVertices < 1) {
      TsglDebug("Cannot have a shape without a vertex."); }
    length = numVertices;
    size = length * 2;
    current = 0;
    vertices = new float[size];
    init = false;
    color = c;
    attribMutex.unlock();
  }

  Shape::~Shape() {
    attribMutex.lock();
    delete[] vertices;
    attribMutex.unlock();
  }

  GLfloat* Shape::getPointerToVerticesArray() {
    attribMutex.lock();
    GLfloat* v = vertices;
    attribMutex.unlock();
    return v;
  }

  int Shape::getNumberOfVertices() {
    attribMutex.lock();
    int l = length;
    attribMutex.unlock();
    return l;
  }

  void Shape::addVertex(float x, float y) {
    attribMutex.lock();
    if (init) {
      TsglDebug("Cannot add anymore vertices.");
      // attribMutex.unlock();
      return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    current += 2;
    if (current == size) init = true;
    attribMutex.unlock();
  }

  ColorFloat Shape::getColor() {
    attribMutex.lock();
    ColorFloat c = color;
    attribMutex.unlock();
    return c;
  }

  ColorFloat* Shape::getObjectColor() {
    attribMutex.lock();
    ColorFloat* c = &color;
    attribMutex.unlock();
    return c;
  }


  void Shape::setColor(const ColorFloat& c) {
    attribMutex.lock();
    color = c;
    attribMutex.unlock();
  }

  void Shape::setCenter(float x, float y) {
    float deltaX = x - getX(); //Change for x
    float deltaY = y - getY(); //Change for y
    attribMutex.lock();

    for(int i = 0; i < length; i++) {
      vertices[i*2]     += deltaX; //Transpose x
      vertices[(i*2)+1] += deltaY; //Transpose y
    }
    attribMutex.unlock();
  }

  float Shape::getX() { //TODO: decide if this is the best system to protect x and y
    attribMutex.lock();
    float minX, maxX;
    minX = maxX = vertices[0];

    //Find min and max X
    for(int i = 0; i < length; i++) {
      if( vertices[i*2] < minX )
        minX = vertices[i*2];
      else if( vertices[i*2] > maxX )
        maxX = vertices[i*2];
    }

    attribMutex.unlock();
    return (minX+maxX)/2;
  }

  float Shape::getY() {
    attribMutex.lock();
    float minY, maxY;
    minY = maxY = vertices[1];

    //Find min and max X
    for(int i = 0; i < length; i++) {
      if( vertices[i*2+1] < minY )
        minY = vertices[i*2+1];
      else if( vertices[i*2+1] > maxY )
        maxY = vertices[i*2+1];
    }

    attribMutex.unlock();
    return (minY+maxY)/2;
  }

  void Shape::centeredRotation(float angle) {
    setRotation(angle, getX(), getY());
  }

}
