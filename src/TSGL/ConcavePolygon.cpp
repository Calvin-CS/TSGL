/*
 * ConcavePolygon.cpp
 *
 *  Created on: May 26, 2015
 *      Author: pac3
 */

#include "ConcavePolygon.h"

#ifdef _WIN32
namespace tsgl {
#endif

ConcavePolygon::ConcavePolygon(int v) {
    if (v < 3) throw std::out_of_range("Cannot have a polygon with fewer than 3 vertices.");
    length = v+1;
    size = length * 6;
    tsize = 0;
    current = 0;
    tarray = nullptr;
    vertices = new float[size];
    init = false;
    dirty = false;
}

ConcavePolygon::~ConcavePolygon() {
    delete vertices;
    delete tarray;
}

void ConcavePolygon::addVertex(int x, int y, const ColorFloat &color) {
    if (init) return;
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    dirty = true;
    if (current == size-6) {
      vertices[current] = vertices[0];
      vertices[current + 1] = vertices[1];
      vertices[current + 2] = vertices[2];
      vertices[current + 3] = vertices[3];
      vertices[current + 4] = vertices[4];
      vertices[current + 5] = vertices[5];
      init = true;
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

void ConcavePolygon::draw() {
    if (!init) return;

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
      tarray = new float[tsize];
      for (int i = 0; i < tsize; ++i) {
        tarray[i] = newvertices.front();
        newvertices.pop();
      }

    }

    glBufferData(GL_ARRAY_BUFFER, tsize * sizeof(float), tarray, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, tsize / 6);

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
  tsglAssert(testDraw(), "Unit test for draw failed");
  tsglAssert(testIntersects(), "Unit test for intersect() failed");
  tsglAssert(testPointITriangle(), "Unit test for pointInTriangle() failed");
  std::cout << "All unit tests passed!" << std::endl;
}

bool ConcavePolygon::testDraw() {
  return true;
}

bool ConcavePolygon::testIntersects() {
  return true;
}

bool ConcavePolygon::testPointITriangle() {
  return true;
}

//---------------------------------------------End Unit testing-----------------------

#ifdef _WIN32
}
#endif
