#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Shape.h"

class Polyline : public Shape {
 private:
  bool init;
  GLfloat* positionData;
  GLfloat* colorData;
  int positionSize, colorSize, currentPosition, currentColor, length;
  GLuint positionLoc, colorLoc;
  float windowWidth, windowHeight;

 public:
  Polyline(int numVertices, int winWidth, int winHeight, GLuint posAttrib, GLuint colorAttrib);

  ~Polyline();

  void addNextVertex(int x, int y, const ColorFloat &color = BLACK);

  void draw(GLuint vertexArray, GLuint shaderProgram);
};

#endif
