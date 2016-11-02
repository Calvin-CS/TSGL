#ifndef _SHAPE_
#define _SHAPE_

#include <GL/glew.h>
#include "Color.h"

class Shape {
 protected:
  bool isTextured;
  
 public:
  Shape() { isTextured = false; }

  virtual ~Shape() {}

  virtual void draw(GLuint vertexArray, GLuint shaderProgram) = 0;

  bool getIsTextured() { return isTextured; }
};

#endif
