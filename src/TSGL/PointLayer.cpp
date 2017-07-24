/*
 * PointLayer.cpp implements the methods defined in PointLayer.h
 */

#include "PointLayer.h"

namespace tsgl {

  PointLayer::PointLayer(const ColorFloat& c) {
    color = c; // TODO have all of these been written incorectly?  Check entire library for pointer issues
  }

  PointLayer::~PointLayer() {
    delete &pointVertices;
  }

  void PointLayer::addPoint(GLfloat x, GLfloat y) {
    pointVertices.push_back(x);
    pointVertices.push_back(y);
  }

  void PointLayer::setColor(const ColorFloat& c) {
    color = c;
  }
};
