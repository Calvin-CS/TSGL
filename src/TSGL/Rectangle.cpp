#include "Rectangle.h"

namespace tsgl {

  Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color, bool filled = true) {
    shapeColor = color;
    vertices[0] = x;
    vertices[1] = y;
    vertices[6] = x + width;
    vertices[7] = y;
    vertices[12] = x;
    vertices[13] = y + height;
    vertices[18] = x + width;
    vertices[19] = y + height;
    vertices[2] = vertices[8] = vertices[14] = vertices[20] = color.R;  // Color of the coords
    vertices[3] = vertices[9] = vertices[15] = vertices[21] = color.G;
    vertices[4] = vertices[10] = vertices[16] = vertices[22] = color.B;
    vertices[5] = vertices[11] = vertices[17] = vertices[23] = color.A;
  }

  float* Rectangle::getVerticesPointerForRenderer() {
    //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
    float* temp = new float[1];
    return temp;
  }

  void Rectangle::render() {
    glColor4f( shapeColor.R, shapeColor.G, shapeColor.B, shapeColor.A );
    // // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    // // glClear(GL_COLOR_BUFFER_BIT);
    glBegin( GL_QUADS );
      glVertex2f( vertices[0], vertices[1] );
      glVertex2f(  vertices[6], vertices[1] );
      glVertex2f(  vertices[6], vertices[13] );
      glVertex2f(  vertices[0], vertices[13] );
    glEnd();

    // float v[8] = {vertices[0], vertices[1], vertices[6], vertices[1], vertices[6], vertices[13], vertices[0], vertices[13]};

    // struct LVertexPos2D
    // {
    //     GLfloat x;
    //     GLfloat y;
    // };
    //
    // LVertexPos2D gQuadVertices[4];
    // gQuadVertices[ 0 ].x = vertices[0];
    // gQuadVertices[ 0 ].y = vertices[1];
    //
    // gQuadVertices[ 1 ].x = vertices[6];
    // gQuadVertices[ 1 ].y = vertices[1];
    //
    // gQuadVertices[ 2 ].x = vertices[6];
    // gQuadVertices[ 2 ].y = vertices[13];
    //
    // gQuadVertices[ 3 ].x = vertices[0];
    // gQuadVertices[ 3 ].y = vertices[13];
    //
    // glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LVertexPos2D), gQuadVertices, GL_DYNAMIC_DRAW);
    // glDrawArrays(GL_QUADS, 0, 4);
    // printf("%s\n", "yo");

  }


  // Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color) {
  //     vertices[0] = x;
  //     vertices[1] = y;
  //     vertices[6] = x + width;
  //     vertices[7] = y;
  //     vertices[12] = x;
  //     vertices[13] = y + height;
  //     vertices[18] = x + width;
  //     vertices[19] = y + height;
  //     vertices[2] = vertices[8] = vertices[14] = vertices[20] = color.R;  // Color of the coords
  //     vertices[3] = vertices[9] = vertices[15] = vertices[21] = color.G;
  //     vertices[4] = vertices[10] = vertices[16] = vertices[22] = color.B;
  //     vertices[5] = vertices[11] = vertices[17] = vertices[23] = color.A;
  // }

  void Rectangle::draw() {
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

}
