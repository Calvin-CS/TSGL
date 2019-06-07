#include "ConcavePolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConcavePolygon.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, bool filled, bool outlined) : Polygon(numVertices) {
  setup(numVertices, filled, outlined);
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with monocolored fill or outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConcavePolygon's fill or outline.
  *   \param filled Whether the ConcavePolygon should be filled
  *     (set to true by default).
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat color, bool filled) : Polygon(numVertices) {
  setup(numVertices, filled, !filled);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], color);
  }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with multicolored fill or outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConcavePolygon's fill or outline.
  *   \param filled Whether the ConcavePolygon should be filled
  *     (set to true by default).
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat color[], bool filled) : Polygon(numVertices) {
  setup(numVertices, filled, !filled);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], color[i]);
  }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with different monocolored fill and outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor The color of the ConcavePolygon's fill.
  *   \param outlineColor The color of the ConcavePolygon's outline.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) : Polygon(numVertices) {
  setup(numVertices, true, true);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], fillColor, outlineColor);
  }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with multicolored fill and monocolored outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor An array of colors for the ConcavePolygon's fill.
  *   \param outlineColor The color of the ConcavePolygon's outline.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) : Polygon(numVertices) {
  setup(numVertices, true, true);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], fillColor[i], outlineColor);
  }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with monocolored fill and multicolored outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor The color of the ConcavePolygon's fill.
  *   \param outlineColor An array of colors for the ConcavePolygon's outline.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) : Polygon(numVertices) {
  setup(numVertices, true, true);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], fillColor, outlineColor[i]);
  }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with different multicolored fill and outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor An array of colors for the ConcavePolygon's fill.
  *   \param outlineColor An array of colors for the ConcavePolygon's outline.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) : Polygon(numVertices) {
  setup(numVertices, true, true);
  for (int i = 0; i < numVertices; i++) {
      addVertex(x[i], y[i], fillColor[i], outlineColor[i]);
  }
}

/*!
 * \brief private helper method that works with the constructor
 * \details Defines a lot of instance variables, initializes vertices and outlineVertices arrays
 *    \param numVertices Number of vertices on the ConcavePolygon
 *    \param filled Whether or not the ConcavePolygon is filled.
 *    \param outlined Whether or not the ConcavePolygon is outlined.
 */
void ConcavePolygon::setup(int numVertices, bool filled, bool outlined) {
  numberOfOutlineVertices = numVertices+1;
  size = numberOfOutlineVertices * 6;
  tsize = 0;
  isFilled = filled;
  hasOutline = outlined;
  dirty = false;
  geometryType = GL_TRIANGLES;
  if(filled) {
    vertices = new float[size];
  }
  if(outlined) {
    outlineVertices = new float[size];
  }
}

 /*!
  * \brief Adds another vertex to a ConcavePolygon.
  * \details This function initializes the next vertex in the ConcavePolygon and adds it to a ConcavePolygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void ConcavePolygon::addVertex(float x, float y, const ColorFloat &color) {
  if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
  }
  if(isFilled) {
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
  }
  if(hasOutline) {
    outlineVertices[current] = x;
    outlineVertices[current + 1] = y;
    outlineVertices[current + 2] = color.R;
    outlineVertices[current + 3] = color.G;
    outlineVertices[current + 4] = color.B;
    outlineVertices[current + 5] = color.A;
  }
  current += 6;
  dirty = true;

  if (current == size-6) {
    if(isFilled) {
      vertices[current] = vertices[0];
      vertices[current + 1] = vertices[1];
      vertices[current + 2] = vertices[2];
      vertices[current + 3] = vertices[3];
      vertices[current + 4] = vertices[4];
      vertices[current + 5] = vertices[5];
    }
    if(hasOutline) {
      outlineVertices[current] = outlineVertices[0];
      outlineVertices[current + 1] = outlineVertices[1];
      outlineVertices[current + 2] = outlineVertices[2];
      outlineVertices[current + 3] = outlineVertices[3];
      outlineVertices[current + 4] = outlineVertices[4];
      outlineVertices[current + 5] = outlineVertices[5];
    }
    init = true;
    if(isFilled) {
      preprocess();
    }
  }
}

 /*!
  * \brief Adds another vertex to a ConcavePolygon.
  * \details This function initializes the next vertex in the ConcavePolygon and adds it to a ConcavePolygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param fillColor The reference variable of the fill color of the vertex.
  *      \param outlineColor The reference variable of the outline color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void ConcavePolygon::addVertex(float x, float y, const ColorFloat &fillColor, const ColorFloat &outlineColor) {
  if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
  }
  if(isFilled) {
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = fillColor.R;
    vertices[current + 3] = fillColor.G;
    vertices[current + 4] = fillColor.B;
    vertices[current + 5] = fillColor.A;
  }
  if(hasOutline) {
    outlineVertices[current] = x;
    outlineVertices[current + 1] = y;
    outlineVertices[current + 2] = outlineColor.R;
    outlineVertices[current + 3] = outlineColor.G;
    outlineVertices[current + 4] = outlineColor.B;
    outlineVertices[current + 5] = outlineColor.A;
  }
  current += 6;
  dirty = true;

  if (current == size-6) {
    if(isFilled) {
      vertices[current] = vertices[0];
      vertices[current + 1] = vertices[1];
      vertices[current + 2] = vertices[2];
      vertices[current + 3] = vertices[3];
      vertices[current + 4] = vertices[4];
      vertices[current + 5] = vertices[5];
    }
    if(hasOutline) {
      outlineVertices[current] = outlineVertices[0];
      outlineVertices[current + 1] = outlineVertices[1];
      outlineVertices[current + 2] = outlineVertices[2];
      outlineVertices[current + 3] = outlineVertices[3];
      outlineVertices[current + 4] = outlineVertices[4];
      outlineVertices[current + 5] = outlineVertices[5];
    }
    init = true;
    if(isFilled) {
      preprocess();
    }
  }
}

 /*!
  * \brief Determines if two lines intersect.
  * \details Simulates two lines inside of a ConcavePolygon object and determines whether
  * those two lines intersect.
  *      \param p0_x The x coordinate of the first point of the first line.
  *      \param p0_y The y coordinate of the first point of the first line.
  *      \param p1_x The x coordinate of the second point of the first line.
  *      \param p1_y The y coordinate of the second point of the first line.
  *      \param p2_x The x coordinate of the first point of the second line.
  *      \param p2_y The y coordinate of the first point of the second line.
  *      \param p3_x The x coordinate of the second point of the second line.
  *      \param p3_y The y coordinate of the second point of the second line.
  * \returns true if the lines do intersect, false if otherwise.
  */
bool ConcavePolygon::intersects(float p0_x, float p0_y, float p1_x, float p1_y,
                                float p2_x, float p2_y, float p3_x, float p3_y) {
  float s1_x, s1_y, s2_x, s2_y;
  s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
  s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

  float s, t;
  s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
  t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

  return (s >= 0 && s <= 1 && t >= 0 && t <= 1);
}

 /*!
  * \brief Determines whether a point resides inside of a Triangle.
  * \details Simulates a Triangle and point inside of a ConcavePolygon object and determines whether the point resides inside of
  * the Triangle.
  *   \param px The x coordinate of the point.
  *   \param py The y coordinate of the point.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  * \returns true if the point does reside in the Triangle, false if otherwise.
  */
bool ConcavePolygon::pointInTriangle (float px, float py, float x1, float y1, float x2, float y2, float x3, float y3)
{
  bool b1 = ( (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2) ) <= 0.0f;
  bool b2 = ( (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3) ) <= 0.0f;
  bool b3 = ( (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1) ) <= 0.0f;

  return ((b1 == b2) && (b2 == b3));
}

 /*!
  * \brief Draw the ConcavePolygon.
  * \details This function alters the vertices array so that it will render a Concave polygon correctly
  * \note This function does nothing if the vertex buffer is not yet full.
  * \warning This is an order of n-cubed operation, and is thus <b>VERY SLOW</b>.
  */
void ConcavePolygon::preprocess() {

  if (dirty) {
    dirty = false;
    std::queue<float> newvertices;

    bool clockwise = (
        (
            (vertices[6]-vertices[0]) * (vertices[13]-vertices[7]) -
            (vertices[7]-vertices[1]) * (vertices[12]-vertices[6])
        ) < 0.0);

    for (int i = 0; i < size-12; i += 6) {
      float x1 = vertices[i], y1 = vertices[i+1];
      for (int j = i+6; j < size-6; j += 6) {
        float x2 = vertices[j], y2 = vertices[j+1];
        for (int k = j+6; k < size; k += 6) {
          float x3 = vertices[k], y3 = vertices[k+1];

          bool open = true;
          for (int n = 0; n < size-6; n += 6) {
            float x4 = vertices[n], y4 = vertices[n+1], x5 = vertices[n+6],y5 = vertices[n+7];
            if (pointInTriangle(x4,y4,x1,y1,x2,y2,x3,y3) || pointInTriangle(x5,y5,x1,y1,x2,y2,x3,y3)) {
              open = false; break;
            }
            if (intersects(x1,y1,x2,y2,x4,y4,x5,y5)) {
              if ( !( (x1==x4 && y1==y4) || (x2==x5 && y2==y5) || (x1==x5 && y1==y5) || (x2==x4 && y2==y4)) ) {
                open = false; break;
              }
            }
            if (intersects(x2,y2,x3,y3,x4,y4,x5,y5)) {
              if ( !( (x2==x4 && y2==y4) || (x3==x5 && y3==y5) || (x2==x5 && y2==y5) || (x3==x4 && y3==y4)) ) {
                open = false; break;
              }
            }
            if (intersects(x1,y1,x3,y3,x4,y4,x5,y5)) {
              if ( !( (x1==x4 && y1==y4) || (x3==x5 && y3==y5) || (x1==x5 && y1==y5) || (x3==x4 && y3==y4)) ) {
                open = false; break;
              }
            }
          }

          //If the angle is not open (intersects something), draw nothing
              if (!open)
                continue;

              //If the angle is concave, draw nothing
              if ( ( ( (x2-x1) * (y3-y2) - (y2-y1) * (x3-x2) ) <= 0.0) != clockwise) {
                continue;
              }

              newvertices.push(x1);
              newvertices.push(y1);
              newvertices.push(vertices[i+2]);
              newvertices.push(vertices[i+3]);
              newvertices.push(vertices[i+4]);
              newvertices.push(vertices[i+5]);
              newvertices.push(x2);
              newvertices.push(y2);
              newvertices.push(vertices[j+2]);
              newvertices.push(vertices[j+3]);
              newvertices.push(vertices[j+4]);
              newvertices.push(vertices[j+5]);
              newvertices.push(x3);
              newvertices.push(y3);
              newvertices.push(vertices[k+2]);
              newvertices.push(vertices[k+3]);
              newvertices.push(vertices[k+4]);
              newvertices.push(vertices[k+5]);
        }
      }
    }

    tsize = newvertices.size();
    delete[] vertices;
    vertices = new float[tsize];
    for (int i = 0; i < tsize; ++i) {
      vertices[i] = newvertices.front();
      newvertices.pop();
    }

    numberOfVertices = tsize / 6;

  }

  //Debug Outline
  //    for (int i = 0; i < size; i += 6) {
  //      vertices[i+2] = 1.0f;
  //      vertices[i+3] = 1.0f;
  //      vertices[i+4] = 1.0f;
  //      vertices[i+5] = 1.0f;
  //    }
  //    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
  //    glDrawArrays(GL_LINE_STRIP, 0, length);
}

//----------------------------------------------Unit testing------------------------------
/*!
 * \brief Runs the Unit tests.
 * \details Runs the Unit tests for the ConcavePolygon class. intersects() and pointInTriangle() are tested.
 */
void ConcavePolygon::runTests() {
  TsglDebug("Testing ConcavePolygon class....");
  tsglAssert(testIntersects(), "Unit test for intersecting lines failed!");
  tsglAssert(testPointITriangle(), "Unit test for pointInTriangle() failed!");
  TsglDebug("Unit tests for ConcavePolygon complete.");
  std::cout << std::endl;
}

bool ConcavePolygon::testIntersects() {
  int passed = 0;
  int failed = 0;
  ConcavePolygon c1(10);

  //Test 1: Intersecting lines
  float x1, y1, x2, y2, x3, y3, x4, y4 = 0.0;
  x1 = 230.0;     //Set up the points so that it simulates two intersecting lines
  y1 = 230.0;
  x2 = 250.0;
  y2 = 400.0;
  x3 = 200.0;
  y3 = 250.0;
  x4 = 400.0;
  y4 = 250.0;

  //They SHOULD intersect
  if(c1.intersects(x1, y1, x2, y2, x3, y3, x4, y4)) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, Intersecting lines for testIntersects() failed!");
  }

  x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0.0;  //Reset the x's and y's for the next test

  //Test 2: Parallel lines
  x1 = 250.0;   //Set up the points so that it simulates two parallel lines in the Canvas
  y1 = 250.0;
  x2 = 250.0;
  y2 = 400.0;
  x3 = 200.0;
  y3 = 250.0;
  x4 = 200.0;
  y4 = 400.0;

  //They should NOT intersect, so if it returns false it passed
  if(c1.intersects(x1, y1, x2, y2, x3, y3, x4, y4)) {
    failed++;
    TsglErr("Test 1, Parallel lines for testIntersects() failed!");
  } else {
    passed++;
  }

  //Results
  if(passed == 2 && failed == 0) {
    TsglDebug("Unit test for intersecting lines passed!");
    return true;
  } else {
    TsglErr("This many passed for testIntersects(): ");
    std::cout << " " << passed << std::endl;
    TsglErr("This many failed for testIntersects(): ");
    std::cout << " " << failed << std::endl;
    return false;
  }
}

bool ConcavePolygon::testPointITriangle() {
    int passed = 0;
    int failed = 0;
    ConcavePolygon c2(10);
    //Test 1: Point is in the triangle
    float px, py, x1, y1, x2, y2, x3, y3 = 0.0;

    //Simulate a triangle and a point inside of it
    x1 = 40.0;  //Vertices...
    y1 = 250.0;
    x2 = 50.0;
    y2 = 80.0;
    x3 = 250.0;
    y3 = 150.0;
    px = 50.0;   //Point coordinates...
    py = 175.0;

    //Point SHOULD be in triangle
    if(c2.pointInTriangle(px, py, x1, y1, x2, y2, x3, y3)) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Point = in triangle for testPointITriangle() failed!");
    }

    //Test 2: Point not in triangle
    px = py = 0.0;  //Reset the point

    px = 400.0;    //Give it new coordinates
    py = 400.0;

    //Point should NOT be in triangle, so it should return false
    if(c2.pointInTriangle(px, py, x1, y1, x2, y2, x3, y3)) {
      failed++;
      TsglErr("Test 2, Point = NOT in triangle for testPointITriangle() failed!");
    } else {
      passed++;
    }

    //Results:
    if(passed == 2 && failed == 0) {
      TsglDebug("Unit test for point in triangle passed!");
      return true;
    } else {
      TsglErr("This many passed for testPointITriangle(): ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many failed for testPointITriangle(): ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}
//---------------------------------------------End Unit testing-----------------------
}
