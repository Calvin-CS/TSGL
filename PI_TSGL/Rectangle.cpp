#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height, int winWidth,
		     int winHeight, const ColorFloat &color, GLuint posAttrib,
		     GLuint colorAttrib) {
  positionLoc = posAttrib;
  colorLoc = colorAttrib;
  windowWidth = winWidth;
  windowHeight = winHeight;
  
  positionData[0] = ((x/windowWidth) - ((windowWidth-x)/windowWidth));
  positionData[1] = ((-y/windowHeight) + ((windowHeight-y)/windowHeight));
  positionData[2] = (((x+width)/windowWidth) -
		     (windowWidth-(x+width))/windowWidth);
  positionData[3] = ((-y/windowHeight) + ((windowHeight-y)/windowHeight));
  positionData[4] = ((x/windowWidth) - ((windowWidth-x)/windowWidth));
  positionData[5] = ((-(y+height)/windowHeight) +
		     (windowHeight-(y+height))/windowHeight);
  positionData[6] = (((x+width)/windowWidth) -
		     (windowWidth-(x+width))/windowWidth);
  positionData[7] = ((-(y+height)/windowHeight) +
		     (windowHeight-(y+height))/windowHeight);
  
  colorData[0] = colorData[4] = colorData[8] = colorData[12] = color.R;
  colorData[1] = colorData[5] = colorData[9] = colorData[13] = color.G;

  colorData[2] = colorData[6] = colorData[10] = colorData[14] = color.B;
  colorData[3] = colorData[7] = colorData[11] = colorData[15] = color.A;
}

void Rectangle::draw(GLuint vertexArray, GLuint shaderProgram) {
  glBindVertexArray(vertexArray);
  
  GLuint positionBuffer;
  glGenBuffers(1, &positionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positionData),
	       positionData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE,
			0,0);

  GLuint colorBuffer;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(colorLoc);
  glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE,
  			0,0);

  glBindVertexArray(0);

  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glFlush();
  glBindVertexArray(0);
}
			  
