#include "Polyline.h"

Polyline::Polyline(int numVertices, int winWidth, int winHeight, GLuint posAttrib, GLuint colorAttrib){
  if(numVertices < 2)
    TsglDebug("cannot have a line with fewer than 2 vertices");

  windowWidth = winWidth;
  windowHeight = winHeight;
  
  positionLoc = posAttrib;
  colorLoc = colorAttrib;

  length = numVertices;

  positionSize = length * 2;
  colorSize = length * 4;

  currentPosition = 0;
  currentColor = 0;
  
  positionData = new GLfloat[positionSize];
  colorData = new GLfloat[colorSize];

  init = false;
}

Polyline::~Polyline(){
  delete[] positionData;
  delete[] colorData;
}

void Polyline::addNextVertex(int x, int y, const ColorFloat &color){
  if (init) {
    TsglDebug("Cannot add more vertices");
    return;
  }

  positionData[currentPosition] = ((x/windowWidth) -
				   ((windowWidth-x)/windowWidth));
  positionData[currentPosition + 1] = ((-y/windowHeight) +
				       ((windowHeight-y)/windowHeight));
  currentPosition += 2;

  colorData[currentColor] = color.R;
  colorData[currentColor + 1] = color.G;
  colorData[currentColor + 2] = color.B;
  colorData[currentColor + 3] = color.A;
  currentColor += 4;

  if(currentPosition == positionSize && currentColor == colorSize)
    init = true;
}

void Polyline::draw(GLuint vertexArray, GLuint shaderProgram){
  if(!init || length < 2){
    TsglDebug("Cannot draw uninitialized Polyline.");
    return;
  }

  glBindVertexArray(vertexArray);

  GLuint positionBuffer;
  glGenBuffers(1, &positionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, positionSize*sizeof(GLfloat),
	       positionData, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLuint colorBuffer;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, colorSize*sizeof(GLfloat),
	       colorData, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(colorLoc);
  glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_LINE_STRIP, 0, length);
  glBindVertexArray(0);
}
