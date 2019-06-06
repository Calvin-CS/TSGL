#include "RegularPolygon.h"

namespace tsgl {

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color, bool filled) : ConvexPolygon(sides, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), color);
  }
}

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat color[], bool filled) : ConvexPolygon(sides, filled, !filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), color[i]);
  }
}

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor, outlineColor);
  }
}

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor[i], outlineColor);
  }
}

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor, outlineColor[i]);
  }
}

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon(sides, true, true) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), fillColor[i], outlineColor[i]);
  }
}

} 