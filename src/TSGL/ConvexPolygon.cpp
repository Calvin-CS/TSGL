#include "ConvexPolygon.h"

namespace tsgl {

ConvexPolygon::ConvexPolygon(int numVertices) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    length = numVertices;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
}

ConvexPolygon::~ConvexPolygon() {
    delete[] vertices;
}

void ConvexPolygon::addVertex(int x, int y, const ColorFloat &color) {
    if (init) {
      TsglDebug("Cannot add anymore vertices.");
      return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == size) init = true;
}

void ConvexPolygon::draw() {
    if (!init) {
      TsglDebug("Cannot draw yet.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, length);
}

float* ConvexPolygon::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

void ConvexPolygon::runTests() {
    TsglDebug("Testing ConvexPolygon class...");
    tsglAssert(testAddVertex(), "Unit test for adding vertices failed");
    TsglDebug("Unit tests for ConvexPolygon complete.");
    std::cout << std::endl;
}

bool ConvexPolygon::testAddVertex() {
    int passed = 0;
    int failed = 0;
    ConvexPolygon c1(4);  //Start with a polygon that has the capacity for four vertices
    //Test 1: Adding a single vertex
    if(c1.size == 24 && c1.current == 0) {   //As it should be...
      c1.addVertex(20, 30, BLACK);  //Add a vertex; only the current should change...
      if(c1.size == 24 && c1.current == 6) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 1, One vertex for testAddVertex() failed!");
      }
    }

    //Test 2: Filling the polygon vertices completely
    //Add three more vertices, checking if the current has changed correspondingly
    c1.addVertex(20, 30, BLACK);
    c1.addVertex(20, 30, BLACK);
    c1.addVertex(20, 30, BLACK);

    //The size should've remained the same but the current should now equal the size
    if(c1.size == 24 && c1.current == 24) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Filling vertices completely for testAddVertex() failed!");
    }

    //Test 3: Attempting to add another vertex to a filled vertices....
    c1.addVertex(20, 30, BLACK);
    //The size and current should've stayed the same
    if(c1.size == 24 && c1.current == 24) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Add more beyond capacity for testAddVertex() failed!");
    }

    if(passed == 3 && failed == 0) {
      TsglDebug("Unit test for adding vertices passed!");
      return true;
    } else {
      TsglErr("This many tests for ConvexPolygon passed: ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many tests for ConvexPolygon failed: ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}

void ConvexPolygon::setColor(ColorFloat c) {
  for(int i = 2; i < size; i+=6) { //Set Red values
    vertices[i] = c.R;
  }
  for(int i = 3; i < size; i+=6) { //Set Green values
    vertices[i] = c.G;
  }
  for(int i = 4; i < size; i+=6) { //Set Blue values
    vertices[i] = c.B;
  }
  for(int i = 5; i < size; i+=6) { //Set Alpha values
    vertices[i] = c.A;
  }
}

// void ConvexPolygon::findCenter() {
//   int minX = maxX = vertices[0];
//   int minY = maxY = vertices[1];
//
//   //Find min and max X
//   for(int i = 0; i < length; i++) {
//     if( vertices[i*6] < minX )
//       minX = vertices[i*6];
//     else if( vertices[i*6] > maxX )
//       maxX = vertices[i*6];
//   }
//
//   //Find min and max Y
//   for(int i = 0; i < length; i++) {
//     if( vertices[i*6+1] < minY )
//       minY = vertices[i*6+1];
//     else if( vertices[i*6+1] > maxY )
//       maxY = vertices[i*6+1];
//   }
//
//   centerX = (minX+maxX)/2;
//   centerY = (minY+maxY)/2;
// }

}
