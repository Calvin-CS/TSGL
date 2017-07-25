/*
 * PointLayer.cpp implements the methods defined in PointLayer.h
 */

#include "PointLayer.h"

namespace tsgl {

  PointLayer::PointLayer(const ColorFloat& c) {
    discreteRender = true;
    color = c; // TODO have all of these been written incorectly?  Check entire library for pointer issues
  }

  PointLayer::~PointLayer() {
    delete &pointVertices;
  }

  void PointLayer::addPoint(GLfloat x, GLfloat y) {
    while (maxPoints > 0 && numPoints > maxPoints) {
      // // Cull the oldest points
      // numPoints--;
      // pointVertices.pop();
      //TODO implement this
    }
    pointVertices.push_back(x);
    pointVertices.push_back(y);
    numPoints++;
  }

  void PointLayer::clearPoints() {
    pointVertices.clear();
    numPoints = 0;
  }

  void PointLayer::render() {
    // glPointSize(2.0f);
    glColor4f(
      color.R,
      color.G,
      color.B,
      color.A
    );
    glVertexPointer(
      2,  // how many points per vertex (for us, that's x and y)
      GL_FLOAT, // the type of data being passed
      0, // byte offset between vertices
      &pointVertices[0]
    );
    glDrawArrays(
      GL_POINTS,
      0, // The starting index of the array
      numPoints
    );
  }

  void PointLayer::setColor(const ColorFloat& c) {
    color = c;
  }
};
