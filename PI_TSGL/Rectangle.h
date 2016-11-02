#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape {
 private:
  GLfloat positionData[8];
  GLfloat colorData[16];
  GLuint positionLoc;
  GLuint colorLoc;
  float windowWidth;
  float windowHeight;

 public:
  Rectangle(int x, int y, int width, int height, int winWidth, int winHeight,
	    const ColorFloat &color, GLuint posAttrib, GLuint colorAttrib);

  void draw(GLuint vertexArray, GLuint shaderProgram);
};

#endif
