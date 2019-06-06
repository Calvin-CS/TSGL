#include "Star.h"

namespace tsgl {

Star::Star(float x, float y, float radius, int points, ColorFloat color, bool ninja, float rotation, bool filled) : ConcavePolygon(points*2, filled, !filled) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), color);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), color);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), color);
  }
}

Star::Star(float x, float y, float radius, int points, ColorFloat color[], bool ninja, float rotation, bool filled) : ConcavePolygon(points*2, filled, !filled) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), color[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), color[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), color[i]);
  }
}

Star::Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor, bool ninja, float rotation) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), fillColor, outlineColor);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), fillColor, outlineColor);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), fillColor, outlineColor);
  }
}

Star::Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor, bool ninja, float rotation) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), fillColor[i], outlineColor);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), fillColor[i], outlineColor);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), fillColor[i], outlineColor);
  }
}

Star::Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor[], bool ninja, float rotation) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), fillColor, outlineColor[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), fillColor, outlineColor[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), fillColor, outlineColor[i]);
  }
}

Star::Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor[], bool ninja, float rotation) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta + rotation), y+(radius/2)*sin(i*delta + rotation), fillColor[i], outlineColor[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta + rotation)), y+(radius*sin(i*delta + rotation)), fillColor[i], outlineColor[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta + rotation)), y+(radius*sin((i+0.5)*delta + rotation)), fillColor[i], outlineColor[i]);
  }
}
}