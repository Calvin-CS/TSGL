#include "ConcavePolygon.h"

namespace tsgl {

ConcavePolygon::ConcavePolygon(int numVertices) : Shape() {
  if (numVertices < 3)
    TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
  length = numVertices+1;
  size = length * 6;
  tsize = 0;
  tarray = new float[size];
  dirty = false;
  geometryType = GL_TRIANGLES;
}

ConcavePolygon::~ConcavePolygon() {
  delete[] tarray;
}

void ConcavePolygon::addVertex(float x, float y, const ColorFloat &color) {
  if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
  }
  tarray[current] = x;
  tarray[current + 1] = y;
  tarray[current + 2] = color.R;
  tarray[current + 3] = color.G;
  tarray[current + 4] = color.B;
  tarray[current + 5] = color.A;
  current += 6;
  dirty = true;

  if (current == size-6) {
    tarray[current] = tarray[0];
    tarray[current + 1] = tarray[1];
    tarray[current + 2] = tarray[2];
    tarray[current + 3] = tarray[3];
    tarray[current + 4] = tarray[4];
    tarray[current + 5] = tarray[5];
    init = true;
    preprocess();
  }
}

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

bool ConcavePolygon::pointInTriangle (float px, float py, float x1, float y1, float x2, float y2, float x3, float y3)
{
  bool b1 = ( (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2) ) <= 0.0f;
  bool b2 = ( (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3) ) <= 0.0f;
  bool b3 = ( (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1) ) <= 0.0f;

  return ((b1 == b2) && (b2 == b3));
}

void ConcavePolygon::preprocess() {

  if (dirty) {
    dirty = false;
    std::queue<float> newvertices;

    bool clockwise = (
        (
            (tarray[6]-tarray[0]) * (tarray[13]-tarray[7]) -
            (tarray[7]-tarray[1]) * (tarray[12]-tarray[6])
        ) < 0.0);

    for (int i = 0; i < size-12; i += 6) {
      float x1 = tarray[i], y1 = tarray[i+1];
      for (int j = i+6; j < size-6; j += 6) {
        float x2 = tarray[j], y2 = tarray[j+1];
        for (int k = j+6; k < size; k += 6) {
          float x3 = tarray[k], y3 = tarray[k+1];

          bool open = true;
          for (int n = 0; n < size-6; n += 6) {
            float x4 = tarray[n], y4 = tarray[n+1], x5 = tarray[n+6],y5 = tarray[n+7];
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
              newvertices.push(tarray[i+2]);
              newvertices.push(tarray[i+3]);
              newvertices.push(tarray[i+4]);
              newvertices.push(tarray[i+5]);
              newvertices.push(x2);
              newvertices.push(y2);
              newvertices.push(tarray[j+2]);
              newvertices.push(tarray[j+3]);
              newvertices.push(tarray[j+4]);
              newvertices.push(tarray[j+5]);
              newvertices.push(x3);
              newvertices.push(y3);
              newvertices.push(tarray[k+2]);
              newvertices.push(tarray[k+3]);
              newvertices.push(tarray[k+4]);
              newvertices.push(tarray[k+5]);
        }
      }
    }

    tsize = newvertices.size();
    // delete[] vertices;
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
